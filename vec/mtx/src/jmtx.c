#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <vec/mtx.h>

int jmtx_new(struct jmtx** h, struct jpps pps) {
  if (!h || pps.n == 0 || pps.m == 0) {
    errno = EINVAL;
    return -1;
  }

  struct jmtx* m = malloc(sizeof(struct jmtx));

  if (!m) {
    errno = ENOMEM;
    return -1;
  }

  m->pps = pps;
  m->data = malloc(sizeof(double (**)(struct vec*)) * pps.m);

  if (!m->data) {
    free(m);

    errno = ENOMEM;
    return -1;
  }

  for (uint i = 0; i < pps.m; ++i) {
    m->data[i] = malloc(sizeof(double (*)(struct vec*)) * pps.n);

    if (!m->data[i]) {
      for (uint j = 0; j < i; ++j)
        free(m->data[j]);

      free(m->data);
      free(m);

      errno = ENOMEM;
      return -1;
    }

    memset(m->data[i], 0, sizeof(double (*)(struct vec*)) * pps.n);
  }

  *h = m;

  return 0;
}

int jmtx_cls(struct jmtx** h) {
  if (!h || !(*h)) {
    errno = EINVAL;
    return -1;
  }

  struct jmtx* m = *h;

  for (uint i = 0; i < m->pps.m; ++i)
    free(m->data[i]);

  free(m->data);
  free(m);

  *h = nullptr;

  return 0;
}

int jmtx_evo(struct jmtx* m, struct vec* x, struct imtx* r) {
  if (
    !m || !x || !r || m->pps.m != r->pps.m || m->pps.n != r->pps.n ||
    m->pps.n != x->dim) {
    errno = EINVAL;
    return -1;
  }

#pragma omp parallel for
  for (uint i = 0; i < m->pps.m; ++i)
#pragma omp parallel for
    for (uint j = 0; j < m->pps.n; ++j)
      r->data[i][j] = m->data[i][j](x);

  return 0;
}
