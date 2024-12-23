#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <vec/dss.h>

int idss_red_slv(struct imtx* m, struct vec* x, struct vec* f) {
  if (
    !m || !x || !f || m->pps.n != m->pps.m || m->pps.n != x->dim ||
    m->pps.n != f->dim) {
    errno = EINVAL;
    return -1;
  }

  uint dim = m->pps.n;
  uint* pos = malloc(sizeof(uint) * dim);

  double** md = m->data;
  double* xd = x->data;
  double* fd = f->data;

  for (uint i = 0; i < dim; ++i)
    pos[i] = i;

  for (uint i = 0; i < dim; ++i) {
    double mv = fabs(md[pos[i]][i]);
    uint mi = i;

    for (uint j = i + 1; j < dim; ++j) {
      double mij = fabs(md[pos[j]][i]);

      if (mij > mv) {
        mv = mij;
        mi = j;
      }
    }

    if (i != mi) {
      uint t = pos[i];
      pos[i] = pos[mi];
      pos[mi] = t;
    }

    for (uint j = i + 1; j < dim; ++j) {
      if (md[pos[i]][i] < 1e-100) {
        free(pos);

        errno = EINVAL;
        return -1;
      }

      double k = md[pos[j]][i] / md[pos[i]][i];

      for (uint c = i + 1; c < dim; ++c)
        md[pos[j]][c] -= md[pos[i]][c] * k;

      fd[pos[j]] -= fd[pos[i]] * k;
    }
  }

  for (uint h = 0, i = dim - 1; h < dim; ++h, --i) {
    double sum = fd[pos[i]];

    for (uint j = i + 1; j < dim; ++j)
      sum -= xd[j] * md[pos[i]][j];

    if (md[pos[i]][i] < 1e-100) {
      free(pos);

      errno = EINVAL;
      return -1;
    }

    xd[i] = sum / md[pos[i]][i];
  }

  free(pos);

  return 0;
}

// static void dss_sky_l(
//     struct mtx_sky* mp,
//     struct vec* xp,
//     struct vec* fp) {
//   int n = mp->pps.n;
//   int* pp = mp->p;
//
//   double* lp = mp->lv;
//   double* xvp = xp->vp;
//   double* fvp = fp->vp;
//
//   for (int i = 0; i < n; ++i) {
//     double sum = 0;
//
//     int k0 = pp[i];
//     int k1 = pp[i + 1];
//     int j0 = i - k1 + k0;
//
//     for (int k = k0, j = j0; k < k1; ++k, ++j)
//       sum += lp[k] * xvp[j];
//
//     xvp[i] = fvp[i] - sum;
//   }
// }
//
// static void dss_sky_d(
//     struct mtx_sky* mp,
//     struct vec* xp,
//     struct vec* fp) {
//   int n = mp->pps.n;
//
//   double* dvp = mp->dv;
//   double* xvp = xp->vp;
//   double* fvp = fp->vp;
//
//   for (int i = 0; i < n; ++i) {
//     double e = fvp[i] / dvp[i];
//     xvp[i] = e;
//   }
// }
//
// static void dss_sky_u(
//     struct mtx_sky* mp,
//     struct vec* xp,
//     struct vec* fp) {
//   int n = mp->pps.n;
//   int* pp = mp->p;
//
//   double* up = mp->uv;
//   double* xvp = xp->vp;
//   double* fvp = fp->vp;
//
//   memcpy(xvp, fvp, sizeof(double) * n);
//
//   for (int j = n - 1; j > 0; --j) {
//     int k0 = pp[j];
//     int k1 = pp[j + 1];
//     int i0 = j - k1 + k0;
//
//     for (int k = k0, i = i0; k < k1; ++k, ++i)
//       xvp[i] -= up[k] * xvp[j];
//   }
// }
//
// void dss_sky_ldu_solve(
//     struct mtx_sky* mp,
//     struct vec* xp,
//     struct vec* fp) {
//   dss_sky_l(mp, xp, fp);
//   dss_sky_d(mp, fp, xp);
//   dss_sky_u(mp, xp, fp);
// }
