#include <stdio.h>
#include <stdlib.h>
#include "../include/model.h"
#include "../include/free_model.h"
#include "../include/configuration.h"

void free_model(model_t* model){

	for (int i = 0; i < model->numberStates_exp; i++)
		free(model->transition[i]);
	free(model->transition);

	for (int i = 0; i < model->numberDimensions; i++){
		for (int j = 0; j < model->numberStates; j++)
			free(model->mu[i][j]);
		free(model->mu[i]);
	}
	free(model->mu);

	for (int i = 0; i < model->numberDimensions; i++){
		for (int j = 0; j < model->numberStates; j++)
			free(model->sigma[i][j]);
		free(model->sigma[i]);
	}
	free(model->sigma);

	for (int i = 0; i < model->numberStates; i++)
		free(model->weights[i]);
	free(model->weights);

	for (int i = 0; i < 2; i++)
		free(model->normalisation[i]);
	free(model->normalisation);

	free(model->list_D);

	free(model);

}