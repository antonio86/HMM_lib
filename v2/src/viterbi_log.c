#include <stdlib.h>
#include <string.h>
#include "../include/configuration.h"
#include "../include/model.h"
#include "../include/viterbi_log.h"
#include "../include/wdensity.h"
#include "../include/viterbi_init.h"
#include "../include/viterbi_cycle.h"
#include "../include/viterbi_end.h"

void viterbi_log(model_t* mat_class, double **Test_Vect, int T, int *X, double *Pvit){
	
	int N = mat_class->numberStates;
	int N_exp = mat_class->numberStates_exp;
	int D = mat_class->numberDimensions;
	int K = mat_class->numberGaussians;

	double *LogPdf = calloc(T*(N_exp - 2), sizeof(double));
	int n, d, t, k, cnt;
	cnt = 0;
	for (n = 1; n < N - 1; n++){

		double * Mu_Vect_Array = malloc(D*K*sizeof(double));
		double * Sigma_Vect_Array = malloc(D*K*sizeof(double));
		double * Test_Vect_Array = malloc(D*T*sizeof(double));
		double * PComp_Vect_Array = malloc(K*sizeof(double));
		
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

		double *Log = calloc(T, sizeof(double));

		// compute the distribution
		wdensity(Mu_Vect_Array, Sigma_Vect_Array, PComp_Vect_Array, Test_Vect_Array, Log, D, N, K, T);

		// de-alloc the memory
		free(Mu_Vect_Array);
		free(Sigma_Vect_Array);
		free(Test_Vect_Array);
		free(PComp_Vect_Array);

		for (int j = 0; j < mat_class->list_D[n-1]; j++)
			for (t = 0; t < T; t++){
				LogPdf[cnt] = Log[t];
				cnt += 1;
			}

		// de-alloc the memory
		free(Log);
	}	

	int n1, n2;
	double * A_Vect_Array = malloc(N_exp*N_exp*sizeof(double));
	for (n1 = 0; n1 < N_exp; n1++)
		for (n2 = 0; n2 < N_exp; n2++)
			A_Vect_Array[n1 + n2*N_exp] = mat_class->transition[n1][n2];
		
	double * FI = calloc(N_exp*T, sizeof(double));
	int * XX = calloc(N_exp*T, sizeof(int));

	viterbi_init(FI, A_Vect_Array, LogPdf, N_exp, T);

	viterbi_cycle(FI, XX, A_Vect_Array, LogPdf, N_exp, T);

	/*
	for (int j = 0; j < T; j++){
		for (int i = 0; i < N_exp; i++)
			printf("%f ", FI[i + j*N_exp]);
		printf("\n");
	}
	*/
	
	viterbi_end(FI, XX, A_Vect_Array, LogPdf, Pvit, N_exp, T); 

	free(LogPdf);
	free(A_Vect_Array);
	free(FI);

	// backtrace of states and probabilities
	X[T - 1] = XX[N_exp*T - 1];
	//Prob[T - 1] = log(A_Vect_Array[X[T - 1]+ N*(N-1)]);  
	for (t = T - 2; t > -1; t--){
		X[t] = XX[(t + 1)*N_exp + X[(t + 1)] - 1];
		//Prob[t] = log(A_Vect_Array[X[t] -1 + N*(X[t+1] - 1)]);
	}

	free(XX);

	if (expanded){
		// transform the expanded in normal	
		double * cumsum_List_D = malloc((N - 2 + 1)*sizeof(double));
		cumsum_List_D[0] = 1;
		for (int i = 0; i < N - 2; i++){
 			cumsum_List_D[i + 1] = cumsum_List_D[i] + mat_class->list_D[i];
			for (t = 0; t < T; t++)
				if ((X[t] > cumsum_List_D[i]) && (X[t] < cumsum_List_D[i + 1]))
					X[t] = i + 2;
		}
		free(cumsum_List_D);
	}
}
