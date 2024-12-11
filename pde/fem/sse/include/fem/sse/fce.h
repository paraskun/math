#ifndef FEM_FCE_H
#define FEM_FCE_H

#include <fem/sse/vtx.h>
#include <vec/mtx.h>
#include <vec/mtx_csj.h>

struct cnd {
  enum type { DIR, NEU, ROB } type;

  union {
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

  union {
    struct {
      struct vec* b;
    } neu;

    struct {
      struct vec* b;
      struct mtx* m;
    } rob;
  } loc;
};

int fce_ini(struct fce** h);
int fce_cls(struct fce** h);

int fce_new(struct fce* f);

int fce_sget(struct fce* f, const char* buf, double (**fun)(struct vtx*));

int fce_evo(struct fce* f, struct vtx** v);
int fce_mov(struct fce* f, struct mtx_csj* a, struct vec* b);

#endif  // FEM_FCE_H
