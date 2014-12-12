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
	puts("");
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
	return pow(count, *(double*)data);
}

