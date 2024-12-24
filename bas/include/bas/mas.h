#ifndef BAS_MAS_H
#define BAS_MAS_H

#include <vec/vec.h>

typedef double (*func)(struct vec*);

int pdif(func f, uint i, double h, struct vec* p, double* r);

#endif  // BAS_MAS_H
