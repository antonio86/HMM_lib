#include <stdio.h>
#include <stdlib.h>
#include "rand_num.h"

float rand_num(float max) {
	if (max != 0)
		return ((float)rand() / (float)(RAND_MAX)) * max;
	else
		return max;
}
