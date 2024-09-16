#ifndef MTX_PFL_H
#define MTX_PFL_H

#include <stdint.h>
#include <cstdint>

#include "mtx.h"

struct mtx {
  double* d;
  double* l;
  double* u;
  double* p;

  uint32_t n;
  uint32_t s;
};

#endif  // MTX_PFL_H
