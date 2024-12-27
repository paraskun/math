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
  uint i;
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
  struct pcut* rec, struct pcut* fun, struct vec* xk, struct vec* fk, struct imtx* jk,
  struct non_opt* opt) {
  if (fun->len < xk->dim) {
    errno = EINVAL;
    return -1;
  }

  struct rec** rp = (struct rec**)rec->dat;
  mfun* fp = (mfun*)fun->dat;

  for (uint i = 0; i < rec->len; ++i) {
    rp[i]->i = i;
    rp[i]->v = fp[i](xk);
  }

  if (rec->len > xk->dim)
    if (cut_srt(rec))
      return -1;

  for (uint i = 0; i < xk->dim; ++i) {
    fk->data[i] = -rp[i]->v;

    if (opt->jac)
      for (uint j = 0; j < xk->dim; ++j)
        jk->data[i][j] = opt->jac->data[rp[i]->i][j](xk);
    else
      for (uint j = 0; j < xk->dim; ++j)
        if (pdif(fp[rp[i]->i], j, opt->hop, xk, &jk->data[i][j]))
          return -1;
  }

  return 0;
}

static int non_evo_con(
  struct pcut* rec, struct pcut* fun, struct vec* xk, struct vec* fk, struct imtx* jk,
  struct non_opt* opt) {
  if (non_evo_exc(rec, fun, xk, fk, jk, opt))
    return -1;

  if (rec->len == xk->dim)
    return 0;

  struct rec** rp = (struct rec**)rec->dat;
  mfun* fp = (mfun*)fun->dat;
  uint l = xk->dim - 1;

  if (rec->len > xk->dim) {
    fk->data[l] = 0;

    for (uint j = 0; j < xk->dim; ++j)
      jk->data[l][j] = 0;

    for (uint i = l; i < rec->len; ++i) {
      fk->data[l] -= rp[i]->v * rp[i]->v;

      if (opt->jac)
        for (uint j = 0; j < xk->dim; ++j)
          jk->data[l][j] += 2 * rp[i]->v * opt->jac->data[rp[i]->i][j](xk);
      else {
        double pd = 0;

        for (uint j = 0; j < xk->dim; ++j) {
          if (pdif(fp[rp[i]->i], j, opt->hop, xk, &pd))
            return -1;

          jk->data[l][j] += 2 * rp[i]->v * pd;
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

  struct pcut* rec = nullptr;

  if (cut_new(&rec))
    goto err;

  if (cut_exp(rec, fun->len))
    goto err;

  rec->ctl = true;
  rec->cmp.call = &rec_cmp_dsc;

  for (uint i = 0; i < fun->len; ++i) {
    struct rec* r = malloc(sizeof(struct rec));

    if (!r) {
      errno = ENOMEM;
      goto err;
    }

    if (cut_add(rec, r))
      goto err;
  }

  struct vec* fk = nullptr;
  struct vec* dk = nullptr;
  struct imtx* jk = nullptr;

  double nrm = 0;
  uint dim = x->dim;

  if (vec_new(&fk, dim))
    goto err;

  if (vec_new(&dk, dim))
    goto err;

  struct ipps p = {dim, dim};

  if (mtx_new(&jk, p))
    goto err;

  switch (opt.mod) {
    case EXC:
      if (non_evo_exc(rec, fun, x, fk, jk, &opt))
        goto err;

      break;
    case CON:
      if (non_evo_con(rec, fun, x, fk, jk, &opt))
        goto err;

      break;
  }

  if (opt.itr) {
    opt.itr->k = 0;
    opt.itr->x = x;
    opt.itr->del = -1;

    if (vec_nrm(fk, &opt.itr->err))
      goto err;
  }

  if (opt.cbk)
    opt.cbk->call(opt.cbk->ctx, 1, opt.itr);

  for (uint k = 1; k <= opt.hem; ++k) {
    if (dss_red_slv(jk, dk, fk))
      goto err;

    if (vec_cmb(x, dk, x, 1))
      goto err;

    if (vec_nrm(dk, &nrm))
      goto err;

    switch (opt.mod) {
      case EXC:
        if (non_evo_exc(rec, fun, x, fk, jk, &opt))
          goto err;

        break;
      case CON:
        if (non_evo_con(rec, fun, x, fk, jk, &opt))
          goto err;

        break;
    }

    if (opt.itr) {
      opt.itr->k = k;
      opt.itr->del = nrm;

      if (vec_nrm(fk, &opt.itr->err))
        goto err;
    }

    if (opt.cbk)
      opt.cbk->call(opt.cbk->ctx, 1, opt.itr);

    if (nrm < opt.eps)
      break;
  }

  int r = 0;

  goto end;

err:
  r = -1;

end:
  cut_cls(&rec);
  vec_cls(&fk);
  vec_cls(&dk);
  mtx_cls(&jk);

  return r;
}
