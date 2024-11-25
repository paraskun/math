#include <fem/fem.h>

#include <stdlib.h>

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
  for (int i = 0; i < fem->hs; ++i)
    hex_evo(fem->hex[i], fem->vtx, &fem->fll);

  return 0;
}

int fem_asm(struct fem* fem) {
  return 0;
}

int fem_slv(struct fem* fem, struct vec* q) {
  return 0;
}

int fem_cls(struct fem* fem) {
  return 0;
}


