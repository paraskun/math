#include <fem/fem.h>
#include <iss/iss_csj.h>

#include <stdlib.h>
#include <string.h>

int vll_add(struct vll* l, int vtx) {
  struct vln* n = malloc(sizeof(struct vln));

  n->vtx = vtx;
  n->next = NULL;

  if (l->beg == NULL) {
    l->beg = n;
    return 0;
  }

  struct vln* e = l->beg;

  if (vtx < e->vtx) {
    n->next = e;
    l->beg = n;

    return 0;
  }

  while (e->next && e->next->vtx < vtx)
    e = e->next;

  n->next = e->next;
  e->next = n;

  return 0;
}

int vll_cls(struct vll* l) {
  struct vln* n = l->beg;

  while (n) {
    struct vln* t = n->next;
    free(n);
    n = t;
  }

  return 0;
}

struct fem* fem_new() {
  return malloc(sizeof(struct fem));
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

  int fs;

  fgets(buf, sizeof(buf), obj);
  sscanf(buf, "#face [%d]", &fs);

  for (int i = 0, h; i < fs; ++i) {
    fscanf(obj, "%d |", &h);

    struct fce* f = fce_new();

    fce_get(obj, f);
    fll_add(&fem->hex[h]->fll, f);
  }

  return 0;
}

int fem_evo(struct fem* fem) {
  struct mtx_csj_pps pps = {fem->vs, 0};
  struct vll* l = malloc(sizeof(struct vll) * fem->vs);

  for (int i = 0; i < fem->vs; ++i)
    l[i].beg = NULL;

  for (int i = 0; i < fem->hs; ++i) {
    struct hex* h = fem->hex[i];

    hex_evo(h, fem->vtx, &fem->fll);

    for (int j = 0; j < 8; ++j) {
      int a = h->vtx[i];

      for (int k = 0; k < 8; ++k) {
        int b = h->vtx[k];

        if (b < a) {
          vll_add(&l[a], h->vtx[k]);
          pps.ne += 1;
        }
      }
    }
  }

  fem->a = mtx_csj_new(pps);
  fem->b = vec_new(pps.n);

  for (int i = 0, e = 0; i < pps.n; ++i) {
    struct vln* n = l[i].beg;

    fem->a->ia[i] = e;

    while (n) {
      fem->a->ja[e] = n->vtx;

      e = e + 1;
      n = n->next;
    }
  }

  fem->a->ia[pps.n] = pps.ne;

  for (int i = 0; i < fem->vs; ++i)
    vll_cls(&l[i]);

  return 0;
}

int fem_asm(struct fem* fem) {
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

  return 0;
}
