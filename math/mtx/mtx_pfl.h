#include <def.h>

#ifdef MTX_PFL_H

#include <mtx.h>
#include <stdint.h>

struct mtx {
  uint32_t* p;

  real* d;
  real* l;
  real* u;

  uint32_t n;
  uint32_t s;
};

struct mtx* mtx_new(uint32_t n, uint32_t s);

void mtx_ldu(struct mtx *m);

#endif  // MTX_PFL_H
