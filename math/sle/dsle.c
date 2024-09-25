#include <def.h>

#ifdef DMTX

#include <math.h>
#include <stdlib.h>

#include <mtx/dmtx.h>
#include <sle/sle.h>

static inline void swap(size_t* p, size_t a, size_t b) {
  if (a == b)
    return;

  real tmp = p[a];
  p[a] = p[b];
  p[b] = tmp;
}

void sle_gauss(struct mtx* mp, struct vec* xp, struct vec* bp) {
  size_t* p = malloc(sizeof(size_t) * mp->n);

  for (size_t i = 0; i < mp->n; ++i)
    p[i] = i;

  for (size_t i = 0; i < mp->n; ++i) {
    real m = fabs(ME(mp, p[i], i));
    size_t mi = i;

    for (size_t j = i + 1; j < mp->n; ++j)
      if (fabs(ME(mp, p[j], i)) > m) {
        m = fabs(ME(mp, p[j], i));
        mi = j;
      }

    swap(p, i, mi);

    for (size_t j = i + 1; j < mp->n; ++j) {
      real k = ME(mp, p[j], i) / ME(mp, p[i], i);

      for (size_t c = i + 1; c < mp->n; ++c)
        ME(mp, p[j], c) -= ME(mp, p[i], c) * k;

      bp->v[p[j]] -= bp->v[p[i]] * k;
    }
  }

  for (size_t o = 0, i; o < mp->n; ++o) {
    i = mp->n - 1 - o;

    xp->v[i] = bp->v[p[i]];

    for (size_t j = i + 1; j < mp->n; ++j)
      xp->v[i] -= xp->v[j] * ME(mp, p[i], j);

    xp->v[i] /= ME(mp, p[i], i);
  }

  free(p);
}

#endif  // DMTX
