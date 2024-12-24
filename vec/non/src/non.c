#include <bas/mas.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <stdx.h>
#include <vec/dss.h>
#include <vec/mtx.h>
#include <vec/non.h>

struct rec {
  uint   i;
  double v;
};

static int rec_cmp_dsc(void* a, void* b) {
  struct rec* ar = (struct rec*)a;
  struct rec* br = (struct rec*)b;

  if (fabs(ar->v) > fabs(br->v))
    return 1;

  if (fabs(ar->v) < fabs(br->v))
    return -1;

  return 0;
}

static int non_evo_exc(
  struct pcut* r, struct pcut* f, struct vec* x, struct vec* fk, struct imtx* jk,
  struct non_opt* opt) {
  uint m = cut_len(f);
  func l;

  struct rec** rp;

  if (m < x->dim) {
    errno = EINVAL;
    return -1;
  }

  cut_pub(r, (void***)&rp);

  for (uint i = 0; i < m; ++i) {
    cut_get(f, i, (void**)&l);

    rp[i]->i = i;
    rp[i]->v = l(x);
  }

  if (m > x->dim)
    cut_srt(r);

  for (uint i = 0; i < x->dim; ++i) {
    cut_get(f, rp[i]->i, (void**)&l);

    fk->data[i] = -rp[i]->v;

    if (opt->jac)
      for (uint j = 0; j < x->dim; ++j)
        jk->data[i][j] = opt->jac->data[rp[i]->i][j](x);
    else
      for (uint j = 0; j < x->dim; ++j)
        pdif(l, j, opt->hop, x, &jk->data[i][j]);
  }

  return 0;
}

static int non_evo_con(
  struct pcut* r, struct pcut* f, struct vec* x, struct vec* fk, struct imtx* jk,
  struct non_opt* opt) {
  uint m = cut_len(f);
  func l;

  struct rec** rp;

  if (m < x->dim) {
    errno = EINVAL;
    return -1;
  }

  cut_pub(r, (void***)&rp);

  for (uint i = 0; i < m; ++i) {
    cut_get(f, i, (void**)&l);

    rp[i]->i = i;
    rp[i]->v = l(x);
  }

  uint i1 = x->dim;

  if (m > x->dim) {
    cut_srt(r);
    i1 = x->dim - 1;
  }

  for (uint i = 0; i < i1; ++i) {
    cut_get(f, rp[i]->i, (void**)&l);

    fk->data[i] = -rp[i]->v;

    if (opt->jac)
      for (uint j = 0; j < x->dim; ++j)
        jk->data[i][j] = opt->jac->data[rp[i]->i][j](x);
    else
      for (uint j = 0; j < x->dim; ++j)
        pdif(l, j, opt->hop, x, &jk->data[i][j]);
  }

  if (m > x->dim) {
    fk->data[i1] = 0;

    for (uint j = 0; j < x->dim; ++j)
      jk->data[i1][j] = 0;

    for (uint i = i1; i < m; ++i) {
      cut_get(f, rp[i]->i, (void**)&l);

      fk->data[i1] -= rp[i]->v * rp[i]->v;

      if (opt->jac)
        for (uint j = 0; j < x->dim; ++j)
          jk->data[i1][j] += 2 * opt->jac->data[rp[i]->i][j](x);
      else {
        double pd = 0;

        for (uint j = 0; j < x->dim; ++j) {
          pdif(l, j, opt->hop, x, &pd);
          jk->data[i1][j] += 2 * pd;
        }
      }
    }
  }

  return 0;
}

int non_new_slv(struct pcut* f, struct vec* x, struct non_opt opt) {
  if (!f || !x) {
    errno = EINVAL;
    return -1;
  }

  struct pcut* r;

  cut_new(&r);
  cut_cmp(r, &rec_cmp_dsc);
  cut_exp(r, cut_len(f));

  for (uint i = 0; i < cut_len(f); ++i) {
    struct rec* rec = malloc(sizeof(struct rec));
    cut_set(r, i, rec);
  }

  struct vec*  fk;
  struct vec*  dk;
  struct imtx* jk;

  double nrm = 0;
  uint   dim = x->dim;

  if (vec_new(&fk, dim))
    return -1;

  if (vec_new(&dk, dim)) {
    vec_cls(&fk);

    return -1;
  }

  struct ipps p = {dim, dim};

  if (mtx_new(&jk, p)) {
    vec_cls(&fk);
    vec_cls(&dk);

    return -1;
  }

  switch (opt.mod) {
    case EXC: non_evo_exc(r, f, x, fk, jk, &opt); break;
    case CON: non_evo_con(r, f, x, fk, jk, &opt); break;
  }

  opt.itr->k = 0;
  opt.itr->x = x;
  opt.itr->del = -1;

  vec_nrm(fk, &opt.itr->err);

  if (opt.cbk)
    opt.cbk->call(opt.cbk->ctx);

  for (uint k = 1; k <= opt.hem; ++k) {
    if (dss_red_slv(jk, dk, fk)) {
      vec_cls(&fk);
      vec_cls(&dk);
      mtx_cls(&jk);

      errno = EDOM;
      return -1;
    }

    vec_cmb(x, dk, x, 1);
    vec_nrm(dk, &nrm);

    switch (opt.mod) {
      case EXC: non_evo_exc(r, f, x, fk, jk, &opt); break;
      case CON: non_evo_con(r, f, x, fk, jk, &opt); break;
    }

    if (opt.itr) {
      opt.itr->k = k;
      opt.itr->del = nrm;

      vec_nrm(fk, &opt.itr->err);
    }

    if (opt.cbk)
      opt.cbk->call(opt.cbk->ctx);

    if (nrm < opt.eps)
      break;
  }

  vec_cls(&fk);
  vec_cls(&dk);
  mtx_cls(&jk);
  cut_cls(&r);

  return 0;
}
