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
typedef int (*diss_counter_cmp_f)(void *, void *, void *);

struct diss_counter;

/**
* Allocates memory, initializes and returns a struct diss_counter with the elements
* provided on the first argument, second argument is the size of these elements,
* third argument is their number and the other arguments are the function to 
* calculate probability and a data that's stored on the struct and passed to it
* on every call. Return NULL on failure.
**/
struct diss_counter *diss_counter_make(void *, size_t, size_t,
    diss_counter_prob_f, void *);

/**
* Frees the memory associated with the struct. It is unusable after that. Does
* nothing if it's called on NULL.
**/
void diss_counter_destroy(struct diss_counter *);

/**
* Return the "next" element of the sequence and updates the counters and
* probabilities of the elements internally.
*/
void *diss_counter_next(struct diss_counter *);

/**
* Returns the number of elements contained on the struct diss_counter.
*/
size_t diss_counter_length(const struct diss_counter *);

/**
* Sets the count of every element to the number passed as parameter.
*/
void diss_counter_reset_counts(struct diss_counter *, unsigned);

/**
* Returns a pointer the associated data given in the make function that
* is passed to the function to calculate probabilities
**/
void *diss_counter_get_data(struct diss_counter *);

/**
* Set the data that is passed to the prob_fun on every call. An additional
* flag passed as parameter determines (when true) if the probabilities
* must be recalculated.
**/
void diss_counter_set_data(struct diss_counter *, void *, int);

/**
* Returns the element at the index.
**/
void *diss_counter_get_elem(struct diss_counter *, size_t);

/**
* Returns the count of the element at the index.
**/
unsigned diss_counter_get_count(struct diss_counter *, size_t);

/**
* Sets the count of the element at the index. Its probability is recalculated.
**/
void diss_counter_set_count(struct diss_counter *, size_t, unsigned);

/**
* Searches the contained elements for an element that is equal to the obj passed
* as parameter using the comparison function. An additional data is passed as
* parameter and is used as the third argument of the comparison function.
**/
size_t diss_counter_get_index(struct diss_counter *, void *, diss_counter_cmp_f, void *);

/**
* Sets the probabiity function. Every probability is recalculated.
**/
void diss_counter_set_prob_fun(struct diss_counter *, diss_counter_prob_f);

#endif