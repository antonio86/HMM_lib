#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/normalize.h"
#include "../include/model.h"
#include "../include/rand_num.h"
#include "../include/viterbi_log.h"

void test_histogram(model_t * model, double ** test_vect, int sample_length, int NClasses, int *Align){

	// parameter initialization
	int winner = -1;
	double max_prob = -3.4e+38;

	// pre-processing operations
	for (int i = 0; i < model->numberDimensions; i++)
		for (int j = 0; j < sample_length; j++)
			test_vect[i][j] = test_vect[i][j] + rand_num(2 * model->noise) - model->noise; 

	// normalization
	for (int pos = 0; pos < sample_length; pos++)
		normalize(model, test_vect, pos);

	// initialization of the test parameters
	double *Pvit_t = (double *)calloc(NClasses, sizeof(double));

	for (int nC = 0; nC < NClasses; nC++){

		// parameter initialization
		int *X = calloc(sample_length, sizeof(int));
		double *Pvit = calloc(1, sizeof(double));

		// computation of the X and Prob double
		viterbi_log(model, test_vect, sample_length, X, Pvit);

		Pvit_t[nC] = *Pvit;

		// to avoid to stock all the results, save only the best
		if (*Pvit > max_prob){
			memcpy(Align, X, sample_length*sizeof(int));
			winner = nC;
			max_prob = *Pvit;
		}

		// free the allocated memory
		free(X);
		free(Pvit);
	}

	// and the winner is:
	winner += 1;

	free(Pvit_t);

}