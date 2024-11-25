#ifndef FEM_FCE_H
#define FEM_FCE_H

#include <fem/vtx.h>

struct cnd {
  enum type { DIR, NEU, ROB } type;

  union pps {
    struct {
      double (*fun)(struct vtx* v);
    } dir;

    struct neu {
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

  struct mtx* m;
  struct vec* b;

  struct fce* next;
};

struct fll {
  struct fce* beg;
  struct fce* end;
};

struct fce* fce_new();

int fce_get(FILE* obj, struct fce* f);
int fce_evo(struct fce* f,
    struct vtx** v,
    double hx, 
    double hy, 
    double hz, 
    double mx[2][2], 
    double my[2][2], 
    double mz[2][2]);

int fce_cls(struct fce* f);

int fll_add(struct fll* l, struct fce* f);
int fll_cls(struct fll* l);

#endif  // FEM_FCE_H
