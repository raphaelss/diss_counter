#include "diss_counter.h"
#include <stdlib.h>

struct diss_counter_entry {
	void *x;
	unsigned count;
	diss_counter_float prob;
};

struct diss_counter {
	diss_counter_float sum;
	diss_counter_prob_f prob_f;
	void *data;
	struct diss_counter_entry *end;
	struct diss_counter_entry table [];
};

struct diss_counter *
diss_counter_make(void *elems, unsigned size, unsigned n,
    diss_counter_prob_f f, void *data)
{
	struct diss_counter *dc =
	    malloc(sizeof(*dc) + n * sizeof(struct diss_counter_entry));
	if (!dc) {
		return NULL;
	}
	dc->prob_f = f;
	dc->data = data;
	dc->sum = 0;
	char *elem_iter = elems;
	struct diss_counter_entry *table_iter = dc->table;
	while (n--) {
		table_iter->x = elem_iter;
		table_iter->count = 1;
		table_iter->prob = f(elem_iter, 1, dc->data);
		dc->sum += table_iter->prob;
		++table_iter;
		elem_iter += size;
	}
	dc->end = table_iter;
	return dc;
}

void
diss_counter_destroy(struct diss_counter *dc)
{
	free(dc);
}

void *
diss_counter_next(struct diss_counter *dc)
{
	diss_counter_float r = (double)rand()/(double)RAND_MAX * dc->sum;
	dc->sum = 0;
	void *chosen = NULL;
	struct diss_counter_entry *iter = dc->end;
	while (--iter >= dc->table) {
		if (chosen) {
			++iter->count;
		} else {
			r -= iter->prob;
			if (r <= 0) {
				chosen = iter->x;
				iter->count = 0;
			} else {
				++iter->count;
			}
		}
		iter->prob = dc->prob_f(iter->x, iter->count, dc->data);
		dc->sum += iter->prob;
	}
	return chosen;
}

unsigned
diss_counter_n(const struct diss_counter *dc)
{
	return dc->end - dc->table;
}

void
diss_counter_set_counts(struct diss_counter *dc, unsigned *counts)
{
	dc->sum = 0;
	struct diss_counter_entry *iter = dc->end;
	while (--iter >= dc->table) {
		iter->count = *counts++;
		iter->prob = dc->prob_f(iter->x, iter->count, dc->data);
		dc->sum += iter->prob;
	}
}

void *
diss_counter_get_data(struct diss_counter *dc)
{
	return dc->data;
}
