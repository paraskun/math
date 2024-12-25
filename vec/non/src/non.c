#include <calc.h>
#include <errno.h>
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdx.h>
#include <vec/dss.h>
#include <vec/mtx.h>
#include <vec/non.h>

struct rec {
  uint   i;
  double v;
};

static int rec_cmp_dsc(void*, uint n, ...) {
  if (n < 2)
    return 0;

  va_list arg;
  va_start(arg, n);

  struct rec* ar = (struct rec*)va_arg(arg, void*);
  struct rec* br = (struct rec*)va_arg(arg, void*);

  va_end(arg);

  if (fabs(ar->v) > fabs(br->v))
    return 1;

  if (fabs(ar->v) < fabs(br->v))
    return -1;

  return 0;
}

static int non_evo_exc(
  struct pcut* rec, struct pcut* fun, struct vec* x, struct vec* fk, struct imtx* jk, struct non_opt* opt) {
  if (fun->len < x->dim) {
    errno = EINVAL;
    return -1;
  }

  struct rec** rp = (struct rec**)rec->dat;
  mfun*        fp = (mfun*)fun->dat;

  for (uint i = 0; i < fun->len; ++i) {
    rp[i]->i = i;
    rp[i]->v = fp[i](x);
  }

  if (fun->len > x->dim)
    cut_srt(rec);

  for (uint i = 0; i < x->dim; ++i) {
    fk->data[i] = -rp[i]->v;

    if (opt->jac)
      for (uint j = 0; j < x->dim; ++j)
        jk->data[i][j] = opt->jac->data[rp[i]->i][j](x);
    else
      for (uint j = 0; j < x->dim; ++j)
        pdif(fp[rp[i]->i], j, opt->hop, x, &jk->data[i][j]);
  }

  return 0;
}

static int non_evo_con(
  struct pcut* rec, struct pcut* fun, struct vec* x, struct vec* fk, struct imtx* jk, struct non_opt* opt) {
  uint l = x->dim - 1;

  if (non_evo_exc(rec, fun, x, fk, jk, opt))
    return -1;

  struct rec** rp = (struct rec**)rec->dat;
  mfun*        fp = (mfun*)fun->dat;

  if (fun->len > x->dim) {
    fk->data[l] = 0;

    for (uint j = 0; j < x->dim; ++j)
      jk->data[l][j] = 0;

    for (uint i = l; i < fun->len; ++i) {
      fk->data[l] -= rp[i]->v * rp[i]->v;

      if (opt->jac)
        for (uint j = 0; j < x->dim; ++j)
          jk->data[l][j] += 2 * opt->jac->data[rp[i]->i][j](x);
      else {
        double pd = 0;

        for (uint j = 0; j < x->dim; ++j) {
          pdif(fp[rp[i]->i], j, opt->hop, x, &pd);
          jk->data[l][j] += 2 * pd;
        }
      }
    }
  }

  return 0;
}

int non_new_slv(struct pcut* fun, struct vec* x, struct non_opt opt) {
  if (!fun || !x || fun->len < x->dim) {
    errno = EINVAL;
    return -1;
  }

  struct pcut* rec;

  cut_new(&rec);
  cut_exp(rec, fun->len);

  rec->ctl = true;
  rec->cmp.call = &rec_cmp_dsc;

  for (uint i = 0; i < fun->len; ++i) {
    struct rec* r = malloc(sizeof(struct rec));

    if (!rec) {
      cut_cls(&rec);

      errno = ENOMEM;
      return -1;
    }

    cut_add(rec, r);
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
    case EXC: non_evo_exc(rec, fun, x, fk, jk, &opt); break;
    case CON: non_evo_con(rec, fun, x, fk, jk, &opt); break;
  }

  if (opt.itr) {
    opt.itr->k = 0;
    opt.itr->x = x;
    opt.itr->del = -1;

    vec_nrm(fk, &opt.itr->err);
  }

  if (opt.cbk)
    opt.cbk->call(opt.cbk->ctx, 1, opt.itr);

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
      case EXC: non_evo_exc(rec, fun, x, fk, jk, &opt); break;
      case CON: non_evo_con(rec, fun, x, fk, jk, &opt); break;
    }

    if (opt.itr) {
      opt.itr->k = k;
      opt.itr->del = nrm;

      vec_nrm(fk, &opt.itr->err);
    }

    if (opt.cbk)
      opt.cbk->call(opt.cbk->ctx, 1, opt.itr);

    if (nrm < opt.eps)
      break;
  }

  cut_cls(&rec);

  vec_cls(&fk);
  vec_cls(&dk);
  mtx_cls(&jk);

  return 0;
}
