#include <mtx_cds.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>

struct mtx_cds* mtx_cds_new(int n, int c) {
  struct mtx_cds* mp = malloc(sizeof(struct mtx_cds));

  mp->n = n;
  mp->c = c;

  mp->la = malloc(sizeof(int) * c);
  mp->ad = malloc(sizeof(double*) * n);

  for (int i = 0; i < n; ++i) {
    mp->ad[i] = malloc(sizeof(double) * c);
    memset(mp->ad[i], 0, sizeof(double) * c);
  }

  return mp;
}

int mtx_cds_fget(FILE* f, struct mtx_cds* mp) {
  int n = mp->n;
  int c = mp->c;

  int* la = mp->la;
  double** ad = mp->ad;

  for (int k = 0; k < c; ++k) {
    fscanf(f, "%d", &la[k]);

    if (la[k] == 0)
      mp->d = k;
  }

  for (int k = 0; k < c; ++k) {
    int i0 = la[k] < 0 ? -la[k] : 0;
    int i1 = la[k] < 0 ? n : n - la[k];

    for (int i = i0; i < i1; ++i)
      fscanf(f, "%lf", &ad[i][k]);
  }

  return 0;
}

int mtx_cds_fput(FILE* f, struct mtx_cds* mp) {
  int n = mp->n;
  int c = mp->c;

  int* la = mp->la;
  double** ad = mp->ad;

  for (int k = 0; k < c; ++k)
    fprintf(f, "%3d ", la[k]);

  fputc('\n', f);

  for (int k = 0; k < c; ++k) {
    int i0 = la[k] < 0 ? -la[k] : 0;
    int i1 = la[k] < 0 ? n : n - la[k];

    for (int i = i0; i < i1; ++i) {
      fprintf(f, "%10.3e ", ad[i][k]);
    }

    fputc('\n', f);
  }

  return 0;
}

int mtx_cds_vmlt(struct mtx_cds* ap, struct vec* xp, struct vec* rp) {
  int n = ap->n;
  int c = ap->c;

  double* xvp = xp->vp;
  double* rvp = rp->vp;

  int* la = ap->la;
  double** ad = ap->ad;

  memset(rvp, 0, sizeof(double) * n);

  for (int k = 0; k < c; ++k) {
    int i0 = la[k] < 0 ? -la[k] : 0;
    int i1 = la[k] < 0 ? n : n - la[k];

    for (int i = i0; i < i1; ++i) {
      int j = i + la[k];
      rvp[i] += ad[i][k] * xvp[j];
    }
  }

  return 0;
}

int mtx_cds_ddm(struct mtx_cds* mp, int m, int k) {
  int n = mp->n;
  int c = mp->c;

  if (c != 7)
    return -1;

  int* la = mp->la;
  double** ad = mp->ad;

  la[0] = 0;
  la[1] = 1;
  la[2] = 2 + m;
  la[3] = 3 + m + k;
  la[4] = -la[1];
  la[5] = -la[2];
  la[6] = -la[3];

  mp->d = 0;

  for (int k = 1; k < c; ++k) {
    int i0 = la[k] < 0 ? -la[k] : 0;
    int i1 = la[k] < 0 ? n : n - la[k];

    for (int i = i0; i < i1; ++i) {
      ad[i][k] = -(rand() % 5);
      ad[i][0] -= ad[i][k];
    }
  }

  return 0;
}

void mtx_cds_free(struct mtx_cds* mp) {
  for (int i = 0; i < mp->n; ++i)
    free(mp->ad[i]);

  free(mp->ad);
  free(mp->la);
  free(mp);
}
