#include <mtx_csr.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define min(a, b) (((a) < (b)) ? (a) : (b))

struct mtx_csr* mtx_csr_new(int n, int ne) {
  struct mtx_csr* mp = malloc(sizeof(struct mtx_csr));

  mp->n = n;
  mp->ne = ne;

  mp->ar = malloc(sizeof(double) * ne);
  mp->ia = malloc(sizeof(int) * (n + 1));
  mp->ja = malloc(sizeof(int) * ne);

  return mp;
}

int mtx_csr_fget(FILE* f, struct mtx_csr* mp) {
  int n = mp->n;
  int ne = mp->ne;

  double* ar = mp->ar;

  int* ia = mp->ia;
  int* ja = mp->ja;

  for (int i = 0; i < ne; ++i)
    fscanf(f, "%lf", &ar[i]);

  for (int i = 0; i <= n; ++i)
    fscanf(f, "%d", &ia[i]);

  for (int i = 0; i < ne; ++i)
    fscanf(f, "%d", &ja[i]);

  return 0;
}

static int mtx_csr_fput_pty(FILE* f, struct mtx_csr* mp) {
  int n = mp->n;

  double* ar = mp->ar;

  int* ia = mp->ia;
  int* ja = mp->ja;

  for (int i = 0; i < n; ++i) {
    int r = ia[i];

    for (int j = 0; j < n; ++j)
      if (r < ia[i + 1] && ja[r] == j) {
        fprintf(f, "%10.3e ", ar[r]);
        r += 1;
      } else
        fprintf(f, "%10.3e ", 0.0);

    putc('\n', f);
  }

  return 0;
}

int mtx_csr_fput(FILE* f, struct mtx_csr* mp, int pty) {
  if (pty)
    return mtx_csr_fput_pty(f, mp);

  int n = mp->n;
  int ne = mp->ne;

  double* ar = mp->ar;

  int* ia = mp->ia;
  int* ja = mp->ja;

  for (int i = 0; i < ne; ++i)
    fprintf(f, "%10.3e ", ar[ne]);

  putc('\n', f);

  for (int i = 0; i < n; ++i)
    fprintf(f, "%5d ", ia[i]);

  putc('\n', f);

  for (int i = 0; i < ne; ++i)
    fprintf(f, "%5d ", ja[i]);

  putc('\n', f);

  return 0;
}

int mtx_csr_ilu(struct mtx_csr* mp, struct mtx_csr* rp) {
  int n = mp->n;
  int ne = mp->ne;

  double* mar = mp->ar;
  double* rar = rp->ar;

  int* ia = mp->ia;
  int* ja = mp->ja;

  memcpy(rp->ia, mp->ia, sizeof(int) * (n + 1));
  memcpy(rp->ja, mp->ja, sizeof(int) * ne);

  double* dr = malloc(sizeof(double) * n);

  for (int i = 0; i < n; ++i) {
    int r0 = ia[i];
    int r1 = ia[i + 1];

    for (int r = r0; r < r1; ++r) {
      int j = ja[r];
      double s = 0;

      for (int rl = r0; rl < r1 && ja[rl] < min(i, j); ++rl) {
        int jl = ja[rl];

        for (int ru = ia[jl]; ru < ia[jl + 1]; ++ru)
          if (ja[ru] == j) {
            s += rar[rl] * rar[ru];
            break;
          }
      }

      rar[r] = mar[r] - s;

      if (j == i) {
        rar[r] = sqrt(fabs(rar[r]));
        dr[i] = rar[r];
      } else
        rar[r] = rar[r] / dr[min(i, j)];
    }
  }

  free(dr);

  return 0;
}

int mtx_csr_vmlt(struct mtx_csr* mp, struct vec* xp, struct vec* fp) {
  int n = mp->n;

  int* ia = mp->ia;
  int* ja = mp->ja;

  double* ar = mp->ar;
  double* xv = xp->vp;
  double* fv = fp->vp;

  for (int i = 0; i < n; ++i) {
    fv[i] = 0;

    for (int r = ia[i]; r < ia[i + 1]; ++r) {
      int j = ja[r];

      fv[i] += xv[j] * ar[r];
    }
  }

  return 0;
}

void mtx_csr_free(struct mtx_csr* mp) {
  free(mp->ar);
  free(mp->ia);
  free(mp->ja);
  free(mp);
}
