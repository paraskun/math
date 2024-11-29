#ifndef FEM_VTX_H
#define FEM_VTX_H

// Узел
struct vtx {
  // Координата по X
  double x;

  // Координата по Y
  double y;

  // Координата по Z
  double z;
};

// Подпрограмма выделения памяти под узел
struct vtx* vtx_new();

// Подпрограмма чтения узла из строки
struct vtx* vtx_get(const char* buf);

int vtx_cls(struct vtx* v);

#endif  // FEM_VTX_H
