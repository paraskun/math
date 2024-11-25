#include <fem/vtx.h>

#include <stdlib.h>

struct vtx* vtx_new() {
  return malloc(sizeof(struct vtx));
}

nrm fce_nrm(struct vtx* a, struct vtx* b, struct vtx* c) {
  if (a->x == c->x && b->x == c->x)
    return X;

  if (a->y == c->y && b->y == c->y)
    return Y;

  return Z;
}

int vtx_get(FILE* obj, struct vtx* v) {
  return fscanf(obj, "%lf %lf %lf", &v->x, &v->y, &v->z) / 3;
}

int vtx_cls(struct vtx* v) {
  if (v)
    free(v);

  return 0;
}
