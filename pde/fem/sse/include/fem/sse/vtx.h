#ifndef FEM_VTX_H
#define FEM_VTX_H

struct vtx {
  double x;
  double y;
  double z;
};

int vtx_ini(struct vtx** h);
int vtx_cls(struct vtx** h);

int vtx_sget(struct vtx* v, const char* buf);

#endif  // FEM_VTX_H
