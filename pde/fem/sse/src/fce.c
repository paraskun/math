#include <fem/sse/const.h>
#include <fem/sse/fce.h>
#include <vec/mtx.h>

#include <stdlib.h>

struct fce* fce_new() {
  struct fce* f = malloc(sizeof(struct fce));

  f->cnd.pps.dir.tmp = NULL;
  f->cnd.pps.neu.tta = NULL;
  f->cnd.pps.rob.tmp = NULL;

  f->dep.neu.b = NULL;
  f->dep.rob.m = NULL;

  return f;
}

int fce_get(const char* buf, struct fce* f, double (**fun)(struct vtx*)) {
  int pos;

  if (buf[0] != 'f')
    return 0;

  buf += 1;

  for (int j = 0, v; j < 4; ++j) {
    sscanf(buf, "%d%n", &v, &pos);
    buf += pos;

    f->vtx[j] = v - 1;
  }

  int type;
  sscanf(buf, " | %d%n", &type, &pos);
  buf += pos;

  f->cnd.type = type - 1;

  switch (f->cnd.type) {
    case DIR:
      int tmp;

      sscanf(buf, " | %d%n", &tmp, &pos);
      buf += pos;

      f->cnd.pps.dir.tmp = fun[tmp];

      break;
    case NEU:
      int tta;

      sscanf(buf, " | %d%n", &tta, &pos);
      buf += pos;

      f->cnd.pps.neu.tta = fun[tta];
      f->dep.neu.b = vec_new(4);

      break;
    case ROB:
      double bet;

      sscanf(buf, " | %d | %lf%n", &tmp, &bet, &pos);
      buf += pos;

      f->cnd.pps.rob.tmp = fun[tmp];
      f->cnd.pps.rob.bet = bet;
      f->dep.rob.b = vec_new(4);
      f->dep.rob.m = mtx_new(4);

      break;
  }

  return 1;
}

int fce_evo(struct fce* f, struct vtx** v) {
  if (f->cnd.type == DIR)
    return 0;

  double mxi[2][2];
  double mzt[2][2];

  double hxi = v[f->vtx[1]]->y - v[f->vtx[0]]->y;
  double hzt = v[f->vtx[2]]->y - v[f->vtx[0]]->y;

  for (int i = 0; i < 2; ++i)
    for (int j = 0; j < 2; ++j) {
      mxi[i][j] = hxi * M[i][j];
      mzt[i][j] = hzt * M[i][j];
    }

  if (f->cnd.type == ROB) {
    double* lb = f->dep.rob.b->vp;
    double** lm = f->dep.rob.m->v;

    double (*fun)(struct vtx*) = f->cnd.pps.rob.tmp;
    double bet = f->cnd.pps.rob.bet;
    double cff[4];

    for (int i = 0; i < 4; ++i)
      cff[i] = fun(v[f->vtx[i]]);

    for (int i = 0; i < 4; ++i) {
      lb[i] = 0;

      int mui = MU[i];
      int nui = NU[i];

      for (int j = 0; j < 4; ++j) {
        int muj = MU[j];
        int nuj = NU[j];

        lm[i][j] = bet * mxi[mui][muj] * mzt[nui][nuj];
        lb[i] += cff[j] * mxi[mui][muj] * mzt[nui][nuj];
      }

      lb[i] = lb[i] * bet;
    }

    return 0;
  }

  if (f->cnd.type == NEU) {
    double* lb = f->dep.neu.b->vp;

    double (*fun)(struct vtx*) = f->cnd.pps.neu.tta;
    double cff[4];

    for (int i = 0; i < 4; ++i)
      cff[i] = fun(v[f->vtx[i]]);

    for (int i = 0; i < 4; ++i) {
      lb[i] = 0;

      int mui = MU[i];
      int nui = NU[i];

      for (int j = 0; j < 4; ++j) {
        int muj = MU[j];
        int nuj = NU[j];

        lb[i] += cff[j] * mxi[mui][muj] * mzt[nui][nuj];
      }
    }
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
      if (f->dep.rob.m)
        free(f->dep.rob.m);

      if (f->dep.rob.b)
        free(f->dep.rob.b);

      break;
    case NEU:
      if (f->dep.neu.b)
        free(f->dep.neu.b);

      break;
    case DIR:
      break;
  }

  free(f);

  return 0;
}
