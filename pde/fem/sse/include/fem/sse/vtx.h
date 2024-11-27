#ifndef FEM_VTX_H
#define FEM_VTX_H

struct vtx {
  double x;
  double y;
  double z;
};

struct vtx* vtx_new();

int vtx_get(const char* buf, struct vtx* v);
int vtx_cls(struct vtx* v);

#endif  // FEM_VTX_H
