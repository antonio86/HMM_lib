#include <stdlib.h>
#include <string.h>
#include "../include/model.h"
#include "../include/viterbi_log.h"

int test_matrix(model_t* foo_mat, double **Test_Vect, int T, int NClasses, int *Align, float *Pvit_w){

	// parameter initialization
	int winner = -1;
	float max_prob = -3.4e+38;
	int nC;

	for (nC = 0; nC < NClasses; nC++){

		// charge a single struct
		//struct Matrix mat_class = foo_mat[nC];

		// parameter initialization
		int *X = calloc(T, sizeof(int));
		float *Pvit = calloc(1, sizeof(float));

		// computation of the X and Prob float
		viterbi_log(foo_mat, Test_Vect, T, X, Pvit);
		Pvit_w[nC] = *Pvit;

		// to avoid to stock all the results, save only the best
		if (*Pvit > max_prob){
			memcpy(Align, X, T*sizeof(int));
			winner = nC;
			max_prob = *Pvit;
		}

		// free the allocated memory
		free(X);
		free(Pvit);
	}

	// return the winner
	return winner+1;
}
