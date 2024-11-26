#ifndef FEM_VTX_H
#define FEM_VTX_H

#include <stdio.h>

typedef enum nrm {
  X, Y, Z
} nrm;

struct vtx {
  double x;
  double y;
  double z;

  double q;
};

struct vtx* vtx_new();

nrm fce_nrm(struct vtx* a, struct vtx* b, struct vtx* c);

int vtx_get(FILE* obj, struct vtx* v);
int vtx_cls(struct vtx* v);

#endif  // FEM_VTX_H
