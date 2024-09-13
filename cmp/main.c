#include <stdio.h>
#include <time.h>
#include <omp.h>

#include "mtx/mtx.h"
#include "mtx/mtx_dense.h"

#define NANO 1000000000
#define N 1000

double to_ms(struct timespec* s, struct timespec* e) {
  double nano =
      (e->tv_sec * NANO + e->tv_nsec) - (s->tv_sec * NANO + s->tv_nsec);
  return nano / 1000000.0;
}

int main() {
  struct timespec s;
  struct timespec e;

  struct mtx* a = mtx_seq(N);
  struct mtx* b = mtx_seq(N);
  struct mtx* c = mtx_new(N);

  double norm;

  clock_gettime(CLOCK_MONOTONIC, &s);
  mtx_mmlt(a, b, c);
  clock_gettime(CLOCK_MONOTONIC, &e);

  mtx_norm(c, &norm);

//  printf("A = \n");
//  mtx_cput(a);
//  printf("B = \n");
//  mtx_cput(b);
//  printf("A * B =\n");
//  mtx_cput(c);
  printf("||C|| = %.7lf\n", norm);
  printf("Execution time: %.4e ms\n", to_ms(&s, &e));

  return 0;
}
