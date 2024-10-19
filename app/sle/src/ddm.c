#include <mtx.h>
#include <sle.h>
#include <vec.h>

#include <stdio.h>
#include <stdlib.h>

#define N 10
#define K 100

int main() {
  srand(0);

#ifdef DMTX
  FILE* data = fopen("data/gauss-mtx_dns-ddm.data", "w+");
  FILE* mout = fopen("data/mtx_dns-ddm.out", "w+");
  struct mtx_dns* ap = mtx_dns_new(N);  // A^k
#elifdef SMTX
  FILE* data = fopen("data/ldu-mtx_pfl-ddm.data", "w+");
  FILE* mout = fopen("data/mtx_pfl-ddm.out", "w+");
  struct mtx_pfl* ap = mtx_pfl_new(N, N * (N - 1) / 2);
#endif

  struct vec* yp = vec_new(N);  // x*
  struct vec* xp = vec_new(N);  // x^k
  struct vec* fp = vec_new(N);  // f^k

  for (int k = 1; k <= K; ++k) {
    mtx_ddm(ap, k);  // ddm -> A^k

    if (k == K / 2)
      mtx_fput(mout, ap);

    vec_seq(yp);           // x* = (1, 2, ... , n)
    mtx_vmlt(ap, yp, fp);  // A^k * x* = f^k

#ifdef DMTX
    dsle_gauss(ap, xp, fp);  // A^k * x^k = f^k
#elifdef SMTX
    ssle_ldu(ap, xp, fp);  // A^k * x^k = f^k
#endif

    double xn;  // ||x^k||
    double dn;  // ||x* - x^k||

    for (int i = 0; i < N; ++i)
      fp->v[i] = yp->v[i] - xp->v[i];  // f^k = x* - x^k

    vec_nrm(xp, &xn);  // ||x^k||
    vec_nrm(fp, &dn);  // ||x* - x^k||

    fprintf(data, "%d\t%.4e\t%.4e\n", k, xn, dn);
  }

  fclose(data);
  fclose(mout);

  mtx_free(ap);
  vec_free(yp);
  vec_free(xp);
  vec_free(fp);

  return 0;
}
