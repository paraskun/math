#ifndef BAS_MAS_H
#define BAS_MAS_H

#include <vec/vec.h>

int pdif(double (*f)(struct vec*), uint i, double h, struct vec* p, double* r);

#endif  // BAS_MAS_H
