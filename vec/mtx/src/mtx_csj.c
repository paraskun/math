#include <mtx_csj.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct mtx_csj* mtx_csj_new(struct mtx_csj_pps pps) {
  struct mtx_csj* mp = malloc(sizeof(struct mtx_csj));

  mp->pps = pps;

  int n = pps.n;
  int ne = pps.ne;

  mp->dr = malloc(sizeof(double) * n);

  memset(mp->dr, 0, sizeof(double) * n);

  mp->lr = NULL;
  mp->ur = NULL;
  mp->ia = NULL;
  mp->ja = NULL;

  if (ne > 0) {
    mp->lr = malloc(sizeof(double) * ne);
    mp->ur = malloc(sizeof(double) * ne);
    mp->ia = malloc(sizeof(int) * (n + 1));
    mp->ja = malloc(sizeof(int) * ne);

    memset(mp->lr, 0, sizeof(double) * ne);
    memset(mp->ur, 0, sizeof(double) * ne);
    memset(mp->ia, 0, sizeof(int) * (n + 1));
    memset(mp->ja, 0, sizeof(int) * ne);
  }

  return mp;
}

int mtx_csj_put(struct mtx_csj_pkt* pkt, struct mtx_csj* mp) {
  int n = mp->pps.n;
  int ne = mp->pps.ne;

  double* dr = mp->dr;
  double* lr = mp->lr;
  double* ur = mp->ur;

  int* ia = mp->ia;
  int* ja = mp->ja;

  for (int i = 0; i < n; ++i)
    fprintf(pkt->dr, "%.7e\n", dr[i]);

  for (int i = 0; i < ne; ++i)
    fprintf(pkt->lr, "%.7e\n", lr[i]);

  for (int i = 0; i < ne; ++i)
    fprintf(pkt->ur, "%.7e\n", ur[i]);

  for (int i = 0; i < n + 1; ++i)
    fprintf(pkt->ia, "%d\n", ia[i]);

  for (int i = 0; i < ne; ++i)
    fprintf(pkt->ja, "%d\n", ja[i]);

  return 0;
}

int mtx_csj_get(struct mtx_csj_pkt* pkt, struct mtx_csj* mp) {
  int n = mp->pps.n;
  int ne = mp->pps.ne;

  double* dr = mp->dr;
  double* lr = mp->lr;
  double* ur = mp->ur;

  int* ia = mp->ia;
  int* ja = mp->ja;

  fseek(pkt->dr, 0, SEEK_SET);

  for (int i = 0; i < n; ++i)
    fscanf(pkt->dr, "%lf", &dr[i]);

  fseek(pkt->lr, 0, SEEK_SET);

  for (int i = 0; i < ne; ++i)
    fscanf(pkt->lr, "%lf", &lr[i]);

  fseek(pkt->ur, 0, SEEK_SET);

  for (int i = 0; i < ne; ++i)
    fscanf(pkt->ur, "%lf", &ur[i]);

  fseek(pkt->ia, 0, SEEK_SET);

  for (int i = 0; i < n + 1; ++i)
    fscanf(pkt->ia, "%d", &ia[i]);

  fseek(pkt->ja, 0, SEEK_SET);

  for (int i = 0; i < ne; ++i)
    fscanf(pkt->ja, "%d", &ja[i]);

  return 0;
}

