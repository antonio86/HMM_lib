#include "../include/normalize.h"
#include "../include/model.h"

void normalize(model_t *model, double** Test_Vect, int pos){
	// test data normalization
	switch (model->normalisationType){
	case NONE:
		break;
	case UNIFORM:
		for (int i = 0; i < model->numberDimensions; i++){
			double min = model->normalisation[0][i];
			double max = model->normalisation[1][i];
			Test_Vect[i][pos] = (Test_Vect[i][pos] - min) / (max - min);
		}
		break;
	case Z:
		for (int i = 0; i < model->numberDimensions; i++){
			double mean = model->normalisation[0][i];
			double std = model->normalisation[1][i];
			Test_Vect[i][pos] = (Test_Vect[i][pos] - mean) / std;
		}
		break;
	}
}