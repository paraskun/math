#include <mtx_csr.h>

#include <stdlib.h>

struct mtx_csr* mtx_csr_new(int n, int ne) {
  struct mtx_csr* mp = malloc(sizeof(struct mtx_csr));

  mp->n = n;
  mp->ne = ne;

  mp->ar = malloc(sizeof(double) * ne);
  mp->ia = malloc(sizeof(int) * (n + 1));
  mp->ja = malloc(sizeof(int) * ne);

  return mp;
}

int mtx_csr_ilu(struct mtx_csr* mp);

int mtx_csr_vmlt(struct mtx_csr* mp, struct vec* xp, struct vec* fp);

void mtx_csr_free(struct mtx_csr* mp) {
  free(mp->ar);
  free(mp->ia);
  free(mp->ia);
  free(mp);
}
