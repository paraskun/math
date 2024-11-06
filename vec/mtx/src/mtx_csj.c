#include <mtx_csj.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct mtx_csj* mtx_csj_new(int n, int le, int ue) {
  struct mtx_csj* mp = malloc(sizeof(struct mtx_csj));

  mp->n = n;
  mp->le = le;
  mp->ue = ue;

  mp->dr = malloc(sizeof(double) * n);
  mp->lr = malloc(sizeof(double) * le);
  mp->ur = malloc(sizeof(double) * ue);

  mp->il = malloc(sizeof(int) * (n + 1));
  mp->ju = malloc(sizeof(int) * (n + 1));

  mp->jl = malloc(sizeof(int) * le);
  mp->iu = malloc(sizeof(int) * ue);

  return mp;
}

int mtx_csj_fget(FILE* f, struct mtx_csj* mp) {
  int n = mp->n;
  int le = mp->le;
  int ue = mp->ue;

  double* dr = mp->dr;
  double* lr = mp->lr;
  double* ur = mp->ur;

  int* il = mp->il;
  int* ju = mp->ju;

  int* jl = mp->jl;
  int* iu = mp->iu;

  for (int i = 0; i < n; ++i)
    fscanf(f, "%lf", &dr[i]);

  for (int i = 0; i < le; ++i)
    fscanf(f, "%lf", &lr[i]);

  for (int i = 0; i < le; ++i)
    fscanf(f, "%d", &jl[i]);

  for (int i = 0; i <= n; ++i)
    fscanf(f, "%d", &il[i]);

  for (int i = 0; i < ue; ++i)
    fscanf(f, "%lf", &ur[i]);

  for (int i = 0; i < ue; ++i)
    fscanf(f, "%d", &iu[i]);

  for (int i = 0; i <= n; ++i)
    fscanf(f, "%d", &ju[i]);

  return 0;
}

int mtx_csj_ilu(struct mtx_csj* mp, struct mtx_csj* rp) {
  int n = mp->n;
  int le = mp->le;
  int ue = mp->ue;

  int* mil = mp->il;
  int* mjl = mp->jl;
  int* mju = mp->ju;
  int* miu = mp->iu;

  int* ril = rp->il;
  int* rjl = rp->jl;
  int* rju = rp->ju;
  int* riu = rp->iu;

  memcpy(ril, mil, sizeof(int) * (n + 1));
  memcpy(rjl, mjl, sizeof(int) * le);
  memcpy(rju, mju, sizeof(int) * (n + 1));
  memcpy(riu, miu, sizeof(int) * ue);

  double* mdr = mp->dr;
  double* mlr = mp->lr;
  double* mur = mp->ur;

  double* rdr = rp->dr;
  double* rlr = rp->lr;
  double* rur = rp->ur;

  for (int d = 0; d < n; ++d) {
    int lr0 = mil[d];
    int lr1 = mil[d + 1];

    for (int lr = lr0; lr < lr1; ++lr) {
      int lj = mjl[lr];

      rlr[lr] = mlr[lr];

      int ur0 = mju[lj];
      int ur1 = mju[lj + 1];

      for (int llr = lr0, uur = ur0; llr < lr && uur < ur1;) {
        int llj = mjl[llr];
        int uui = miu[uur];

        if (llj == uui) {
          rlr[lr] -= rlr[llr] * rur[uur];

          llr += 1;
          uur += 1;

          continue;
        }

        if (llj < uui)
          llr += 1;
        else
          uur += 1;
      }

      rlr[lr] /= rdr[lj];
    }

    int ur0 = mju[d];
    int ur1 = mju[d + 1];

    for (int ur = ur0; ur < ur1; ++ur) {
      int ui = miu[ur];

      rur[ur] = mur[ur];

      int lr0 = mil[ui];
      int lr1 = mil[ui + 1];

      for (int llr = lr0, uur = ur0; llr < lr1 && uur < ur;) {
        int llj = mjl[llr];
        int uui = miu[uur];

        if (llj == uui) {
          rur[ur] -= rlr[llr] * rur[uur];

          llr += 1;
          uur += 1;

          continue;
        }

        if (llj < uui)
          llr += 1;
        else
          uur += 1;
      }

      rur[ur] /= rdr[ui];
    }

    rdr[d] = mdr[d];

    for (int lr = lr0, ur = ur0; lr < lr1 && ur < ur1;) {
      int lj = mjl[lr];
      int ui = miu[ur];

      if (lj == ui) {
        rdr[d] -= rlr[lr] * rur[ur];

        lr += 1;
        ur += 1;

        continue;
      }

      if (lj < ui)
        lr += 1;
      else
        ur += 1;
    }

    rdr[d] = sqrt(fabs(rdr[d]));
  }

  return 0;
}

int mtx_csj_vmlt(struct mtx_csj* mp, struct vec* xp, struct vec* fp) {
  int n = mp->n;

  int* mil = mp->il;
  int* mjl = mp->jl;
  int* mju = mp->ju;
  int* miu = mp->iu;

  double* mdr = mp->dr;
  double* mlr = mp->lr;
  double* mur = mp->ur;

  double* xv = xp->vp;
  double* fv = fp->vp;

  for (int i = 0; i < n; ++i)
    fv[i] = xv[i] * mdr[i];

  for (int i = 1; i < n; ++i) {
    int lr0 = mil[i];
    int lr1 = mil[i + 1];

    for (int lr = lr0; lr < lr1; ++lr) {
      int lj = mjl[lr];

      fv[i] += xv[lj] * mlr[lr];
    }
  }

  for (int j = 1; j < n; ++j) {
    int ur0 = mju[j];
    int ur1 = mju[j + 1];

    for (int ur = ur0; ur < ur1; ++ur) {
      int ui = miu[ur];

      fv[ui] += xv[j] * mur[ur];
    }
  }

  return 0;
}

void mtx_csj_free(struct mtx_csj* mp) {
  free(mp->dr);
  free(mp->lr);
  free(mp->ur);

  free(mp->il);
  free(mp->ju);
  free(mp->iu);
  free(mp->jl);

  free(mp);
}
