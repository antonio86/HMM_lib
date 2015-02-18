#include "math.h"
#include "viterbi_cycle.h"
#include "../configuration/configuration.h"

void viterbi_cycle(float *FI, int *XX, float *A_Vect, float *LogPdf, int N, int T){

	// principal cycle of viterbi
	float maxll, temp;
	int t, n1, n2, tpos;

	for (t = 1; t < T; t++)
	{
		for (n1 = 1; n1 < (N - 1); n1++)
		{
			tpos = 2;
			if (A_Vect[1 + n1*N] == 0)
				maxll = FI[1 + (t - 1)*N] + min_val;
			else
				maxll = FI[1 + (t - 1)*N] + log(A_Vect[1 + n1*N]);
			for (n2 = 2; n2 < (N - 1); n2++)
			{ 
				if (A_Vect[n2 + n1*N] == 0)
					temp = FI[n2 + (t - 1)*N] + min_val;
				else
					temp = FI[n2 + (t - 1)*N] + log(A_Vect[n2 + n1*N]);
				if (temp > maxll){
					maxll = temp;
					tpos = n2 + 1;
				}
			}

			XX[n1 + t*N] = tpos;
			FI[n1 + t*N] = maxll + LogPdf[t+(n1 - 1)*T];

		}
	}

	return;

}
