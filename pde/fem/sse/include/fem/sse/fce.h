#ifndef FEM_FCE_H
#define FEM_FCE_H

#include <fem/sse/vtx.h>
#include <vec/mtx.h>
#include <vec/mtx_csj.h>

struct cnd {
  enum type { DIR, NEU, ROB } type;

  union pps {
    struct {
      double (*tmp)(struct vtx* v);
    } dir;

    struct {
      double (*tta)(struct vtx* v);
    } neu;

    struct {
      double (*tmp)(struct vtx* v);
      double bet;
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
      struct vec* b;
      struct mtx* m;
    } rob;
  } dep;
};

struct fce* fce_new();

int fce_get(const char* buf, struct fce* f, double (**fun)(struct vtx*));
int fce_evo(struct fce* f, struct vtx** v);
int fce_mov(struct fce* f, struct mtx_csj* a, struct vec* b);
int fce_cls(struct fce* f);

#endif  // FEM_FCE_H
