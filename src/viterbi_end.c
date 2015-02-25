#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/viterbi_end.h"
#include "../include/configuration.h"

void maxx(double *Ar, int dim, double *val, int *pos){

	*val = -FLT_MAX;
	*pos = 0;
	int n;
	for (n = 0; n < dim; n++){
		if (Ar[n] > *val){
			*val = Ar[n];
			*pos = n;
		}

	}
}

void viterbi_end(double *FI, int *XX, double *A_Vect, double *LogPdf, double *Pvit, int N, int T){

	// end of viterbi
	double *temp = (double*)calloc(N - 2, sizeof(double));
	int n;
	for (n = 1; n < (N - 1); n++){
		if (A_Vect[(N - 1)*N + n] == 0)
			temp[n - 1] = FI[(T - 1)*N + n] + min_val;
		else
			temp[n - 1] = FI[(T - 1)*N + n] + log(A_Vect[(N - 1)*N + n]);
		//printf("%f ", FI[(T - 1)*N + n]);
	}


	// Allocation of memory
	int *max_pos = (int*)malloc(sizeof(int));
	maxx(temp, N - 2, Pvit, max_pos);


	XX[N*T - 1] = *max_pos + 2;

	// de-allocation of memory
	free(temp);
	free(max_pos);

	return;
}
