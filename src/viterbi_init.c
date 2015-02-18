#include "math.h"
#include "../include/viterbi_init.h"
#include "../include/configuration.h"

void viterbi_init(double *FI, double *A_Vect, double *LogPdf, int N, int T){
	// initiliazation of viterbi
	int n;
	for (n = 1; n < (N - 1); n++)
		if (A_Vect[n*N] == 0)
			FI[n] = LogPdf[(n - 1)*T] + min_val;
		else
			FI[n] = LogPdf[(n - 1)*T] + log(A_Vect[n*N]);
	
	return;
}
