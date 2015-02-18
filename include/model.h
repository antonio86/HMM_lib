#include <stdint.h>
#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#define T_MAX 1

typedef enum{
	GMM = 0,
	HMM = 1
} algorithm_t;


typedef enum{
	SENSOR = 0,
	ACTIVITY = 1,
	WEATHER = 2,
	AGENDA = 3
} dimensionType_t;

typedef struct{
	char * url;
	dimensionType_t type;
} dimension_t;

typedef enum{
	NONE = 0,
	UNIFORM = 1,
	Z = 2
} normalisation_t;

typedef struct{
	uint8_t number;
	char *name;
} state_t;

typedef struct{
	algorithm_t algorithm;
	float noise;
	uint16_t samplingRate;
	uint8_t numberGaussians;
	char *className;
	uint8_t numberDimensions;
	dimension_t *dimensions;
	uint8_t *removedFeatures;
	normalisation_t normalisationType;
	float **normalisation;
	uint8_t numberStates;
	state_t *states;
	float ***mu;
	float ***sigma;
	float **transition;
	float **weights;
} model_t;

#endif