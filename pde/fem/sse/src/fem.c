#include <fem/sse/const.h>
#include <fem/sse/fem.h>
#include <ull.h>
#include <vec/iss_csj.h>

#include <stdlib.h>
#include <string.h>

const double G[2][2] = {{1.0, -1.0}, {-1.0, 1.0}};
const double M[2][2] = {{2.0 / 6.0, 1.0 / 6.0}, {1.0 / 6.0, 2.0 / 6.0}};
const double X[2][2] = {{-0.5, 0.5}, {-0.5, 0.5}};

const int MU[8] = {0, 1, 0, 1, 0, 1, 0, 1};
const int NU[8] = {0, 0, 1, 1, 0, 0, 1, 1};
const int TT[8] = {0, 0, 0, 0, 1, 1, 1, 1};

const double C = 10e100;

struct fem* fem_new(double (**fun)(struct vtx*)) {
  struct fem* fem = malloc(sizeof(struct fem));

  fem->vs = 0;
  fem->hs = 0;
  fem->fs = 0;

  fem->a = NULL;
  fem->b = NULL;

  fem->vtx = NULL;
  fem->hex = NULL;
  fem->fce = NULL;

  fem->pps.fun = fun;

  return fem;
}

int fem_get(FILE* obj, struct fem* fem) {
  char buf[0xff];

  fgets(buf, sizeof(buf), obj);
  sscanf(buf, "#vertex [%d]", &fem->vs);

  fem->vtx = malloc(sizeof(struct vtx*) * fem->vs);

  for (int i = 0; i < fem->vs; ++i) {
    fgets(buf, sizeof(buf), obj);
    fem->vtx[i] = vtx_get(buf);
  }

  fgets(buf, sizeof(buf), obj);
  fgets(buf, sizeof(buf), obj);
  sscanf(buf, "#hexahedron [%d]", &fem->hs);

  fem->hex = malloc(sizeof(struct hex*) * fem->hs);

  for (int i = 0; i < fem->hs; ++i) {
    fgets(buf, sizeof(buf), obj);
    fem->hex[i] = hex_get(buf, fem->pps.fun);
  }

  fgets(buf, sizeof(buf), obj);
  fgets(buf, sizeof(buf), obj);
  sscanf(buf, "#face [%d]", &fem->fs);

  fem->fce = malloc(sizeof(struct fce*) * fem->fs);

  for (int i = 0; i < fem->fs; ++i) {
    fgets(buf, sizeof(buf), obj);
    fem->fce[i] = fce_get(buf, fem->pps.fun);
  }

  return 0;
}

int fem_evo(struct fem* fem) {
  int ne = 0;
  struct ull* l = malloc(sizeof(struct ull) * fem->vs);

  for (int i = 0; i < fem->vs; ++i) {
    l[i].beg = NULL;
    l[i].end = NULL;
  }

  for (int i = 0; i < fem->hs; ++i) {
    struct hex* h = fem->hex[i];

    hex_evo(h, fem->vtx);

    for (int j = 0; j < 8; ++j) {
      int a = h->vtx[j];

      for (int k = 0; k < 8; ++k) {
        int b = h->vtx[k];

        if (b < a) {
          ull_ins(&l[a], b);
          ne += 1;
        }
      }
    }
  }

  for (int i = 0; i < fem->fs; ++i)
    fce_evo(fem->fce[i], fem->vtx);

  fem->a = mtx_csj_new(fem->vs, ne);
  fem->b = vec_new(fem->vs);

  for (int i = 0, e = 0; i < fem->vs; ++i) {
    struct lln* n = l[i].beg;

    fem->a->ia[i] = e;

    while (n) {
      fem->a->ja[e] = n->e;

      e = e + 1;
      n = n->next;
    }
  }

  fem->a->ia[fem->vs] = ne;

  for (int i = 0; i < fem->vs; ++i)
    ull_cls(&l[i]);

  free(l);

  return 0;
}

int fem_asm(struct fem* fem) {
  struct ull dir;

  dir.beg = NULL;
  dir.end = NULL;

  for (int i = 0; i < fem->hs; ++i)
    hex_mov(fem->hex[i], fem->a, fem->b);

  for (int i = 0; i < fem->fs; ++i) {
    struct fce* f = fem->fce[i];

    switch (f->cnd.type) {
      case DIR:
        ull_ins(&dir, i);
        continue;
      case NEU:
      case ROB:
        fce_mov(f, fem->a, fem->b);
        continue;
    }
  }

  struct lln* n = dir.beg;

  while (n) {
    struct fce* f = fem->fce[n->e];

    for (int i = 0; i < 4; ++i) {
      struct vtx* v = fem->vtx[f->vtx[i]];

      fem->a->dr[f->vtx[i]] = C;
      fem->b->vp[f->vtx[i]] = C * f->cnd.pps.dir.tmp(v);
    }

    n = n->next;
  }

  ull_cls(&dir);

  return 0;
}

int fem_slv(struct fem* fem, struct vec* q) {
  struct iss_pps pps = {1e-5, 10000};
  struct iss_res res = {0, 0};

  iss_csj_bcg_slv(fem->a, q, fem->b, &pps, &res, NULL);

  return 0;
}

int fem_cls(struct fem* fem) {
  if (fem->a)
    mtx_csj_cls(fem->a);

  if (fem->b)
    vec_cls(fem->b);

  for (int i = 0; i < fem->vs; ++i)
    vtx_cls(fem->vtx[i]);

  for (int i = 0; i < fem->hs; ++i)
    hex_cls(fem->hex[i]);

  for (int i = 0; i < fem->fs; ++i)
    fce_cls(fem->fce[i]);

  free(fem->vtx);
  free(fem->hex);
  free(fem->fce);

  free(fem);

  return 0;
}
