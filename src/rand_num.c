#include <stdio.h>
#include <stdlib.h>
#include "../include/rand_num.h"

double rand_num(double max) {
	if (max != 0)
		return ((double)rand() / (double)(RAND_MAX)) * max;
	else
		return max;
}
