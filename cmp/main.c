#include <omp.h>
#include <stdio.h>
#include <time.h>

#include "vec/vec.h"

#define NANO 1000000000
#define N 100000

double to_ms(struct timespec* s, struct timespec* e) {
  double nano =
      (e->tv_sec * NANO + e->tv_nsec) - (s->tv_sec * NANO + s->tv_nsec);
  return nano / 1000000.0;
}

int main() {
  struct timespec s;
  struct timespec e;

  struct vec* r = vec_new(N);
  struct vec* x = vec_seq(N);
  struct vec* y = vec_seq(N);

  double norm;
  double a = 3.25;

  clock_gettime(CLOCK_MONOTONIC, &s);
  vec_cmb(x, y, r, a);
  clock_gettime(CLOCK_MONOTONIC, &e);

  vec_norm(r, &norm);

  printf("||r|| = %.7lf\n", norm);
  printf("Execution time: %.4e ms\n", to_ms(&s, &e));

  vec_free(r);
  vec_free(x);
  vec_free(y);

  return 0;
}
