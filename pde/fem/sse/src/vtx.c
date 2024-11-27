#include <fem/sse/vtx.h>

#include <stdio.h>
#include <stdlib.h>

struct vtx* vtx_new() {
  return malloc(sizeof(struct vtx));
}

int vtx_get(const char* buf, struct vtx* v) {
  if (buf[0] != 'v')
    return 0;

  sscanf(buf + 1, "%lf %lf %lf", &v->x, &v->y, &v->z);

  return 1;
}

int vtx_cls(struct vtx* v) {
  if (v)
    free(v);

  return 0;
}
