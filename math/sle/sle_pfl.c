#include <def.h>

#ifdef MTX_PFL_H

#include <mtx_pfl.h>
#include <sle.h>

static void sle_gauss_l(struct mtx* a, struct vec* x, struct vec* b) {}
static void sle_gauss_d(struct mtx* a, struct vec* x, struct vec* b) {}
static void sle_gauss_u(struct mtx* a, struct vec* x, struct vec* b) {}

void sle_gauss(struct mtx* a, struct vec* x, struct vec* b) {
  mtx_ldu(a);

  sle_gauss_l(a, x, b);
  sle_gauss_d(a, b, x);
  sle_gauss_u(a, x, b);
}

#endif  // SLE_PFL