int mtx_csj_ilu(struct mtx_csj* mp, struct mtx_csj* rp) {
  int n = mp->pps.n;
  int ne = mp->pps.ne;

  int* mia = mp->ia;
  int* mja = mp->ja;

  int* ria = rp->ia;
  int* rja = rp->ja;

  memcpy(ria, mia, sizeof(int) * (n + 1));
  memcpy(rja, mja, sizeof(int) * ne);

  double* mdr = mp->dr;
  double* mlr = mp->lr;
  double* mur = mp->ur;

  double* rdr = rp->dr;
  double* rlr = rp->lr;
  double* rur = rp->ur;

  for (int d = 0; d < n; ++d) {
    int lr0 = ria[d];
    int lr1 = ria[d + 1];

    double sd = 0;

    for (int lr = lr0; lr < lr1; ++lr) {
      int j = rja[lr];

      int ur0 = ria[j];
      int ur1 = ria[j + 1];

      double sl = 0;
      double su = 0;

      for (int lrr = lr0, urr = ur0; urr < ur1 && lrr < lr;) {
        int lj = rja[lrr];
        int ui = rja[urr];

        if (lj == ui) {
          sl += rlr[lrr] * rur[urr];
          su += rlr[urr] * rur[lrr];

          lrr += 1;
          urr += 1;

          continue;
        }

        if (lj < ui)
          lrr += 1;
        else
          urr += 1;
      }

      rlr[lr] = (mlr[lr] - sl) / rdr[j];
      rur[lr] = (mur[lr] - su) / rdr[j];

      sd += rlr[lr] * rur[lr];
    }

    rdr[d] = sqrt(mdr[d] - sd);
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

int mtx_csj_all(struct mtx_csj* mp) {
  int n = mp->pps.n;
  int ne = mp->pps.ne;

  int* ia = mp->ia;
  int* ja = mp->ja;

  for (int i = 0, ar = 0; i < n; ++i) {
    ia[i] = ar;

    for (int j = 0; j < i; ++j, ++ar)
      ja[ar] = j;
  }

  ia[n] = ne;

  return 0;
}

int mtx_csj_ddm(struct mtx_csj* mp, int k) {
  int n = mp->pps.n;

  int* ia = mp->ia;
  int* ja = mp->ja;

  double* dr = mp->dr;
  double* lr = mp->lr;
  double* ur = mp->ur;

  memset(dr, 0, sizeof(double) * n);

  for (int i = 1; i < n; ++i) {
    int ar0 = ia[i];
    int ar1 = ia[i + 1];

    for (int ar = ar0; ar < ar1; ++ar) {
      int j = ja[ar];

      lr[ar] = -(rand() % 5);
      ur[ar] = -(rand() % 5);

      dr[i] -= lr[ar];
      dr[j] -= ur[ar];

      lr[ar] *= k;
      ur[ar] *= k;
    }
  }

  dr[0] += 1;

  return 0;
}

int mtx_csj_hlb(struct mtx_csj* mp) {
  int n = mp->pps.n;

  int* ia = mp->ia;
  int* ja = mp->ja;

  double* dr = mp->dr;
  double* lr = mp->lr;
  double* ur = mp->ur;

  for (int i = 0; i < n; ++i) {
    dr[i] = 1.0 / (i + i + 1);

    int ar0 = ia[i];
    int ar1 = ia[i + 1];

    for (int ar = ar0; ar < ar1; ++ar) {
      int j = ja[ar];

      lr[ar] = 1.0 / (i + j + 1);
      ur[ar] = lr[ar];
    }
  }

  return 0;
}

int mtx_csj_vmlt(struct mtx_csj* mp, struct vec* xp, struct vec* fp) {
  int n = mp->pps.n;

  int* ia = mp->ia;
  int* ja = mp->ja;

  double* dr = mp->dr;
  double* lr = mp->lr;
  double* ur = mp->ur;

  double* xv = xp->vp;
  double* fv = fp->vp;

  for (int i = 0; i < n; ++i) {
    fv[i] = xv[i] * dr[i];

    int ar0 = ia[i];
    int ar1 = ia[i + 1];

    for (int ar = ar0; ar < ar1; ++ar) {
      int j = ja[ar];

      fv[i] += xv[j] * lr[ar];
      fv[j] += xv[i] * ur[ar];
    }
  }

  return 0;
}

void mtx_csj_cls(struct mtx_csj* mp) {
  free(mp->dr);

  if (mp->pps.ne > 0) {
    free(mp->lr);
    free(mp->ur);
    free(mp->ia);
    free(mp->ja);
  }

  free(mp);
}

void mtx_csj_pkt_cls(struct mtx_csj_pkt* pkt) {
  if (pkt->pps)
    fclose(pkt->pps);

  if (pkt->dr)
    fclose(pkt->dr);

  if (pkt->lr)
    fclose(pkt->lr);

  if (pkt->ur)
    fclose(pkt->ur);

  if (pkt->ia)
    fclose(pkt->ia);

  if (pkt->ja)
    fclose(pkt->ja);
}
