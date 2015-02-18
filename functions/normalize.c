#include "normalize.h"
#include "../HMM-code/model.h"

void normalize(model_t *model, float** Test_Vect, int pos){
	// test data normalization
	switch (model->normalisationType){
	case NONE:
		break;
	case UNIFORM:
		for (int i = 0; i < model->numberDimensions; i++){
			float min = model->normalisation[0][i];
			float max = model->normalisation[1][i];
			Test_Vect[i][pos] = (Test_Vect[i][pos] - min) / (max - min);
		}
		break;
	case Z:
		for (int i = 0; i < model->numberDimensions; i++){
			float mean = model->normalisation[0][i];
			float std = model->normalisation[1][i];
			Test_Vect[i][pos] = (Test_Vect[i][pos] - mean) / std;
		}
		break;
	}
}