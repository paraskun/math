#include <bas/mas.h>
#include <errno.h>
#include <stdx.h>
#include <stdlib.h>
#include <vec/dss.h>
#include <vec/mtx.h>
#include <vec/non.h>
#include <math.h>

struct rec {
  uint i;
  double v;
};

int rec_cmp_dsc(void* a, void* b) {
  struct rec* ra = (struct rec*)a;
  struct rec* rb = (struct rec*)b;

  if (fabs(ra->v) > fabs(rb->v))
    return 1;

  if (fabs(ra->v) < fabs(rb->v))
    return -1;

  return 0;
}

int non_new_slv(uint m, double (**f)(struct vec*), struct vec* x, struct non_pps pps) {
  if (!f || !x) {
    errno = EINVAL;
    return -1;
  }

  struct pcut* c;
  struct rec** r;

  if (cut_new(&c))
    return -1;

  if (cut_dev(c, m)) {
    cut_cls(&c);
    return -1;
  }

  cut_cmp(c, *rec_cmp_dsc);

  for (uint i = 0; i < m; ++i) {
    struct rec* r = malloc(sizeof(struct rec));

    r->i = i;
    r->v = -f[i](x);

    cut_set(c, i, r);
  }

  uint dim = x->dim;

  struct vec* fk;
  struct vec* dk;
  struct imtx* jk;

  double nrm;

  if (vec_new(&fk, dim))
    return -1;

  if (vec_new(&dk, dim)) {
    vec_cls(&fk);

    return -1;
  }

  if (mtx_new(&jk, (struct ipps){dim})) {
    vec_cls(&fk);
    vec_cls(&dk);

    return -1;
  }

  cut_srt(c);
  cut_pub(c, (void***)&r);

  for (uint i = 0; i < dim; ++i) {
    fk->data[i] = r[i]->v;

    for (uint j = 0; j < dim; ++j)
      pdif(f[r[i]->i], j, pps.hop, x, &jk->data[i][j]);
  }

  if (pps.res) {
    pps.res->k = 0;
    pps.res->x = x;
    pps.res->del = -1;

    vec_nrm(fk, &pps.res->err);
  }

  if (pps.cbk)
    pps.cbk(pps.res);

  for (uint k = 1; k <= pps.hem; ++k) {
    if (dss_red_slv(jk, dk, fk)) {
      vec_cls(&fk);
      vec_cls(&dk);
      mtx_cls(&jk);

      return -1;
    }

    vec_cmb(x, dk, x, 1);
    vec_nrm(dk, &nrm);

    for (uint i = 0; i < m; ++i) {
      r[i]->i = i;
      r[i]->v = -f[i](x);
    }

    cut_srt(c);

    for (uint i = 0; i < dim; ++i) {
      fk->data[i] = r[i]->v;

      for (uint j = 0; j < dim; ++j)
        pdif(f[r[i]->i], j, pps.hop, x, &jk->data[i][j]);
    }

    if (pps.res) {
      pps.res->k = k;
      pps.res->del = nrm;

      vec_nrm(fk, &pps.res->err);
    }

    if (pps.cbk)
      pps.cbk(pps.res);

    if (nrm < pps.eps)
      break;
  }

  vec_cls(&fk);
  vec_cls(&dk);
  mtx_cls(&jk);

  for (uint i = 0; i < m; ++i)
    free(r[i]);

  cut_cls(&c);

  return 0;
}
