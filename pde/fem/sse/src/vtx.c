#include <fem/vtx.h>

#include <stdlib.h>

struct vtx* vtx_new() {
  return malloc(sizeof(struct vtx));
}

int vtx_get(FILE* obj, struct vtx* v) {
  if (fgetc(obj) != 'v')
    return 0;

  fscanf(obj, "%lf %lf %lf", &v->x, &v->y, &v->z);
  fscanf(obj, "| %lf", &v->pps.q);

  return 1;
}

int vtx_cls(struct vtx* v) {
  if (v)
    free(v);

  return 0;
}
