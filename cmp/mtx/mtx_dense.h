#ifndef MTX_DENSE_H
#define MTX_DENSE_H

#include <stdint.h>

#include "mtx.h"

struct mtx {
  double* v;
  uint32_t n;
};

#endif  // MTX_DENSE_H
