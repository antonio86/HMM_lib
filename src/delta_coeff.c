#include <stdlib.h>
#include "../include/delta_coeff.h"

/*
// TO BE TESTED
double** delta_coeff_mat(double **matrix_coeff, int dimensions, int length, int window){
	// initialize an array to be used for every dimension
	double *array_coeff = (double*)calloc(length, sizeof(double));
	// allocate the memory
	double **mat_coeff = (double**)malloc(dimensions*sizeof(double*));
	for (int i = 0; i < dimensions; i++)
		mat_coeff[i] = (double*)malloc(length*sizeof(double));

	for (int w = 0; w < dimensions; w++){
		for (int i = 0; i < length; i++)
			array_coeff[i] = matrix_coeff[w][i];
		mat_coeff[w] = delta_coeff_arr(array_coeff, length, window);
	}
	return mat_coeff;
}
*/
// TESTED
double* delta_coeff_arr(double *array_coeff, int length, int window){
	double *array_coeff_temp = (double*)malloc((length + 2 * window)*sizeof(double));
	for (int l = length + 2 * window - 1; l >= 0; l--){
		if (l > length + window - 1)
			array_coeff_temp[l] = array_coeff[length - 1];
		if ((l <= length + window - 1) & (l > window -1 ))
			array_coeff_temp[l] = array_coeff[l - window];
		if (l <= window - 1)
			array_coeff_temp[l] = array_coeff[0];
	}
	double *delta_coeff = (double*)calloc(length,sizeof(double));
	for (int i = 0; i < length; i++)
		for (int j = -window; j < window+1; j++)
			delta_coeff[i] += j*array_coeff_temp[i+window-j];
	free(array_coeff_temp);
	return delta_coeff;
}

/*
// TO DO
double** delta_2coeff_mat(double **matrix_coeff, int dimensions, int length, int window){
	// initialize an array to be used for every dimension
	double **mat_coeff = delta_coeff_mat(matrix_coeff,dimensions,length,window);
	// call the delta_2coeff_arr function
}
*/

// TESTED
double* delta_2coeff_arr(double *array_coeff, int length, int window){
	double *delta_coeff = delta_coeff_arr(array_coeff, length, window);
	double *delta_coeff_temp = (double*)calloc(length + 2, sizeof(double));
	for (int l = length + 1; l >= 0; l--){
		if (l > length)
			delta_coeff_temp[l] = delta_coeff[length - 1];
		if ((l <= length) & (l > 0))
			delta_coeff_temp[l] = delta_coeff[l - 1];
		if (l <= 0)
			delta_coeff_temp[l] = delta_coeff[0];
	}
	double *delta_2coeff = (double*)calloc(length, sizeof(double));
	for (int i = 1; i < length - 1; i++)
		delta_2coeff[i - 1] = delta_coeff_temp[i + 1] - delta_coeff_temp[i - 1];
	free(delta_coeff_temp);

	return delta_2coeff;
}

