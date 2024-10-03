#include <pmtx.h>

#include <stdio.h>
#include <stdlib.h>

#define WIDTH 10

struct pmtx* pmtx_new(int n, int r, int i) {
  struct pmtx* mp = malloc(sizeof(struct pmtx));

  mp->v = malloc(sizeof(real) * n * r);
  mp->n = n;
  mp->r = r;
  mp->i = i;

  return mp;
}

int pmtx_fget(FILE* f, struct pmtx* mp) {
  fseek(f, mp->i * WIDTH * mp->n, SEEK_SET);

  for (int i = 0; i < mp->n * mp->r; ++i)
    if (fscanf(f, "%lf", &mp->v[i]) != 1)
      return -1;

  return 0;
}

int pmtx_vmlt(struct pmtx* mp, struct vec* xp, struct vec* bp) {
  int r = 0;

#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = mp->i; i < mp->i + mp->r; ++i) {
    bp->v[i] = 0;

    for (int j = 0; j < mp->n; ++j)
      bp->v[i] += mp->v[r * mp->n + j] * xp->v[i];

    r += 1;
  }

  return 0;
}

int pmtx_free(struct pmtx* mp) {
  free(mp->v);
  free(mp);

  return 0;
}
