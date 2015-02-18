#include <stdio.h>
#include <stdbool.h>
#include "../include/model.h"
#include "../include/rand_num.h"
#include "../include/check_error.h"
#include "../include/cJSON.h"
#include "../include/configuration.h"

void model_initialization(model_t* model, uint8_t nGaussians, uint8_t nDimensions, uint8_t nStates){

	// some information
	model->algorithm = algorithm_type;
	model->numberGaussians = nGaussians;
	model->numberDimensions = nDimensions;
	model->numberStates = nStates;
	model->noise = noise_value;

	// normalization definition & assign
	normalisation_t norm = norm_type;
	model->normalisationType = norm;

	// allocation of the memory for the model matrixes
	model->transition = (float**)malloc(model->numberStates * sizeof(float*));
	for (int i = 0; i < model->numberStates; i++)
		model->transition[i] = (float*)calloc(model->numberStates, sizeof(float));

	model->mu = (float***)malloc(model->numberDimensions * sizeof(float**));
	for (int i = 0; i < model->numberDimensions; i++){
		model->mu[i] = (float**)malloc(model->numberStates * sizeof(float*));
		for (int j = 0; j < model->numberStates; j++)
			model->mu[i][j] = (float*)calloc(model->numberGaussians, sizeof(float));
	}

	model->sigma = (float***)malloc(model->numberDimensions * sizeof(float**));
	for (int i = 0; i < model->numberDimensions; i++){
		model->sigma[i] = (float**)malloc(model->numberStates * sizeof(float*));
		for (int j = 0; j < model->numberStates; j++)
			model->sigma[i][j] = (float*)calloc(model->numberGaussians, sizeof(float));
	}

	model->weights = (float**)malloc(model->numberStates * sizeof(float*));
	for (int i = 0; i < model->numberStates; i++)
		model->weights[i] = (float*)calloc(model->numberGaussians, sizeof(float));

	model->normalisation = (float**)malloc(2 * sizeof(float*));
	for (int i = 0; i < 2; i++)
		model->normalisation[i] = (float*)calloc(model->numberDimensions, sizeof(float));
}

void parse_object_mat2(cJSON * item, float** mat2, uint8_t * vect_ind_org, uint8_t lev)
{
	uint8_t *vect_ind = calloc(2, sizeof(uint8_t));
	memcpy(vect_ind, vect_ind_org, 2 * sizeof(uint8_t));
	lev += 1;
	while (item)
	{
		if (item->child)
			parse_object_mat2(item->child, mat2, vect_ind, lev);
		else 			
			mat2[vect_ind[0]][vect_ind[1]] = item->valuedouble;
		//printf("%d %d-> %f\n", vect_ind[0], vect_ind[1], item->valuedouble);
		if ((lev >= 0) & (lev <= 2))
			vect_ind[lev] += 1;
		item = item->next;
	}
	free(vect_ind);
}

void parse_object_mat3(cJSON * item, float*** mat3, uint8_t * vect_ind_org, int8_t lev)
{
	uint8_t *vect_ind = calloc(3, sizeof(uint8_t));
	memcpy(vect_ind, vect_ind_org, 3 * sizeof(uint8_t));
	lev += 1;
	while (item)
	{
		if (item->child)
			parse_object_mat3(item->child, mat3, vect_ind, lev);
		else{
			//printf("%d %d %d-> %f\n", vect_ind[0], vect_ind[1], vect_ind[2], item->valuedouble);
			mat3[vect_ind[1]][vect_ind[2]][vect_ind[0]] = item->valuedouble;
		}
		if ((lev >= 0) & (lev <= 3))
			vect_ind[lev] += 1;
		item = item->next;
	}
	free(vect_ind);
}


void test_histogram(char * string_name, double ** test_vect, int sample_length, int NClasses, int *Align){
	
	// open the json file
	FILE *f; long len; char *data;
	f = fopen(string_name, "rb"); fseek(f, 0, SEEK_END); len = ftell(f); fseek(f, 0, SEEK_SET);
	data = (char*)malloc(len + 1); fread(data, 1, len, f); fclose(f);

	// initialize all format and root
	cJSON * root = cJSON_Parse(data);
	cJSON * format_transition = cJSON_GetObjectItem(root, "A");
	cJSON * format_mi = cJSON_GetObjectItem(root, "MI");
	cJSON * format_sigma = cJSON_GetObjectItem(root, "SIGMA");
	cJSON * format_weights = cJSON_GetObjectItem(root, "PComp");

	// creation of the model
	model_t * model = malloc(sizeof(model_t));
	model_initialization(model, cJSON_GetArraySize(format_mi), cJSON_GetArraySize(format_mi->child), cJSON_GetArraySize(format_mi->child->child));

	// MI
	if (format_mi->next)
		format_mi->next = NULL;
	uint8_t *vect_ind_mat3 = (uint8_t *) calloc(3, sizeof(uint8_t));
	parse_object_mat3(format_mi, model->mu, vect_ind_mat3, -2);

	// SIGMA
	if (format_sigma->next)
		format_sigma->next = NULL;
	for (int i = 0; i < 3; i++)
		vect_ind_mat3[i] = 0;
	parse_object_mat3(format_sigma, model->sigma, vect_ind_mat3, -2);
	free(vect_ind_mat3);

	// A
	if (format_transition->next)
		format_transition->next = NULL;
	uint8_t *vect_ind_mat2 = calloc(2, sizeof(uint8_t));
	parse_object_mat2(format_transition, model->transition, vect_ind_mat2, -2);

	// PComp
	if (format_weights->next)
		format_weights->next = NULL;
	for (int i = 0; i < 2; i++)
		vect_ind_mat2[i] = 0;
	parse_object_mat2(format_weights, model->weights, vect_ind_mat2, -2);
	free(vect_ind_mat2);

	// free the root
	cJSON_Delete(root);

	// check the errors
	check_error(&model);

	// pre-processing operations
	for (int i = 0; i < model->numberDimensions; i++)
		for (int j = 0; j < sample_length; j++)
			test_vect[i][j] = test_vect[i][j] + rand_num(2 * model->noise) - model->noise; 

	// normalization
	for (int pos = 0; pos < sample_length; pos++)
		normalize(&model, test_vect, pos);

	// initialization of the test parameters
	int winner = -1;
	float *Pvit_t = calloc(NClasses, sizeof(float));

	// test the sample using the trained Matrix
	winner = test_matrix(model, test_vect, sample_length, NClasses, Align, Pvit_t, NClasses);

	free(Pvit_t);


}