/**
* diss_counter - a dissonant counterpoint algorithm implementation
* Copyright (C) 2015  Raphael Sousa Santos, http://www.raphaelss.com
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/

#include "diss_counter.h"
#include <string.h>

#define ENTRY_SIZE(dc) (sizeof(struct diss_counter_entry) + (dc)->obj_size)
#define STEP_TABLE_ITER(table_iter, dc) \
	((struct diss_counter_entry *)(((char *)table_iter) + ENTRY_SIZE(dc)))
#define TABLE_LOOP(pair, dc) \
	for (struct {size_t index; struct diss_counter_entry *iter;} pair = { 0, dc->table}; \
	    pair.index < dc->length; \
	    pair.iter = STEP_TABLE_ITER(pair.iter, dc), ++pair.index)
#define AT_INDEX(dc, i) \
	(*((struct diss_counter_entry *)(((char *)(dc)->table) + ((i) * ENTRY_SIZE(dc)))))

struct diss_counter_entry {
	unsigned count;
	double prob;
	char obj [];
};

struct diss_counter {
	void *data;
	diss_counter_prob_f prob_f;
	double sum;
	size_t obj_size;
	size_t length;
	struct diss_counter_entry table [];
};

static void
recalc_probs(struct diss_counter *dc)
{
	dc->sum = 0;
	TABLE_LOOP(pair, dc) {
		pair.iter->prob = dc->prob_f(pair.iter->obj, pair.iter->count, dc->data);
		dc->sum += pair.iter->prob;
	}
}

struct diss_counter *
diss_counter_make(void *elems, size_t size, size_t n,
    diss_counter_prob_f f, void *data)
{
	struct diss_counter *dc =
	    malloc(sizeof(*dc) + n * (sizeof(struct diss_counter_entry) + size));
	if (!dc) {
		return NULL;
	}
	dc->obj_size = size;
	dc->prob_f = f;
	dc->data = data;
	dc->sum = 0;
	char *elem_iter = elems;
	dc->length = n;
	TABLE_LOOP(pair, dc) {
		memcpy(pair.iter->obj, elem_iter, size);
		pair.iter->count = 1;
		pair.iter->prob = f(elem_iter, 1, dc->data);
		dc->sum += pair.iter->prob;
		elem_iter += size;
	}
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
	TABLE_LOOP(pair, dc) {
		if (chosen) {
			++pair.iter->count;
		} else {
			r -= pair.iter->prob;
			if (r <= 0) {
				chosen = &pair.iter->obj;
				pair.iter->count = 0;
			} else {
				++pair.iter->count;
			}
		}
		pair.iter->prob =
		    dc->prob_f(pair.iter->obj, pair.iter->count, dc->data);
		dc->sum += pair.iter->prob;
	}
	return chosen;
}

size_t
diss_counter_length(const struct diss_counter *dc)
{
	return dc->length;
}

void
diss_counter_reset_counts(struct diss_counter *dc, unsigned c)
{
	dc->sum = 0;
	TABLE_LOOP(pair, dc) {
		pair.iter->count = c;
		pair.iter->prob = dc->prob_f(pair.iter->obj, pair.iter->count, dc->data);
		dc->sum += pair.iter->prob;
	}
}

void *
diss_counter_get_data(struct diss_counter *dc)
{
	return dc->data;
}

void
diss_counter_set_data(struct diss_counter *dc, void *data, int recalc)
{
	dc->data = data;
	if (recalc) {
		recalc_probs(dc);
	}
}

void *
diss_counter_get_elem(struct diss_counter *dc, size_t i)
{
	return AT_INDEX(dc, i).obj;
}

unsigned
diss_counter_get_count(struct diss_counter *dc, size_t i)
{
	return AT_INDEX(dc, i).count;
}

void
diss_counter_set_count(struct diss_counter *dc, size_t i, unsigned count)
{
	struct diss_counter_entry *entry = &AT_INDEX(dc, i);
	entry->count = count;
	dc->sum -= entry->prob;
	dc->sum += (entry->prob = dc->prob_f(entry->obj, entry->count, dc->data));
}

size_t
diss_counter_get_index(struct diss_counter *dc , void *obj, diss_counter_cmp_f f, void *data)
{
	TABLE_LOOP(pair, dc) {
		if (f(pair.iter->obj, obj, data)) {
			return pair.index;
		}
	}
	return dc->length;
}


void
diss_counter_set_prob_f(struct diss_counter * dc, diss_counter_prob_f f)
{
	dc->prob_f = f;
	recalc_probs(dc);
}

