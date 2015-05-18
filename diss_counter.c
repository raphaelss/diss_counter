#include "diss_counter.h"
#include <stdlib.h>
#include <string.h>

#define ENTRY_SIZE(dc) (sizeof(struct diss_counter_entry) + (dc)->size)
#define STEP_TABLE_ITER(table_iter, dc) \
	((struct diss_counter_entry *)((char *)(table_iter) + ENTRY_SIZE(dc)))

struct diss_counter_entry {
	unsigned count;
	double prob;
	char x [];
};

struct diss_counter {
	double sum;
	unsigned size;
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
	    malloc(sizeof(*dc) + n * (sizeof(struct diss_counter_entry) + size));
	if (!dc) {
		return NULL;
	}
	dc->size = size;
	dc->prob_f = f;
	dc->data = data;
	dc->sum = 0;
	char *elem_iter = elems;
	struct diss_counter_entry *table_iter = dc->table;
	while (n--) {
		memcpy(table_iter->x, elem_iter, size);
		table_iter->count = 1;
		table_iter->prob = f(elem_iter, 1, dc->data);
		dc->sum += table_iter->prob;
		table_iter = STEP_TABLE_ITER(table_iter, dc);
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
	double r =
	    (double)rand()/(double)RAND_MAX * dc->sum;
	dc->sum = 0;
	void *chosen = NULL;
	struct diss_counter_entry *iter = dc->table;
	for (; iter < dc->end; iter = STEP_TABLE_ITER(iter, dc)) {
		if (chosen) {
			++iter->count;
		} else {
			r -= iter->prob;
			if (r <= 0) {
				chosen = &iter->x;
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
diss_counter_reset_counts(struct diss_counter *dc, unsigned c)
{
	struct diss_counter_entry *iter = dc->end;
	dc->sum = 0;
	while (--iter >= dc->table) {
		iter->count = c;
		iter->prob = dc->prob_f(iter->x, iter->count, dc->data);
		dc->sum += iter->prob;
	}
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

void *
diss_counter_get_elem(struct diss_counter *dc, unsigned i)
{
	return dc->table[i].x;
}

void
diss_counter_iter(struct diss_counter *dc, diss_counter_iter_f f, void *data)
{
	double old_sum = dc->sum;
	dc->sum = 0;
	struct diss_counter_entry *iter = dc->end;
	while (--iter >= dc->table) {
		f(iter->x, &iter->count, iter->prob / old_sum, data);
		iter->prob = dc->prob_f(iter->x, iter->count, dc->data);
		dc->sum += iter->prob;
	}
}

void
diss_counter_set_prob_f(struct diss_counter * dc, diss_counter_prob_f f)
{
	dc->prob_f = f;
	dc->sum = 0;
	struct diss_counter_entry  *iter = dc->end;
	while (--iter >= dc->table) {
		iter->prob = dc->prob_f(iter->x, iter->count, dc->data);
		dc->sum += iter->prob;
	}
}

