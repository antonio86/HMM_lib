#ifndef VITERBIEND_H_INCLUDED
#define VITERBIEND_H_INCLUDED

void maxx(float *Ar, int dim, float *val, int *pos);

void viterbi_end(float *FI, int *XX, float *A_Vect, float *LogPdf, float *Pvit, int N, int T);

#endif
