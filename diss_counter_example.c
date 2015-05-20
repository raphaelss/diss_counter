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

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "diss_counter.h"

void usage(void);
double power_fun(void *x, unsigned count, void *data);

int
main(int argc, char **argv)
{
	srand(time(NULL));
	int n;
	if (argc < 5 || !(n = atoi(argv[1]))) {
		usage();
		return 1;
	}
	double alpha = atof(argv[2]);
	struct diss_counter *dc = diss_counter_make(argv + 3, sizeof(char *),
	    argc - 3, power_fun, &alpha);
	for (int i = 0; i < n; ++i) {
		printf("%s ", *(char **)diss_counter_next(dc));
	}
	putchar('\n');
	diss_counter_destroy(dc);
	return 0;
}

void
usage(void)
{
	puts("Usage: diss_counter_example n alpha [values...]");
}

double
power_fun(void *x, unsigned count, void *data)
{
	return pow(count, *((double*)data));
}