#include <pmtx.h>

#include <stdio.h>
#include <stdlib.h>

#define WIDTH 10

struct pmtx* pmtx_new(int n, int s, int c) {
  struct pmtx* mp = malloc(sizeof(struct pmtx));

  mp->v = malloc(sizeof(real) * n * c);
  mp->n = n;
  mp->s = s;
  mp->c = c;

  return mp;
}

int pmtx_fget(FILE* f, struct pmtx* mp) {
  fseek(f, mp->s * mp->n * WIDTH, SEEK_SET);

  for (int i = 0; i < mp->n * mp->c; ++i)
    if (fscanf(f, FMT, &mp->v[i]) != 1)
      return -1;

  return 0;
}

int pmtx_vmlt(struct pmtx* mp, struct vec* xp, struct vec* bp) {
  int r = 0;

#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = mp->s; i < mp->s + mp->c; ++i) {
    bp->v[i] = 0;

    for (int j = 0; j < mp->n; ++j)
      bp->v[i] += mp->v[r + j] * xp->v[j];

    r += mp->n;
  }

  return 0;
}

int pmtx_free(struct pmtx* mp) {
  free(mp->v);
  free(mp);

  return 0;
}
