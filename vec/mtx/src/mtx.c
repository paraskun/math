#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <vec/mtx.h>

int imtx_new(struct imtx** h, struct ipps pps) {
  if (!h || pps.dim == 0) {
    errno = EINVAL;
    return -1;
  }

  struct imtx* m = malloc(sizeof(struct imtx));

  if (!m) {
    errno = ENOMEM;
    return -1;
  }

  m->pps  = pps;
  m->data = malloc(sizeof(double*) * pps.dim);

  if (!m->data) {
    free(m);

    errno = ENOMEM;
    return -1;
  }

  for (uint i = 0; i < pps.dim; ++i) {
    m->data[i] = malloc(sizeof(double) * pps.dim);

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

  for (uint i = 0; i < m->pps.dim; ++i)
    free(m->data[i]);

  free(m->data);
  free(m);

  return 0;
}

int imtx_vmlt(struct imtx* m, struct vec* v, struct vec* r) {
  uint dim = m->pps.dim;

  double** md = m->data;
  double* vd  = v->data;
  double* rd  = r->data;

#ifdef OMP_THREADS_NUM
#  pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (uint i = 0; i < dim; ++i) {
    double s = 0;

#ifdef OMP_THREADS_NUM
#  pragma omp parallel for reduction(+ : s) num_threads(OMP_THREADS_NUM)
#endif  // OMP
    for (uint j = 0; j < dim; ++j)
      s += md[i][j] * vd[j];

    rd[i] = s;
  }

  return 0;
}

int imtx_mmlt(struct imtx* a, struct imtx* b, struct imtx* r) {
  uint dim = a->pps.dim;

  double** ad = a->data;
  double** bd = b->data;
  double** rd = r->data;

#ifdef OMP_THREADS_NUM
#  pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (uint i = 0; i < dim; ++i) {
#ifdef OMP_THREADS_NUM
#  pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
    for (uint j = 0; j < dim; ++j) {
      double s = 0;

#ifdef OMP_THREADS_NUM
#  pragma omp parallel for reduction(+ : s) num_threads(OMP_THREADS_NUM)
#endif  // OMP
      for (uint e = 0; e < dim; ++e)
        s += ad[i][e] * bd[e][i];

      rd[i][j] = s;
    }
  }

  return 0;
}
