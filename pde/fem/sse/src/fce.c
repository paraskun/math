#include <fem/fce.h>
#include <fem/const.h>
#include <mtx_all.h>

#include <stdlib.h>

struct fce* fce_new() {
  struct fce* f = malloc(sizeof(struct fce));

  if (!f)
    return f;

  f->m = NULL;
  f->b = NULL;

  return f;
}

int fce_get(FILE* obj, struct fce* f) {
  for (int j = 0, v; j < 4; ++j) {
    fscanf(obj, "%d", &v);
    f->vtx[j] = v - 1;
  }

  fscanf(obj, "| %d", &f->cnd.type);

  switch (f->cnd.type) {
    case DIR:
      break;
    case NEU:
      fscanf(obj, "| %lf", &f->cnd.pps.neu.tta);
      f->b = vec_new(4);
      break;
    case ROB:
      fscanf(obj, "| %lf %lf", &f->cnd.pps.rob.bet, &f->cnd.pps.rob.cff);
      f->b = vec_new(4);
      f->m = mtx_new(4);
      break;
  }

  return 1;
}

int fce_evo(struct fce* f,
    struct vtx** v,
    double hx, 
    double hy, 
    double hz, 
    double mx[2][2], 
    double my[2][2], 
    double mz[2][2]) {
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

          f->m->vp[e] = f->cnd.pps.rob.bet * 
            mx[mui][muj] *
            my[nui][nuj] *
            mz[tti][ttj];
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

  if (f->m)
    free(f->m);

  if (f->b)
    free(f->b);

  free(f);

  return 0;
}

int fll_add(struct fll* l, struct fce* f) {
  if (l->beg == NULL)
    l->beg = f;
  else
    l->end->next = f;

  l->end = f;

  return 0;
}

int fll_cls(struct fll* l) {
  struct fce* f = l->beg;

  while (f) {
    struct fce* n = f->next;
    fce_cls(f);
    f = n;
  }

  return 0;
}
