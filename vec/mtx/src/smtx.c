#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <numx/vec/mtx.h>

int smtx_new(struct smtx* m, struct smtx_pps pps) {
  if (!m || pps.n == 0) {
    errno = EINVAL;
    return -1;
  }

  m->pps = pps;

  m->dr = nullptr;
  m->ia = nullptr;

  m->dr = malloc(sizeof(double) * pps.n);
  m->ia = malloc(sizeof(uint) * (pps.n + 1));

  if (!m->dr || !m->ia) {
    free(m->dr);
    free(m->ia);

    errno = ENOMEM;
    return -1;
  }

  memset(m->dr, 0, sizeof(double) * pps.n);
  memset(m->ia, 0, sizeof(uint) * (pps.n + 1));

  m->lr = nullptr;
  m->ur = nullptr;
  m->ja = nullptr;

  if (pps.z > 0) {
    m->lr = malloc(sizeof(double) * pps.z);
    m->ur = malloc(sizeof(double) * pps.z);
    m->ja = malloc(sizeof(uint) * pps.z);

    if (!m->lr || !m->ur || !m->ja) {
      free(m->dr);
      free(m->ia);
      free(m->lr);
      free(m->ur);
      free(m->ja);

      errno = ENOMEM;
      return -1;
    }

    memset(m->lr, 0, sizeof(double) * pps.z);
    memset(m->ur, 0, sizeof(double) * pps.z);
    memset(m->ja, 0, sizeof(uint) * pps.z);
  }

  return 0;
}

int smtx_cls(struct smtx* m) {
  if (!m) {
    errno = EINVAL;
    return -1;
  }

  free(m->dr);
  free(m->lr);
  free(m->ur);
  free(m->ia);
  free(m->ja);

  return 0;
}

int smtx_ilu(struct smtx* m, struct smtx* r) {
  if (!m || !r || m->pps.n != r->pps.n || m->pps.z != r->pps.z) {
    errno = EINVAL;
    return -1;
  }

  uint n = m->pps.n;
  uint z = m->pps.z;

  uint* mia = m->ia;
  uint* mja = m->ja;

  uint* ria = r->ia;
  uint* rja = r->ja;

  memcpy(ria, mia, sizeof(uint) * (n + 1));
  memcpy(rja, mja, sizeof(uint) * z);

  double* mdr = m->dr;
  double* mlr = m->lr;
  double* mur = m->ur;

  double* rdr = r->dr;
  double* rlr = r->lr;
  double* rur = r->ur;

  for (uint d = 0; d < n; ++d) {
    uint lr0 = ria[d];
    uint lr1 = ria[d + 1];

    double sd = 0;

    for (uint lr = lr0; lr < lr1; ++lr) {
      uint j = rja[lr];

      uint ur0 = ria[j];
      uint ur1 = ria[j + 1];

      double sl = 0;
      double su = 0;

      for (uint lrr = lr0, urr = ur0; urr < ur1 && lrr < lr;) {
        uint lj = rja[lrr];
        uint ui = rja[urr];

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

int smtx_dgl(struct smtx* m, struct smtx* r) {
  if (!m || !r || m->pps.n != r->pps.n) {
    errno = EINVAL;
    return -1;
  }

  uint n = m->pps.n;

  double* mdr = m->dr;
  double* rdr = r->dr;

  for (uint i = 0; i < n; ++i)
    rdr[i] = sqrt(mdr[i]);

  return 0;
}

int smtx_vmlt(struct smtx* m, struct vec* x, struct vec* f) {
  if (!m || !x || !f || m->pps.n != x->n || m->pps.n != f->n) {
    errno = EINVAL;
    return -1;
  }

  uint n = m->pps.n;

  uint* ia = m->ia;
  uint* ja = m->ja;

  double* dr = m->dr;
  double* lr = m->lr;
  double* ur = m->ur;

  double* xv = x->dat;
  double* fv = f->dat;

  for (uint i = 0; i < n; ++i) {
    fv[i] = xv[i] * dr[i];

    uint ar0 = ia[i];
    uint ar1 = ia[i + 1];

    for (uint ar = ar0; ar < ar1; ++ar) {
      uint j = ja[ar];

      fv[i] += xv[j] * lr[ar];
      fv[j] += xv[i] * ur[ar];
    }
  }

  return 0;
}
