#include <bits/time.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include <def.h>
#include <mtx/mtx_dns.h>
#include <sle/sle.h>
#include <vec/vec.h>
#include "mtx/mtx.h"

#define N 50000

double to_ms(struct timespec* s, struct timespec* e) {
  double s_ns = s->tv_sec * 1000000000.0 + s->tv_nsec;
  double e_ns = e->tv_sec * 1000000000.0 + e->tv_nsec;

  return (e_ns - s_ns) / 1000000.0;
}

struct mtx* mtx_sequ(size_t n) {
  struct mtx* m = mtx_new(n);

  for (size_t i = 0; i < n; ++i)
    for (size_t j = 0; j < n; ++j)
      m->v[i * n + j] = j >= i ? n - j : 0.0;

  return m;
}

int main(int argc, char* argv[argc]) {
  struct timespec s;
  struct timespec e;

  struct mtx* a = mtx_sequ(N);
  struct vec* x = vec_seq(N);
  struct vec* b = vec_new(N);

  real bfr;
  real aft;

  vec_norm(x, &bfr);
  mtx_vmlt(a, x, b);
  vec_fput("x-bfr", x);

  clock_gettime(CLOCK_MONOTONIC_RAW, &s);
  sle_gauss(a, x, b);
  clock_gettime(CLOCK_MONOTONIC_RAW, &e);

  vec_norm(x, &aft);
  vec_fput("x-aft", x);

  printf("Norm Difference: %.3e\n", fabs(aft - bfr));
  printf("Execution time: %.3e ms.\n", to_ms(&s, &e));

  mtx_free(a);
  vec_free(x);
  vec_free(b);

  return 0;
}
