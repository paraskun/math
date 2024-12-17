#include <vec/vec.h>

#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

int vec_new(struct vec** h, uint cap) {
  if (!h || cap == 0) {
    errno = EINVAL;
    return -1;
  }

  struct vec* v = malloc(sizeof(struct vec));

  if (!v) {
    errno = ENOMEM;
    return -1;
  }

  v->cap = cap;
  v->data = malloc(sizeof(double) * cap);

  if (!v->data) {
    free(v);

    errno = ENOMEM;
    return -1;
  }

  memset(v->data, 0, sizeof(double) * cap);

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
  if (!a || !b || !r || a->cap != r->cap || b->cap != r->cap) {
    errno = EINVAL;
    return -1;
  }

  uint cap = a->cap;

  double* ad = a->data;
  double* bd = b->data;
  double* rd = r->data;

#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (uint i = 0; i < cap; ++i)
    rd[i] = ad[i] + k * bd[i];

  return 0;
}

int vec_dot(struct vec* a, struct vec* b, double* r) {
  if (!a || !b || !r) {
    errno = EINVAL;
    return -1;
  }

  uint cap = a->cap;

  double* ad = a->data;
  double* bd = b->data;

  double s = 0;

#ifdef OMP_THREADS_NUM
#pragma omp parallel for reduction(+ : s) num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (uint i = 0; i < cap; ++i)
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
  if (!s || !d || s->cap != d->cap) {
    errno = EINVAL;
    return -1;
  }

  memcpy(d->data, s->data, sizeof(double) * s->cap);

  return 0;
}

int vec_rst(struct vec* v) {
  if (!v) {
    errno = EINVAL;
    return -1;
  }

  memset(v->data, 0, sizeof(double) * v->cap);

  return 0;
}

