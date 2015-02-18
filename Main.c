#include <stdio.h>
#include <stdbool.h>
#include "include/test_histogram.h"
#include "include/delta_coeff.h"

#define T 59

int main() {

	// example
	double Test[T] = {7262, 7079, 6853, 6642, 6474, 6360, 6305, 6279, 6280, 6278, 6279, 6277, 6279, 6266, 6263, 6290, 6312, 6417, 6628, 6883, 7140, 7305, 7366, 7380, 7437, 7500, 7638, 7793, 7977, 8091, 8008, 7726, 7380, 7041, 6878, 6846, 6841, 6699, 6481, 6323, 6313, 6307, 6317, 6326, 6337, 6338, 6350, 6391, 6421, 6429, 6386, 6375, 6468, 6654, 6807, 7269, 7417, 7578, 7661 };
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

	char *string_name = "Matrix.json";
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