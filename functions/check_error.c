#include <stdio.h>
#include <stdlib.h>
#include "check_error.h"

typedef enum { false, true } bool;

// checktest
void check_error(model_t* model){
	
	bool pass = true;

	// check the number of dimensions
	if (model->numberDimensions < 1){
		fprintf(stderr, "Minimum 1 dimension\n");
		pass = false;
	}

	// check the number of states
	if (model->numberStates < 3){
		fprintf(stderr, "Minimum 3 states: 1 + start + end\n");
		pass = false;
	}

	// check the number of Gaussians
	if (model->numberDimensions < 1){
		fprintf(stderr, "Minimum 1 Gaussian\n");
		pass = false;
	}

	// check if the number of the states correspond to the algorithm
	if ((model->algorithm == 0 && model->numberStates != 3) || (model->algorithm == 1 && model->numberStates < 4)){
		fprintf(stderr, "Problem with the algorithm and the number of states\n");
		pass = false;
	}

	if (pass == false){
		getchar();
		exit(1);
	}
	
	return;
}