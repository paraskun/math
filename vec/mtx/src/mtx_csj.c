#include <mtx_csj.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct mtx_csj* mtx_csj_new(struct mtx_csj_pps pps) {
  struct mtx_csj* mp = malloc(sizeof(struct mtx_csj));

  mp->pps = pps;

  int n = pps.n;
  int le = pps.le;
  int ue = pps.ue;

  mp->dr = malloc(sizeof(double) * n);

  if (le > 0) {
    mp->lr = malloc(sizeof(double) * le);
    mp->jl = malloc(sizeof(int) * le);
    mp->il = malloc(sizeof(int) * (n + 1));
  }

  if (ue > 0) {
    mp->ur = malloc(sizeof(double) * ue);
    mp->ju = malloc(sizeof(int) * (n + 1));
    mp->iu = malloc(sizeof(int) * ue);
  }

  return mp;
}

int mtx_csj_fget(struct mtx_csj_fio* f, struct mtx_csj* mp) {
  int n = mp->pps.n;
  int le = mp->pps.le;
  int ue = mp->pps.ue;

  double* dr = mp->dr;
  double* lr = mp->lr;
  double* ur = mp->ur;

  int* il = mp->il;
  int* ju = mp->ju;

  int* jl = mp->jl;
  int* iu = mp->iu;

  fseek(f->dr, 0, SEEK_SET);

  for (int i = 0; i < n; ++i)
    fscanf(f->dr, "%lf", &dr[i]);

  fseek(f->lr, 0, SEEK_SET);

  for (int i = 0; i < le; ++i)
    fscanf(f->lr, "%lf", &lr[i]);

  fseek(f->jl, 0, SEEK_SET);

  for (int i = 0; i < le; ++i)
    fscanf(f->jl, "%d", &jl[i]);

  fseek(f->il, 0, SEEK_SET);

  for (int i = 0; i <= n; ++i)
    fscanf(f->il, "%d", &il[i]);

  fseek(f->ur, 0, SEEK_SET);

  for (int i = 0; i < ue; ++i)
    fscanf(f->ur, "%lf", &ur[i]);

  fseek(f->iu, 0, SEEK_SET);

  for (int i = 0; i < ue; ++i)
    fscanf(f->iu, "%d", &iu[i]);

  fseek(f->ju, 0, SEEK_SET);

  for (int i = 0; i <= n; ++i)
    fscanf(f->ju, "%d", &ju[i]);

  return 0;
}

int mtx_csj_ilu(struct mtx_csj* mp, struct mtx_csj* rp) {
  int n = mp->pps.n;
  int le = mp->pps.le;
  int ue = mp->pps.ue;

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

      int ur0 = mju[lj];
      int ur1 = mju[lj + 1];

      double s = 0;

      for (int llr = lr0, uur = ur0; llr < lr && uur < ur1;) {
        int llj = mjl[llr];
        int uui = miu[uur];

        if (llj == uui) {
          s += rlr[llr] * rur[uur];

          llr += 1;
          uur += 1;

          continue;
        }

        if (llj < uui)
          llr += 1;
        else
          uur += 1;
      }

      rlr[lr] = (mlr[lr] - s) / rdr[lj];
    }

    int ur0 = mju[d];
    int ur1 = mju[d + 1];

    for (int ur = ur0; ur < ur1; ++ur) {
      int ui = miu[ur];

      int lr0 = mil[ui];
      int lr1 = mil[ui + 1];

      double s = 0;

      for (int llr = lr0, uur = ur0; llr < lr1 && uur < ur;) {
        int llj = mjl[llr];
        int uui = miu[uur];

        if (llj == uui) {
          s += rlr[llr] * rur[uur];

          llr += 1;
          uur += 1;

          continue;
        }

        if (llj < uui)
          llr += 1;
        else
          uur += 1;
      }

      rur[ur] = (mur[ur] - s) / rdr[ui];
    }

    double s = 0;

    for (int lr = lr0, ur = ur0; lr < lr1 && ur < ur1;) {
      int lj = mjl[lr];
      int ui = miu[ur];

      if (lj == ui) {
        s += rlr[lr] * rur[ur];

        lr += 1;
        ur += 1;

        continue;
      }

      if (lj < ui)
        lr += 1;
      else
        ur += 1;
    }

    rdr[d] = sqrt(mdr[d] - s);
  }

  return 0;
}

int mtx_csj_dgl(struct mtx_csj* mp, struct mtx_csj* rp) {
  int n = mp->pps.n;

  double* rdr = rp->dr;
  double* mdr = mp->dr;

  for (int i = 0; i < n; ++i)
    rdr[i] = sqrt(mdr[i]);

  return 0;
}

int mtx_csj_vmlt(struct mtx_csj* mp, struct vec* xp, struct vec* fp) {
  int n = mp->pps.n;

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

    double s = 0;

    for (int lr = lr0; lr < lr1; ++lr) {
      int lj = mjl[lr];

      s += xv[lj] * mlr[lr];
    }

    fv[i] += s;
  }

  for (int j = 1; j < n; ++j) {
    int ur0 = mju[j];
    int ur1 = mju[j + 1];

    double x = xv[j];

    for (int ur = ur0; ur < ur1; ++ur) {
      int ui = miu[ur];

      fv[ui] += x * mur[ur];
    }
  }

  return 0;
}

void mtx_csj_free(struct mtx_csj* mp) {
  free(mp->dr);

  if (mp->pps.le > 0) {
    free(mp->lr);
    free(mp->jl);
    free(mp->il);
  }

  if (mp->pps.ue > 0) {
    free(mp->ur);
    free(mp->ju);
    free(mp->iu);
  }

  free(mp);
}

void mtx_csj_fio_close(struct mtx_csj_fio* f) {
  fclose(f->pps);

  fclose(f->lr);
  fclose(f->ur);
  fclose(f->dr);

  fclose(f->il);
  fclose(f->jl);
  fclose(f->iu);
  fclose(f->ju);
}
