#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <vec/vec.h>

int vec_new(struct vec** h, uint dim) {
  if (!h || dim == 0) {
    errno = EINVAL;
    return -1;
  }

  struct vec* v = malloc(sizeof(struct vec));

  if (!v) {
    errno = ENOMEM;
    return -1;
  }

  v->dim  = dim;
  v->data = malloc(sizeof(double) * dim);

  if (!v->data) {
    free(v);

    errno = ENOMEM;
    return -1;
  }

  memset(v->data, 0, sizeof(double) * dim);

  *h = v;

  return 0;
}

int vec_cls(struct vec** h) {
  if (!h || !(*h)) {
    errno = EINVAL;
    return -1;
  }

  free((*h)->data);
  free(*h);

  return 0;
}

int vec_cmb(struct vec* a, struct vec* b, struct vec* r, double k) {
  if (!a || !b || !r || a->dim != r->dim || b->dim != r->dim) {
    errno = EINVAL;
    return -1;
  }

  uint dim = a->dim;

  double* ad = a->data;
  double* bd = b->data;
  double* rd = r->data;

#ifdef OMP_THREADS_NUM
#  pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (uint i = 0; i < dim; ++i)
    rd[i] = ad[i] + k * bd[i];

  return 0;
}

int vec_dot(struct vec* a, struct vec* b, double* r) {
  if (!a || !b || !r) {
    errno = EINVAL;
    return -1;
  }

  uint dim = a->dim;

  double* ad = a->data;
  double* bd = b->data;

  double s = 0;

#ifdef OMP_THREADS_NUM
#  pragma omp parallel for reduction(+ : s) num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (uint i = 0; i < dim; ++i)
    s += ad[i] * bd[i];

  *r = s;

  return 0;
}

int vec_nrm(struct vec* v, double* r) {
  if (!v || !r) {
    errno = EINVAL;
    return -1;
  }

  vec_dot(v, v, r);
  *r = sqrt(*r);

  return 0;
}

int vec_cpy(struct vec* s, struct vec* d) {
  if (!s || !d || s->dim != d->dim) {
    errno = EINVAL;
    return -1;
  }

  memcpy(d->data, s->data, sizeof(double) * s->dim);

  return 0;
}

int vec_rst(struct vec* v) {
  if (!v) {
    errno = EINVAL;
    return -1;
  }

  memset(v->data, 0, sizeof(double) * v->dim);

  return 0;
}
