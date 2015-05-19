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

#ifndef _DISS_COUNTER_H_INCLUDED_
#define _DISS_COUNTER_H_INCLUDED_
#include <stdlib.h>

typedef double (*diss_counter_prob_f)(void *, unsigned, void *);
typedef void (*diss_counter_iter_f)(void *, unsigned *, double, void *);

struct diss_counter;

struct diss_counter *diss_counter_make(void *, size_t, size_t,
    diss_counter_prob_f, void *);
void diss_counter_destroy(struct diss_counter *);
void *diss_counter_next(struct diss_counter *);
size_t diss_counter_length(const struct diss_counter *);
void diss_counter_reset_counts(struct diss_counter *, unsigned);
void diss_counter_set_counts(struct diss_counter *, unsigned *);
void *diss_counter_get_data(struct diss_counter *);
void *diss_counter_get_elem(struct diss_counter *, unsigned);
void diss_counter_iter(struct diss_counter *, diss_counter_iter_f, void *);
void diss_counter_set_prob_fun(struct diss_counter *, diss_counter_prob_f);

#endif