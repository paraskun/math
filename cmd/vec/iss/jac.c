#include <stdio.h>

#include <iss_cds.h>

int main() {
  struct pps p;

  int n;
  int c;

  FILE* fpi = fopen("jac.pps", "r");
  FILE* fmi = fopen("jac-m.mtx", "r");
  FILE* fxo = fopen("jac-x.vec", "w+");
  FILE* ffo = fopen("jac-f.vec", "w+");

  fscanf(fpi, "%d %d", &n, &c);
  fscanf(fpi, "%lf %lf %d", &p.omg, &p.eps, &p.ms);

  struct mtx_cds* mp = mtx_cds_new(n, c);
  struct vec* xp = vec_new(n);
  struct vec* fp = vec_new(n);

  mtx_cds_fget(fmi, mp);
  vec_seq(xp, 1);
  mtx_cds_vmlt(mp, xp, fp);
  vec_fput(ffo, fp);

  for (int i = 0; i < xp->n; ++i)
    xp->vp[i] = 1;

  iss_cds_jac_solve(mp, xp, fp, p);

  vec_fput(fxo, xp);

  fclose(fpi);
  fclose(fmi);
  fclose(fxo);
  fclose(ffo);

  return 0;
}
