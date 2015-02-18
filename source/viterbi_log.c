#include <stdlib.h>
#include <string.h>
#include "../include/model.h"
#include "../include/viterbi_log.h"
#include "../include/wdensity.h"
#include "../include/viterbi_init.h"
#include "../include/viterbi_cycle.h"
#include "../include/viterbi_end.h"

void viterbi_log(model_t* mat_class, double **Test_Vect, int T, int *X, float *Pvit){
	
	int N = mat_class->numberStates;
	int D = mat_class->numberDimensions;
	int K = mat_class->numberGaussians;

	float *LogPdf = calloc(T*(N - 2), sizeof(float));
	int n, d, t, k;
	for (n = 1; n < N - 1; n++){

		float * Mu_Vect_Array = malloc(D*K*sizeof(float));
		float * Sigma_Vect_Array = malloc(D*K*sizeof(float));
		double * Test_Vect_Array = malloc(D*T*sizeof(double));
		float * PComp_Vect_Array = malloc(K*sizeof(float));
		
		for (d = 0; d < D; d++){
			for (k = 0; k < K; k++){
				Mu_Vect_Array[k + d*K] = mat_class->mu[d][n][k];
				Sigma_Vect_Array[k + d*K] = mat_class->sigma[d][n][k];
			}
			for (t = 0; t < T; t++)
				Test_Vect_Array[t + d*T] = Test_Vect[d][t]; 
		}
		for (k = 0; k < K; k++)
			PComp_Vect_Array[k] = mat_class->weights[n][k];

		float *Log = calloc(T, sizeof(float));
		// compute the distribution
		wdensity(Mu_Vect_Array, Sigma_Vect_Array, PComp_Vect_Array, Test_Vect_Array, Log, D, N, K, T);

		// de-alloc the memory
		free(Mu_Vect_Array);
		free(Sigma_Vect_Array);
		free(Test_Vect_Array);
		free(PComp_Vect_Array);

		for (t = 0; t < T; t++)
			LogPdf[t + (n - 1)*T] = Log[t];

		// de-alloc the memory
		free(Log);
	}

	int n1, n2;
	float * A_Vect_Array = malloc(N*N*sizeof(float));
	for (n1 = 0; n1 < N ; n1++)
		for (n2 = 0; n2 < N; n2++)
			A_Vect_Array[n1 + n2*N] = mat_class->transition[n1][n2];
		
	float * FI = calloc(N*T,sizeof(float));
	int * XX = calloc(N*T, sizeof(int));

	viterbi_init(FI, A_Vect_Array, LogPdf,N, T);

	viterbi_cycle(FI, XX, A_Vect_Array, LogPdf,N, T);

	/*
	for (int j = 0; j < T; j++){
		for (int i = 0; i < N; i++)
			printf("%f ", FI[i + j*7]);
		printf("\n");
	}
	*/

	viterbi_end(FI, XX, A_Vect_Array, LogPdf, Pvit,N,T);

	free(LogPdf);
	free(A_Vect_Array);
	free(FI);

	// backtrace of states and probabilities
	X[T - 1] = XX[N*T - 1];
	//Prob[T - 1] = log(A_Vect_Array[X[T - 1]+ N*(N-1)]);  
	for (t = T - 2; t > -1; t--){
		X[t] = XX[(t+1)*N+X[(t+1)]-1];
		//Prob[t] = log(A_Vect_Array[X[t] -1 + N*(X[t+1] - 1)]);
	}

	free(XX);
	
}
