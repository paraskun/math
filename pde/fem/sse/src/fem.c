#include <fem/sse/const.h>
#include <fem/sse/fem.h>
#include <vec/iss_csj.h>

#include <stdlib.h>
#include <string.h>

int cmp(int a, int b) {
  return a > b ? -1 : a == b ? 0 : 1;
}

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

  fem->fun = fun;

  return fem;
}

int fem_get(FILE* obj, struct fem* fem) {
  char buf[0xff];

  fgets(buf, sizeof(buf), obj);
  sscanf(buf, "#vertex [%d]", &fem->vs);

  fem->vtx = malloc(sizeof(struct vtx*) * fem->vs);

  for (int i = 0; i < fem->vs; ++i) {
    fem->vtx[i] = vtx_new();
    vtx_get(obj, fem->vtx[i]);
  }

  fgets(buf, sizeof(buf), obj);
  sscanf(buf, "#hexahedron [%d]", &fem->hs);

  fem->hex = malloc(sizeof(struct hex*) * fem->hs);

  for (int i = 0; i < fem->hs; ++i) {
    fem->hex[i] = hex_new();
    hex_get(obj, fem->hex[i]);
  }

  fgets(buf, sizeof(buf), obj);
  sscanf(buf, "#face [%d]", &fem->fs);

  for (int i = 0; i < fem->fs; ++i) {
    struct fce* f = fce_new();
    fce_get(obj, f);
  }

  return 0;
}

int fem_evo(struct fem* fem) {
  struct mtx_csj_pps pps = {fem->vs, 0};
  struct sll* l = malloc(sizeof(struct sll) * fem->vs);

  for (int i = 0; i < fem->vs; ++i) {
    l[i].beg = NULL;
    l[i].end = NULL;
    l[i].cmp = &cmp;
  }

  for (int i = 0; i < fem->hs; ++i) {
    struct hex* h = fem->hex[i];

    hex_evo(h, fem->vtx);

    for (int j = 0; j < 8; ++j) {
      int a = h->vtx[i];

      for (int k = 0; k < 8; ++k) {
        int b = h->vtx[k];

        if (b < a) {
          sll_ins(&l[a], b);
          pps.ne += 1;
        }
      }
    }
  }

  for (int i = 0; i < fem->fs; ++i)
    fce_evo(fem->fce[i], fem->vtx);

  fem->a = mtx_csj_new(pps);
  fem->b = vec_new(pps.n);

  for (int i = 0, e = 0; i < pps.n; ++i) {
    struct lln* n = l[i].beg;

    fem->a->ia[i] = e;

    while (n) {
      fem->a->ja[e] = n->e;

      e = e + 1;
      n = n->next;
    }
  }

  fem->a->ia[pps.n] = pps.ne;

  for (int i = 0; i < fem->vs; ++i)
    sll_cls(&l[i]);

  free(l);

  return 0;
}

int fem_asm(struct fem* fem) {
  struct sll dfl;

  dfl.beg = NULL;
  dfl.end = NULL;
  dfl.cmp = NULL;

  for (int i = 0; i < fem->hs; ++i)
    hex_mov(fem->hex[i], fem->a, fem->b);

  for (int i = 0; i < fem->fs; ++i) {
    struct fce* f = fem->fce[i];

    switch (f->cnd.type) {
      case DIR:
        sll_ins(&dfl, i);
        continue;
      case NEU:
      case ROB:
        fce_mov(f, fem->a, fem->b);
        continue;
    }
  }

  struct lln* n = dfl.beg;

  while (n) {
    struct fce* f = fem->fce[n->e];

    for (int i = 0; i < 4; ++i) {
      struct vtx* v = fem->vtx[f->vtx[i]];

      fem->a->dr[f->vtx[i]] = C;
      fem->b->vp[f->vtx[i]] = C * f->cnd.pps.dir.tmp(v);
    }

    n = n->next;
  }

  sll_cls(&dfl);

  return 0;
}

int fem_slv(struct fem* fem, struct vec* q) {
  struct iss_pps pps = {1e-5, 10000};
  struct iss_res res = {0, 0};

  iss_csj_los_slv(fem->a, q, fem->b, &pps, &res, NULL);

  return 0;
}

int fem_cls(struct fem* fem) {
  mtx_csj_cls(fem->a);
  vec_cls(fem->b);

  for (int i = 0; i < fem->vs; ++i)
    vtx_cls(fem->vtx[i]);

  for (int i = 0; i < fem->hs; ++i)
    hex_cls(fem->hex[i]);

  for (int i = 0; i < fem->fs; ++i)
    fce_cls(fem->fce[i]);

  return 0;
}
