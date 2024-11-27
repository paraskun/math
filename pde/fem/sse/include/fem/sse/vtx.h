#ifndef FEM_VTX_H
#define FEM_VTX_H

#include <stdio.h>

struct vtx {
  double x;
  double y;
  double z;

  struct {
    double q;
  } pps;
};

struct vtx* vtx_new();

int vtx_get(FILE* obj, struct vtx* v);
int vtx_cls(struct vtx* v);

#endif  // FEM_VTX_H
