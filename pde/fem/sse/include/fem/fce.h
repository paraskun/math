#ifndef FEM_FCE_H
#define FEM_FCE_H

#include <fem/vtx.h>
#include <mtx/mtx.h>

struct cnd {
  enum type { DIR, NEU, ROB } type;

  union pps {
    struct {
      double (*fun)(struct vtx* v);
    } dir;

    struct {
      double tta;
    } neu;

    struct {
      double bet;
      double cff;
    } rob;
  } pps;
};

struct fce {
  int vtx[4];

  struct cnd cnd;

  union dep {
    struct {
      struct vec* b;
    } neu;

    struct {
      struct mtx* m;
      struct vec* b;
    } rob;
  } dep;
};

struct fce* fce_new();

int fce_get(FILE* obj, struct fce* f);
int fce_nrm(struct fce* f, struct vtx** v);
int fce_evo(struct fce* f, struct vtx** v);
int fce_cls(struct fce* f);

#endif  // FEM_FCE_H
