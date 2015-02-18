#include "../include/struct_matrix.h"
#include "../include/rand_num.h"

struct Matrix Learned[NClasses];

struct Matrix * learn_matrix(void){
	int nC, n, d, k, nj;
	float minVal = 0;

	for (nC = 0; nC < NClasses; nC++){

		// parameter initialization
		float cumProb = 0.0;
		float randX = 0.0;

		// Transition Matrix
		for (n = 0; n < N; n++){
			// first column equal to zero
			Learned[nC].transition[n][0] = minVal;
			// last row equal to zero
			Learned[nC].transition[N-1][n] = minVal;
		}
		// cannot go from state 0 to end
		Learned[nC].transition[N-1][N-1] = minVal;

		if (N > 3){
			// first line
			cumProb = 0.0;
			randX = 0.0;
			for (n = 1; n < N - 2; n++){
				randX = rand_num(1 - cumProb);
				cumProb += randX;
				Learned[nC].transition[0][n] = randX;
			}
			// for having 1 as total on the row	
			Learned[nC].transition[0][N - 2] = (1 - cumProb);

			// other lines
			for (nj = 1; nj < N - 1; nj++){
				cumProb = 0.0;
				for (n = 1; n < N - 1; n++){
					randX = rand_num(1 - cumProb);
					cumProb += randX;
					Learned[nC].transition[nj][n] = randX;
				}
				// for having 1 as total on the row	
				Learned[nC].transition[nj][N - 1] = (1 - cumProb);
			}
		}
		else { // if N == 3
			Learned[nC].transition[0][1] = 1;
			Learned[nC].transition[0][2] = minVal;
			Learned[nC].transition[1][1] = 1;
			Learned[nC].transition[1][2] = 1;
		}



		// Emission Matrix
		float MAX_MU = 2.0;
		float MAX_SIGMA = 3.0;

		// first and last colunm equal to zero
		for (d = 0; d < D; d++){
			int k;
			for (k = 0; k < K; k++){
				Learned[nC].mu[d][0][k] = minVal;
				Learned[nC].sigma[d][0][k] = minVal;
				Learned[nC].mu[d][N-1][k] = minVal;
				Learned[nC].sigma[d][N-1][k] = minVal;
			}
		}

		// random values
		for (d = 0; d < D; d++){
			for (n = 1; n < N-1; n++){
				for (k = 0; k < K; k++){
					Learned[nC].mu[d][n][k] = rand_num(MAX_MU) - MAX_MU / 2;
					// always positive
					Learned[nC].sigma[d][n][k] = rand_num(MAX_SIGMA);
				}
			}
		}

		// Pcomp initialzated here
		// first and last colunm equal to zero
		for (k = 0; k < K; k++){
			Learned[nC].weight[0][k] = minVal;
			Learned[nC].weight[N-1][k] = minVal;
		}
		for (n = 1; n < N-1; n++){
			cumProb = 0.0;
			for (k = 0; k < K-1; k++){
				randX = rand_num(1 - cumProb);
				cumProb += randX;
				Learned[nC].weight[n][k] = randX;
			}
			// for having 1 as total on the row	
			Learned[nC].weight[n][K - 1] = (1 - cumProb);
		}
		
	}
		
	return Learned;
}
