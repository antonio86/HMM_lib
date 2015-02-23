#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "include/test_histogram.h"
#include "include/delta_coeff.h"
#include "include/configuration.h"

#define T 59

int main() {

	// example
	//double Test[T] = {7262, 7079, 6853, 6642, 6474, 6360, 6305, 6279, 6280, 6278, 6279, 6277, 6279, 6266, 6263, 6290, 6312, 6417, 6628, 6883, 7140, 7305, 7366, 7380, 7437, 7500, 7638, 7793, 7977, 8091, 8008, 7726, 7380, 7041, 6878, 6846, 6841, 6699, 6481, 6323, 6313, 6307, 6317, 6326, 6337, 6338, 6350, 6391, 6421, 6429, 6386, 6375, 6468, 6654, 6807, 7269, 7417, 7578, 7661 };
	double Test[T] = { 0.546498905908096, 0.446389496717724, 0.322757111597374, 0.207330415754923, 0.115426695842451, 0.0530634573304158, 0.0229759299781182, 0.00875273522975930, 0.00929978118161926, 0.00820568927789934, 0.00875273522975930, 0.00765864332603939, 0.00875273522975930, 0.00164113785557987, 0, 0.0147702407002188, 0.0268052516411379, 0.0842450765864333, 0.199671772428884, 0.339168490153173, 0.479759299781182, 0.570021881838075, 0.603391684901532, 0.611050328227571, 0.642231947483589, 0.676695842450766, 0.752188183807440, 0.836980306345733, 0.937636761487965, 1, 0.954595185995624, 0.800328227571116, 0.611050328227571, 0.425601750547046, 0.336433260393873, 0.318927789934355, 0.316192560175055, 0.238512035010941, 0.119256017505470, 0.0328227571115974, 0.0273522975929978, 0.0240700218818381, 0.0295404814004376, 0.0344638949671772, 0.0404814004376368, 0.0410284463894967, 0.0475929978118162, 0.0700218818380744, 0.0864332603938731, 0.0908096280087527, 0.0672866520787746, 0.0612691466083151, 0.112144420131291, 0.213894967177243, 0.297592997811816, 0.550328227571116, 0.631291028446390, 0.719365426695843, 0.764770240700219 };
	double * res = delta_coeff_arr(Test, T, 2);
	double * res2 = delta_2coeff_arr(Test, T, 2);

	// allocate the memory
	double **Test_Vect = (double**)calloc(3, sizeof(double*));
	for (int i = 0; i < 3; i++)
		Test_Vect[i] = (double*)calloc(T,sizeof(double));
	for (int j = 0; j < T; j++){
		Test_Vect[0][j] = Test[j];
		Test_Vect[1][j] = res[j];
		Test_Vect[2][j] = res2[j];
	}
	free(res);
	free(res2);

	/*
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < T; j++)
			printf("%f\n", Test_Vect[i][j]);
	*/

	// number of classes
	int NClasses = 1;
	int *Align = (int*)calloc(T, sizeof(int));

	char *string_name;
	if (expanded)
		string_name = "Matrix_Fer.json";
	else
		string_name = "Matrix.json";

	// test the histogram
	test_histogram(string_name, Test_Vect, T, NClasses, Align);

	// print the resuts
	for (int t = 0; t < T; t++)
		printf("%d ", Align[t]);
	printf("\n");

	// free the allocated memory
	free(Test_Vect);
	free(Align);

	// read the results
	int numITM;
	scanf("%d", &numITM);

	return 0;
}