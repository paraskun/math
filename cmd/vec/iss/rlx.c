#include <stdio.h>

#include <iss_cds.h>

static int itr;
static double res;

void f(int i, double r) {
  printf("\rIteration: %d; Residual: %lf", i, r);

  itr = i;
  res = r;
}

int main(int argc, char* argv[argc]) {
  struct iss_jac_pps pps;

  int n;
  int c;

  FILE* fpi = fopen("rlx.pps", "r");
  FILE* fmi = fopen(argv[1], "r");

  fscanf(fpi, "%d %d", &n, &c);
  fscanf(fpi, "%lf %lf %d", &pps.omg, &pps.pps.eps, &pps.pps.mk);

  struct mtx_cds* mp = mtx_cds_new(n, c);
  struct vec* xp = vec_new(n);
  struct vec* xt = vec_new(n);
  struct vec* fp = vec_new(n);

  vec_seq(xt, 1);
  mtx_cds_fget(fmi, mp);
  mtx_cds_vmlt(mp, xt, fp);

  pps.omg = 0.00;

  for (int s = 0; s < 300; ++s) {
    pps.omg += 0.01;

    for (int i = 0; i < xp->n; ++i)
      xp->vp[i] = 0;

    iss_cds_rlx_solve(mp, xp, fp, pps, &f);

    putchar('\n');

    double nxp;
    double ntp;
    double nd;

    vec_nrm(xp, &nxp);
    vec_sub(xt, xp, xp);
    vec_nrm(xp, &nd);
    vec_nrm(xt, &ntp);

    printf("%.2lf\t%.7e\t%.7e\t%.7e\t%.7e\t%d\n", pps.omg, nxp, ntp, nd, res,
           itr + 1);
  }

  fclose(fpi);
  fclose(fmi);

  return 0;
}
