#include <smtx.h>

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define min(x, y) (((x) < (y)) ? (x) : (y))

struct smtx* smtx_new(int n, int s) {
  struct smtx* mp = malloc(sizeof(struct smtx));

  mp->n = n;
  mp->s = s;
  mp->p = malloc(sizeof(int) * (n + 1));
  mp->d = malloc(sizeof(real) * n);
  mp->l = malloc(sizeof(real) * s);
  mp->u = malloc(sizeof(real) * s);

  return mp;
}

void smtx_ddm(struct smtx* mp, int k) {
  int n = mp->n;

  real* mlp = mp->l;
  real* mup = mp->u;
  real* mdp = mp->d;
  int* mpp = mp->p;

  preal sum = 0;

  mpp[0] = 0;

  for (int i = 1, ir = 0; i < n; ++i, ir += i - 1) {
    mpp[i] = ir;
    mlp[ir] = -(rand() % 4) - 1;
    mup[ir] = -(rand() % 4) - 1;

    sum += mlp[ir] + mup[ir];

    for (int j = 1; j < i; ++j) {
      mlp[ir + j] = -(rand() % 5);
      mup[ir + j] = -(rand() % 5);

      sum += mlp[ir + j] + mup[ir + j];
    }
  }

  mpp[n] = n * (n - 1) / 2;
  mdp[0] = 1.0 / pow(10.0, k) - sum;

  for (int i = 1; i < n; ++i)
    mdp[i] = -sum;
}

void smtx_hlb(struct smtx* mp) {
  int n = mp->n;
  int ir = 0;

  real* mlp = mp->l;
  real* mup = mp->u;
  real* mdp = mp->d;
  int* mpp = mp->p;

  mpp[0] = ir;
  mdp[0] = 1;

  for (int i = 1, i2 = 2; i < n; ++i, i2 += 2) {
    ir += i - 1;

    mpp[i] = ir;
    mdp[i] = 1.0 / (i2 + 1.0);

    for (int j = 0; j < i; ++j) {
      real v = 1.0 / (i + j + 1.0);

      mlp[ir + j] = v;
      mup[ir + j] = v;
    }
  }

  mpp[n] = n * (n - 1) / 2;
}

void smtx_fget(FILE* f, struct smtx* mp) {
  int n = mp->n;
  int s = mp->s;

  real* mlp = mp->l;
  real* mup = mp->u;
  real* mdp = mp->d;
  int* mpp = mp->p;

  for (int i = 0; i <= n; ++i)
    fscanf(f, "%d", &mpp[i]);

  for (int i = 0; i < n; ++i)
    fscanf(f, "%lf", &mdp[i]);

  for (int i = 0; i < s; ++i)
    fscanf(f, "%lf", &mlp[i]);

  for (int i = 0; i < s; ++i)
    fscanf(f, "%lf", &mup[i]);
}

void smtx_vmlt(struct smtx* ap, struct vec* bp, struct vec* rp) {
  int n = ap->n;

  int* app = ap->p;
  real* adp = ap->d;
  real* alp = ap->l;
  real* aup = ap->u;
  real* bvp = bp->v;
  real* rvp = rp->v;

  memset(rvp, 0, n * sizeof(real));

  for (int i = 0; i < n; ++i) {
    rvp[i] += adp[i] * bvp[i];

    int k0 = app[i + 1] - 1;
    int k1 = app[i];

    for (int k = k0, j = i - 1; k >= k1; --k, --j) {
      rvp[i] += alp[k] * bvp[j];
      rvp[j] += aup[k] * bvp[i];
    }
  }
}

void smtx_fput(FILE* f, struct smtx* mp) {
  int n = mp->n;
  int s = mp->s;

  real* mlp = mp->l;
  real* mup = mp->u;
  real* mdp = mp->d;
  int* mpp = mp->p;

  for (int i = 0; i <= n; ++i)
    fprintf(f, "%u ", mpp[i]);

  fputc('\n', f);

  for (int i = 0; i < n; ++i)
    fprintf(f, "%.7e ", mdp[i]);

  fputc('\n', f);

  for (int i = 0; i < s; ++i)
    fprintf(f, "%.7e ", mlp[i]);

  fputc('\n', f);

  for (int i = 0; i < s; ++i)
    fprintf(f, "%.7e ", mup[i]);
}

void smtx_ldu(struct smtx* mp) {
  int n = mp->n;

  int* mpp = mp->p;
  real* mdp = mp->d;
  real* mlp = mp->l;
  real* mup = mp->u;

  for (int i = 0; i < n; ++i) {
    int ic = mpp[i + 1] - mpp[i];
    preal dsum = mdp[i];

    for (int k = 1; k <= ic; ++k)
      dsum -= mlp[mpp[i] + ic - k] * mup[mpp[i] + ic - k] * mdp[i - k];

    mdp[i] = dsum;

    for (int j = i + 1; j < n; ++j) {
      int jc = mpp[j + 1] - mpp[j] - j + i;

      if (jc > -1) {
        preal lsum = mlp[mpp[j] + jc];
        preal usum = mup[mpp[j] + jc];

        for (int k = 1; k <= min(jc, ic); ++k) {
          lsum -= mlp[mpp[j] + jc - k] * mup[mpp[i] + ic - k] * mdp[i - k];
          usum -= mup[mpp[j] + jc - k] * mlp[mpp[i] + ic - k] * mdp[i - k];
        }

        mlp[mpp[j] + jc] = lsum / mdp[i];
        mup[mpp[j] + jc] = usum / mdp[i];
      }
    }
  }
}

void smtx_free(struct smtx* mp) {
  free(mp->d);
  free(mp->l);
  free(mp->u);
  free(mp->p);
  free(mp);
}
