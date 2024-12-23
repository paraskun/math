#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <vec/mtx.h>

int imtx_new(struct imtx** h, struct ipps pps) {
  if (!h || pps.m == 0 || pps.n == 0) {
    errno = EINVAL;
    return -1;
  }

  struct imtx* m = malloc(sizeof(struct imtx));

  if (!m) {
    errno = ENOMEM;
    return -1;
  }

  m->pps = pps;
  m->data = malloc(sizeof(double*) * pps.m);

  if (!m->data) {
    free(m);

    errno = ENOMEM;
    return -1;
  }

  for (uint i = 0; i < pps.m; ++i) {
    m->data[i] = malloc(sizeof(double) * pps.n);

    if (!m->data[i]) {
      for (uint j = 0; j < i; ++j)
        free(m->data[j]);

      free(m->data);
      free(m);

      errno = ENOMEM;
      return -1;
    }
  }

  *h = m;

  return 0;
}

int imtx_cls(struct imtx** h) {
  if (!h || !(*h)) {
    errno = EINVAL;
    return -1;
  }

  struct imtx* m = *h;

  for (uint i = 0; i < m->pps.m; ++i)
    free(m->data[i]);

  free(m->data);
  free(m);

  return 0;
}

int imtx_vmlt(struct imtx* m, struct vec* v, struct vec* r) {
  if (!m || !v || !r || m->pps.n != v->dim || m->pps.m != r->dim) {
    errno = EINVAL;
    return -1;
  }

  double** md = m->data;
  double* vd = v->data;
  double* rd = r->data;

#pragma omp parallel for
  for (uint i = 0; i < m->pps.m; ++i) {
    double s = 0;

#pragma omp parallel for reduction(+ : s)
    for (uint j = 0; j < m->pps.n; ++j)
      s += md[i][j] * vd[j];

    rd[i] = s;
  }

  return 0;
}

int imtx_mmlt(struct imtx* a, struct imtx* b, struct imtx* r) {
  if (
    !a || !b || !r || a->pps.n != b->pps.m || a->pps.m != r->pps.m ||
    b->pps.n != r->pps.n) {
    errno = EINVAL;
    return -1;
  }

  double** ad = a->data;
  double** bd = b->data;
  double** rd = r->data;

#pragma omp parallel for
  for (uint i = 0; i < r->pps.m; ++i) {
#pragma omp parallel for
    for (uint j = 0; j < r->pps.n; ++j) {
      double s = 0;

#pragma omp parallel for reduction(+ : s)
      for (uint e = 0; e < a->pps.n; ++e)
        s += ad[i][e] * bd[e][i];

      rd[i][j] = s;
    }
  }

  return 0;
}
