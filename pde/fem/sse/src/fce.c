#include <fem/sse/const.h>
#include <fem/sse/fce.h>
#include <vec/mtx.h>

#include <stdlib.h>

struct fce* fce_new() {
  return malloc(sizeof(struct fce));
}

int fce_get(FILE* obj, struct fce* f) {
  if (fgetc(obj) != 'f')
    return 0;

  for (int j = 0, v; j < 4; ++j) {
    fscanf(obj, "%d", &v);
    f->vtx[j] = v - 1;
  }

  int type;
  fscanf(obj, "| %d", &type);
  f->cnd.type = type - 1;

  return 1;
}

int fce_nrm(struct fce* f, struct vtx** v) {
  struct vtx* a = v[f->vtx[0]];
  struct vtx* b = v[f->vtx[1]];
  struct vtx* c = v[f->vtx[2]];

  if (a->x == c->x && b->x == c->x)
    return 0;

  if (a->y == c->y && b->y == c->y)
    return 1;

  return 2;
}

int fce_evo(struct fce* f, struct vtx** v) {
  switch (f->cnd.type) {
    case DIR:
      return 0;
    case NEU:
    case ROB:
      return v[0]->x;
  }

  return 0;
}

int fce_mov(struct fce* f, struct mtx_csj* a, struct vec* b) {
  struct mtx* lm;
  struct vec* lb;

  switch (f->cnd.type) {
    case DIR:
      return 0;
    case ROB:
      lm = f->dep.rob.m;
      lb = f->dep.rob.b;

      for (int i = 0; i < 4; ++i) {
        int gi = f->vtx[i];

        b->vp[gi] += lb->vp[i];

        for (int j = 0; j < 4; ++j) {
          int gj = f->vtx[j];

          if (gi == gj) {
            a->dr[gi] += lm->v[i][j];
            continue;
          }

          if (gi > gj) {
            int lr0 = a->ia[gi];
            int lr1 = a->ia[gi + 1];

            for (int lr = lr0; lr < lr1; ++lr) {
              int lj = a->ja[lr];

              if (lj == gj) {
                a->lr[lr] += lm->v[i][j];
                break;
              }

              if (lj > gj)
                break;
            }
          } else {
            int ur0 = a->ia[gj];
            int ur1 = a->ia[gj + 1];

            for (int ur = ur0; ur < ur1; ++ur) {
              int ui = a->ja[ur];

              if (ui == gi) {
                a->ur[ur] += lm->v[i][j];
                break;
              }

              if (ui > gi)
                break;
            }
          }
        }
      }

      break;
    case NEU:
      lb = f->dep.neu.b;

      for (int i = 0; i < 4; ++i)
        b->vp[f->vtx[i]] += lb->vp[i];
  }

  return 0;
}

int fce_cls(struct fce* f) {
  if (!f)
    return 0;

  switch (f->cnd.type) {
    case ROB:
      free(f->dep.rob.b);
      free(f->dep.rob.m);
      break;
    case NEU:
      free(f->dep.neu.b);
      break;
    case DIR:
      break;
  }

  free(f);

  return 0;
}
