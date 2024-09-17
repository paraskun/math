#ifndef MTX_H
#define MTX_PFL_H

#include <mtx.h>

struct mtx {
  real* d;
  real* l;
  real* u;
  real* p;

  uint32_t n;
  uint32_t s;
};

#endif  // MTX_PFL_H
