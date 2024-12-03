#include <vec/mtx_cds.h>

#include <stdlib.h>
#include <string.h>

struct mtx_cds* mtx_cds_new(int n, int c) {
  struct mtx_cds* mp = malloc(sizeof(struct mtx_cds));

  mp->pps.n = n;
  mp->pps.c = c;

  mp->la = malloc(sizeof(int) * c);
  mp->ad = malloc(sizeof(double*) * n);

  for (int i = 0; i < n; ++i) {
    mp->ad[i] = malloc(sizeof(double) * c);
    memset(mp->ad[i], 0, sizeof(double) * c);
  }

  return mp;
}

int mtx_cds_fget(FILE* f, struct mtx_cds* mp) {
  int n = mp->pps.n;
  int c = mp->pps.c;

  int* la = mp->la;
  double** ad = mp->ad;

  for (int k = 0; k < c; ++k) {
    fscanf(f, "%d", &la[k]);

    if (la[k] == 0)
      mp->pps.d = k;
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
  int n = mp->pps.n;
  int c = mp->pps.c;

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

int mtx_cds_vmlt(struct mtx_cds* ap, struct vec* xp, struct vec* fp) {
  int n = ap->pps.n;
  int c = ap->pps.c;

  double* xvp = xp->vp;
  double* fvp = fp->vp;

  int* la = ap->la;
  double** ad = ap->ad;

  memset(fvp, 0, sizeof(double) * n);

  for (int k = 0; k < c; ++k) {
    int i0 = la[k] < 0 ? -la[k] : 0;
    int i1 = la[k] < 0 ? n : n - la[k];

    for (int i = i0; i < i1; ++i) {
      int j = i + la[k];
      fvp[i] += ad[i][k] * xvp[j];
    }
  }

  return 0;
}

int mtx_cds_cls(struct mtx_cds* mp) {
  for (int i = 0; i < mp->pps.n; ++i)
    free(mp->ad[i]);

  free(mp->ad);
  free(mp->la);
  free(mp);

  return 0;
}
