#include <fem/sse/const.h>
#include <fem/sse/fce.h>
#include <vec/mtx.h>

#include <stdlib.h>
#include "vec/vec.h"

struct fce* fce_new(enum type type) {
  struct fce* f = malloc(sizeof(struct fce));

  f->cnd.type = type;

  f->cnd.pps.dir.tmp = NULL;
  f->cnd.pps.neu.tta = NULL;
  f->cnd.pps.rob.tmp = NULL;

  if (type == DIR)
    return f;

  f->loc.rob.b = vec_new(4);

  if (type == ROB)
    f->loc.rob.m = mtx_new(4);

  return f;
}

struct fce* fce_get(const char* buf, double (**fun)(struct vtx*)) {
  int pos, pty;

  if (buf[0] != 'f')
    return NULL;

  buf += 1;

  sscanf(buf, "%d |%n", &pty, &pos);
  buf += pos;

  struct fce* f = fce_new(pty - 1);

  for (int j = 0; j < 4; ++j) {
    sscanf(buf, "%d%n", &pty, &pos);
    buf += pos;

    f->vtx[j] = pty - 1;
  }

  switch (f->cnd.type) {
    case DIR:
      sscanf(buf, " | %d%n", &pty, &pos);
      buf += pos;

      f->cnd.pps.dir.tmp = fun[pty - 1];

      break;
    case NEU:
      sscanf(buf, " | %d%n", &pty, &pos);
      buf += pos;

      f->cnd.pps.neu.tta = fun[pty - 1];

      break;
    case ROB:
      double bet;

      sscanf(buf, " | %d | %lf%n", &pty, &bet, &pos);
      buf += pos;

      f->cnd.pps.rob.tmp = fun[pty - 1];
      f->cnd.pps.rob.bet = bet;

      break;
  }

  return f;
}

static inline int fce_nrm(struct fce* f, struct vtx** v) {
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
  if (f->cnd.type == DIR)
    return 0;

  double mxi[2][2];
  double mzt[2][2];

  double hxi;
  double hzt;

  switch (fce_nrm(f, v)) {
    case 0:
      hxi = v[f->vtx[1]]->y - v[f->vtx[0]]->y;
      hzt = v[f->vtx[2]]->z - v[f->vtx[0]]->z;

      break;
    case 1:
      hxi = v[f->vtx[1]]->x - v[f->vtx[0]]->x;
      hzt = v[f->vtx[2]]->z - v[f->vtx[0]]->z;

      break;
    case 2:
      hxi = v[f->vtx[1]]->x - v[f->vtx[0]]->x;
      hzt = v[f->vtx[2]]->y - v[f->vtx[0]]->y;

      break;
  }

  for (int i = 0; i < 2; ++i)
    for (int j = 0; j < 2; ++j) {
      mxi[i][j] = hxi * M[i][j];
      mzt[i][j] = hzt * M[i][j];
    }

  if (f->cnd.type == NEU) {
    double* bloc = f->loc.neu.b->vp;

    double dec[4];

    for (int i = 0; i < 4; ++i)
      dec[i] = f->cnd.pps.neu.tta(v[f->vtx[i]]);

    for (int i = 0; i < 4; ++i) {
      bloc[i] = 0;

      int mui = MU[i];
      int nui = NU[i];

      for (int j = 0; j < 4; ++j) {
        int muj = MU[j];
        int nuj = NU[j];

        bloc[i] += dec[j] * mxi[mui][muj] * mzt[nui][nuj];
      }
    }

    return 0;
  }

  if (f->cnd.type == ROB) {
    double** mloc = f->loc.rob.m->v;
    double* bloc = f->loc.rob.b->vp;

    double bet = f->cnd.pps.rob.bet;
    double dec[4];

    for (int i = 0; i < 4; ++i)
      dec[i] = f->cnd.pps.rob.tmp(v[f->vtx[i]]);

    for (int i = 0; i < 4; ++i) {
      bloc[i] = 0;

      int mui = MU[i];
      int nui = NU[i];

      for (int j = 0; j < 4; ++j) {
        int muj = MU[j];
        int nuj = NU[j];

        mloc[i][j] = bet * mxi[mui][muj] * mzt[nui][nuj];
        bloc[i] += dec[j] * mxi[mui][muj] * mzt[nui][nuj];
      }

      bloc[i] *= bet;
    }
  }

  return 0;
}

static inline int fce_mov_neu(struct fce* f, struct vec* b) {
  double* bloc = f->loc.neu.b->vp;

  for (int i = 0; i < 4; ++i)
    b->vp[f->vtx[i]] += bloc[i];

  return 0;
}

static inline int fce_mov_rob(struct fce* f, struct mtx_csj* a, struct vec* b) {
  struct vec* bloc = f->loc.rob.b;
  struct mtx* mloc = f->loc.rob.m;

  for (int i = 0; i < 4; ++i) {
    int iglob = f->vtx[i];

    b->vp[iglob] += bloc->vp[i];

    for (int j = 0; j < 4; ++j) {
      int jglob = f->vtx[j];

      if (iglob == jglob) {
        a->dr[iglob] += mloc->v[i][j];
        continue;
      }

      if (iglob > jglob) {
        int lr0 = a->ia[iglob];
        int lr1 = a->ia[iglob + 1];

        for (int lr = lr0; lr < lr1; ++lr) {
          int lj = a->ja[lr];

          if (lj == jglob) {
            a->lr[lr] += mloc->v[i][j];
            break;
          }

          if (lj > jglob)
            break;
        }
      } else {
        int ur0 = a->ia[jglob];
        int ur1 = a->ia[jglob + 1];

        for (int ur = ur0; ur < ur1; ++ur) {
          int ui = a->ja[ur];

          if (ui == iglob) {
            a->ur[ur] += mloc->v[i][j];
            break;
          }

          if (ui > iglob)
            break;
        }
      }
    }
  }

  return 0;
}

int fce_mov(struct fce* f, struct mtx_csj* a, struct vec* b) {
  switch (f->cnd.type) {
    case DIR:
      return 0;
    case NEU:
      return fce_mov_neu(f, b);
    case ROB:
      return fce_mov_rob(f, a, b);
  }

  return 0;
}

int fce_cls(struct fce* f) {
  if (!f)
    return 0;

  if (f->cnd.type == DIR)
    return 0;

  vec_cls(f->loc.rob.b);

  if (f->cnd.type == ROB)
    mtx_cls(f->loc.rob.m);

  free(f);

  return 0;
}
