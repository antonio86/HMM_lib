#ifndef VITERBIEND_H_INCLUDED
#define VITERBIEND_H_INCLUDED

void maxx(double *Ar, int dim, double *val, int *pos);

void viterbi_end(double *FI, int *XX, double *A_Vect, double *LogPdf, double *Pvit, int N, int T);

#endif
