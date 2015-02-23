#ifndef STRUCT_MAT_H_INCLUDED
#define STRUCT_MAT_H_INCLUDED

#define T_MAX 40
#define D 3
#define N 4
#define K 7
#define NClasses 1

struct Matrix
{
	double transition[N][N];	// Transition Matrix
	double mu[D][N][K]; // Emission Matrix (MU)
	double sigma[D][N][K];// Emission Matrix (SIGMA)
	double weight[N][K]; // Emission Matrix (PComp)

};

#endif
