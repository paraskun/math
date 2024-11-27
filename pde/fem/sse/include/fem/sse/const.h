#ifndef FEM_CONST_H
#define FEM_CONST_H

#include <limits.h>

const double G[2][2] = {
  {1.0, -1.0}, 
  {-1.0, 1.0}
};

const double M[2][2] = {
  {2.0 / 6.0, 1.0 / 6.0},
  {1.0 / 6.0, 2.0 / 6.0}
};

const int MU[8] = {0, 1, 0, 1, 0, 1, 0, 1};
const int NU[8] = {0, 0, 1, 1, 0, 0, 1, 1};
const int TT[8] = {0, 0, 0, 0, 1, 1, 1, 1};

const unsigned long C = ULONG_MAX;

#endif  // FEM_CONST_H
