#include <vec/mtx_sky.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define min(x, y) (((x) < (y)) ? (x) : (y))

struct mtx_sky* mtx_sky_new(int n, int s) {
  struct mtx_sky* mp = malloc(sizeof(struct mtx_sky));

  mp->pps.n = n;
  mp->pps.s = s;
  mp->p = malloc(sizeof(int) * (n + 1));

  mp->dv = malloc(sizeof(double) * n);
  mp->lv = malloc(sizeof(double) * s);
  mp->uv = malloc(sizeof(double) * s);

  return mp;
}

int mtx_sky_fget(FILE* f, struct mtx_sky* mp) {
  int n = mp->pps.n;
  int s = mp->pps.s;

  int* mpp = mp->p;

  double* mlp = mp->lv;
  double* mup = mp->uv;
  double* mdp = mp->dv;

  for (int i = 0; i <= n; ++i)
    fscanf(f, "%d", &mpp[i]);

  for (int i = 0; i < n; ++i)
    fscanf(f, "%lf", &mdp[i]);

  for (int i = 0; i < s; ++i)
    fscanf(f, "%lf", &mlp[i]);

  for (int i = 0; i < s; ++i)
    fscanf(f, "%lf", &mup[i]);

  return 0;
}

int mtx_sky_fput(FILE* f, struct mtx_sky* mp) {
  int n = mp->pps.n;
  int s = mp->pps.s;

  int* mpp = mp->p;

  double* mlp = mp->lv;
  double* mup = mp->uv;
  double* mdp = mp->dv;

  for (int i = 0; i <= n; ++i)
    fprintf(f, "%u ", mpp[i]);

  fputc('\n', f);

  for (int i = 0; i < n; ++i)
    fprintf(f, "%.4e ", mdp[i]);

  fputc('\n', f);

  for (int i = 0; i < s; ++i)
    fprintf(f, "%.4e ", mlp[i]);

  fputc('\n', f);

  for (int i = 0; i < s; ++i)
    fprintf(f, "%.4e ", mup[i]);

  return 0;
}

int mtx_sky_ldu(struct mtx_sky* mp) {
  int n = mp->pps.n;

  int* mpp = mp->p;

  double* mdp = mp->dv;
  double* mlp = mp->lv;
  double* mup = mp->uv;

  for (int i = 0; i < n; ++i) {
    int ic = mpp[i + 1] - mpp[i];
    double dsum = mdp[i];

    for (int k = 1; k <= ic; ++k)
      dsum -= mlp[mpp[i] + ic - k] * mup[mpp[i] + ic - k] * mdp[i - k];

    mdp[i] = dsum;

    for (int j = i + 1; j < n; ++j) {
      int jc = mpp[j + 1] - mpp[j] - j + i;

      if (jc > -1) {
        double lsum = mlp[mpp[j] + jc];
        double usum = mup[mpp[j] + jc];

        for (int k = 1; k <= min(jc, ic); ++k) {
          lsum -= mlp[mpp[j] + jc - k] * mup[mpp[i] + ic - k] * mdp[i - k];
          usum -= mup[mpp[j] + jc - k] * mlp[mpp[i] + ic - k] * mdp[i - k];
        }

        mlp[mpp[j] + jc] = lsum / mdp[i];
        mup[mpp[j] + jc] = usum / mdp[i];
      }
    }
  }

  return 0;
}

int mtx_sky_hlb(struct mtx_sky* mp) {
  int n = mp->pps.n;
  int ir = 0;

  int* mpp = mp->p;

  double* mlp = mp->lv;
  double* mup = mp->uv;
  double* mdp = mp->dv;

  mpp[0] = ir;
  mdp[0] = 1;

  for (int i = 1, i2 = 2; i < n; ++i, i2 += 2) {
    ir += i - 1;

    mpp[i] = ir;
    mdp[i] = 1.0 / (i2 + 1.0);

    for (int j = 0; j < i; ++j) {
      double v = 1.0 / (i + j + 1.0);

      mlp[ir + j] = v;
      mup[ir + j] = v;
    }
  }

  mpp[n] = n * (n - 1) / 2;

  return 0;
}

int mtx_sky_vmlt(struct mtx_sky* ap, struct vec* bp, struct vec* rp) {
  int n = ap->pps.n;

  int* app = ap->p;

  double* adp = ap->dv;
  double* alp = ap->lv;
  double* aup = ap->uv;
  double* bvp = bp->vp;
  double* rvp = rp->vp;

  memset(rvp, 0, n * sizeof(double));

  for (int i = 0; i < n; ++i) {
    rvp[i] += adp[i] * bvp[i];

    int k0 = app[i + 1] - 1;
    int k1 = app[i];

    for (int k = k0, j = i - 1; k >= k1; --k, --j) {
      rvp[i] += alp[k] * bvp[j];
      rvp[j] += aup[k] * bvp[i];
    }
  }

  return 0;
}

void mtx_sky_free(struct mtx_sky* mp) {
  free(mp->dv);
  free(mp->lv);
  free(mp->uv);
  free(mp->p);
  free(mp);
}
