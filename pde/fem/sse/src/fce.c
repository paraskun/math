#include <fem/const.h>
#include <fem/fce.h>
#include <mtx/mtx.h>

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
  double mult = f->cnd.pps.neu.tta;

  switch (f->cnd.type) {
    case DIR:
      return -1;
    case ROB:
      for (int i = 0, e = 0; i < 4; ++i) {
        int mui = MU[i];
        int nui = NU[i];
        int tti = TT[i];

        for (int j = 0; j < 4; ++j, ++e) {
          int muj = MU[j];
          int nuj = NU[j];
          int ttj = TT[j];

          f->dep.rob.m->v[e] =
              f->cnd.pps.rob.bet * mx[mui][muj] * my[nui][nuj] * mz[tti][ttj];
        }
      }

      mult = f->cnd.pps.rob.bet * f->cnd.pps.rob.cff;
    case NEU:
      double h;

      switch (fce_nrm(v[f->vtx[0]], v[f->vtx[1]], v[f->vtx[2]])) {
        case X:
          h = hy * hz / 4;
          break;
        case Y:
          h = hx * hz / 4;
          break;
        case Z:
          h = hx * hy / 4;
          break;
      }

      f->b->vp[0] = mult * h;
      f->b->vp[1] = mult * h;
      f->b->vp[2] = mult * h;
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
