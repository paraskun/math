#ifndef CALC_H
#define CALC_H

#include <vec/vec.h>

typedef double (*mfun)(struct vec*);

int pdif(mfun f, uint i, double hop, struct vec* p, double* r);

#endif  // CALC_H
