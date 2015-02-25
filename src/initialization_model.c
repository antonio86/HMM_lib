#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/normalize.h"
#include "../include/model.h"
#include "../include/check_error.h"
#include "../include/cJSON.h"
#include "../include/configuration.h"
#include "../include/initialization_model.h"

void model_initialization(model_t* model, uint8_t nGaussians, uint8_t nDimensions, uint8_t nStates, int nStates_exp){

	// some information
	model->algorithm = algorithm_type;
	model->numberGaussians = nGaussians;
	model->numberDimensions = nDimensions;
	model->numberStates = nStates;
	model->numberStates_exp = nStates_exp;
	model->noise = noise_value;

	// normalization definition & assign
	normalisation_t norm = norm_type;
	model->normalisationType = norm;

	// allocation of the memory for the model matrixes
	model->transition = (double**)malloc(model->numberStates_exp * sizeof(double*));
	for (int i = 0; i < model->numberStates_exp; i++)
		model->transition[i] = (double*)calloc(model->numberStates_exp, sizeof(double));

	model->mu = (double***)malloc(model->numberDimensions * sizeof(double**));
	for (int i = 0; i < model->numberDimensions; i++){
		model->mu[i] = (double**)malloc(model->numberStates * sizeof(double*));
		for (int j = 0; j < model->numberStates; j++)
			model->mu[i][j] = (double*)calloc(model->numberGaussians, sizeof(double));
	}

	model->sigma = (double***)malloc(model->numberDimensions * sizeof(double**));
	for (int i = 0; i < model->numberDimensions; i++){
		model->sigma[i] = (double**)malloc(model->numberStates * sizeof(double*));
		for (int j = 0; j < model->numberStates; j++)
			model->sigma[i][j] = (double*)calloc(model->numberGaussians, sizeof(double));
	}

	model->weights = (double**)malloc(model->numberStates * sizeof(double*));
	for (int i = 0; i < model->numberStates; i++)
		model->weights[i] = (double*)calloc(model->numberGaussians, sizeof(double));

	model->normalisation = (double**)malloc(2 * sizeof(double*));
	for (int i = 0; i < 2; i++)
		model->normalisation[i] = (double*)calloc(model->numberDimensions, sizeof(double));

	model->list_D = (double*)malloc((model->numberStates - 2)*sizeof(double));
	if (!expanded)
		for (int i = 0; i < model->numberStates - 2; i++)
			model->list_D[i] = 1;

}

void parse_object_mat1(cJSON * item, double* mat1, uint8_t * vect_ind_org, uint8_t lev)
{
	uint8_t *vect_ind = (uint8_t *)calloc(2, sizeof(uint8_t));
	memcpy(vect_ind, vect_ind_org, 2 * sizeof(uint8_t));
	lev += 1;
	while (item)
	{
		if (item->child)
			parse_object_mat1(item->child, mat1, vect_ind, lev);
		else
			mat1[vect_ind[0]] = item->valuedouble;
		//printf("%d-> %f\n", vect_ind[0], item->valuedouble);
		if ((lev >= 0) & (lev <= 2))
			vect_ind[lev] += 1;
		item = item->next;
	}
	free(vect_ind);
}

void parse_object_mat2(cJSON * item, double** mat2, uint8_t * vect_ind_org, uint8_t lev)
{
	uint8_t *vect_ind = (uint8_t *)calloc(2, sizeof(uint8_t));
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

void parse_object_mat3(cJSON * item, double*** mat3, uint8_t * vect_ind_org, int8_t lev)
{
	uint8_t *vect_ind = (uint8_t *)calloc(3, sizeof(uint8_t));
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


model_t * initialization_model(char * string_name){

	// open the json file
	FILE *f; long len; char *data;
	errno_t err;
	err = fopen_s(&f, string_name, "rb"); fseek(f, 0, SEEK_END); len = ftell(f); fseek(f, 0, SEEK_SET);
	data = (char*)malloc(len + 1); fread(data, 1, len, f); fclose(f);

	// initialize all format and root
	cJSON * root = cJSON_Parse(data);
	cJSON * format_transition = cJSON_GetObjectItem(root, "A");
	cJSON * format_mi = cJSON_GetObjectItem(root, "MI");
	cJSON * format_sigma = cJSON_GetObjectItem(root, "SIGMA");
	cJSON * format_weights = cJSON_GetObjectItem(root, "PComp");
	cJSON * format_list_D = NULL;
	if (expanded)
		format_list_D = cJSON_GetObjectItem(root, "list_D");

	// creation of the model
	model_t * model = (model_t *)malloc(sizeof(model_t));
	model_initialization(model, cJSON_GetArraySize(format_mi), cJSON_GetArraySize(format_mi->child), cJSON_GetArraySize(format_mi->child->child), cJSON_GetArraySize(format_transition));

	// MI
	if (format_mi->next)
		format_mi->next = NULL;
	uint8_t *vect_ind_mat3 = (uint8_t *)calloc(3, sizeof(uint8_t));
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
	uint8_t *vect_ind_mat2 = (uint8_t *)calloc(2, sizeof(uint8_t));
	parse_object_mat2(format_transition, model->transition, vect_ind_mat2, -2);

	// PComp
	if (format_weights->next)
		format_weights->next = NULL;
	for (int i = 0; i < 2; i++)
		vect_ind_mat2[i] = 0;
	parse_object_mat2(format_weights, model->weights, vect_ind_mat2, -2);
	free(vect_ind_mat2);

	// list_D
	if (expanded){
		uint8_t *vect_ind_mat1 = (uint8_t *)calloc(1, sizeof(uint8_t));
		if (format_list_D->next)
			format_list_D->next = NULL;
		parse_object_mat1(format_list_D, model->list_D, vect_ind_mat1, -2);
		free(vect_ind_mat1);
	}

	// free the root
	cJSON_Delete(root);

	// check the errors
	check_error(model);

	return model;
}
