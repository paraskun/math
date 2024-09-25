#include <def.h>

#ifdef DMTX

#include <mtx/dmtx.h>

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>

static int rnd() {
  uint8_t rnd = 255;

  while (rnd > 254)
    if (getrandom(&rnd, 1, 0) == -1)
      exit(-1);

  return rnd % 5 - 4;
}

struct mtx* mtx_new(size_t n) {
  struct mtx* mp = malloc(sizeof(struct mtx));

  mp->v = malloc(sizeof(real) * n * n);
  mp->n = n;

  return mp;
}

void mtx_rnd(struct mtx* mp, size_t u) {
#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP_THREADS_NUM
  for (size_t i = 0; i < mp->n; ++i)
    for (size_t j = 0; j < mp->n; ++j)
      ME(mp, i, j) = rand() % (u + 1);
}

void mtx_seq(struct mtx* mp) {
#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP_THREADS_NUM
  for (size_t i = 0; i < mp->n; ++i)
    for (size_t j = 0; j < mp->n; ++j)
      ME(mp, i, j) = j;
}

void mtx_ddm(struct mtx* mp, size_t k) {
  real sum = 0;

  for (size_t i = 0; i < mp->n; ++i)
    for (size_t j = 0; j < mp->n; ++j)
      if (i != j) {
        ME(mp, i, j) = rnd();
        sum += ME(mp, i, j);
      }

  ME(mp, 0, 0) = -sum + 1 / pow(10, k);

  for (size_t i = 1; i < mp->n; ++i)
    ME(mp, i, i) = -sum;
}

void mtx_hlb(struct mtx* mp) {
  for (size_t i = 1; i <= mp->n; ++i)
    for (size_t j = 1; j <= mp->n; ++j)
      ME(mp, i - 1, j - 1) = 1.0 / (i + j - 1.0);
}

void mtx_fget(FILE* f, struct mtx* mp) {
  for (size_t i = 0; i < mp->n; ++i)
    for (size_t j = 0; j < mp->n; ++j)
      fscanf(f, "%lf", &ME(mp, i, j));
}

void mtx_fput(FILE* f, struct mtx* mp) {
  for (size_t i = 0; i < mp->n; ++i) {
    for (size_t j = 0; j < mp->n; ++j)
      fprintf(f, "%.7e ", ME(mp, i, j));

    fputc('\n', f);
  }
}

void mtx_mmlt(struct mtx* ap, struct mtx* bp, struct mtx* cp) {
#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP_THREADS_NUM
  for (size_t i = 0; i < ap->n; ++i)
    for (size_t j = 0; j < ap->n; ++j) {
      ME(cp, i, j) = 0.0;

      for (size_t e = 0; e < ap->n; ++e)
        ME(cp, i, j) += ME(ap, i, e) * ME(bp, e, j);
    }
}

void mtx_vmlt(struct mtx* ap, struct vec* bp, struct vec* cp) {
#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP_THREADS_NUM
  for (size_t i = 0; i < ap->n; ++i) {
    cp->v[i] = 0.0;

    for (size_t j = 0; j < ap->n; ++j)
      cp->v[i] += ME(ap, i, j) * bp->v[j];
  }
}

void mtx_norm(struct mtx* mp, real* rp) {
  for (size_t i = 0; i < mp->n; ++i)
    for (size_t j = 0; j < mp->n; ++j)
      *rp += ME(mp, i, j) * ME(mp, i, j);

  *rp = sqrt(*rp);
}

void mtx_free(struct mtx* mp) {
  free(mp->v);
  free(mp);
}

#endif  // MTX_DNS
