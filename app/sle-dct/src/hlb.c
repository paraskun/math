#include <mtx.h>
#include <sle.h>
#include <vec.h>

#include <stdio.h>
#include <stdlib.h>

#define K 100

int main() {
  srand(0);

#ifdef DMTX
  FILE* data = fopen("data/gauss-dmtx-hlb.data", "w+");
  FILE* mout = fopen("data/dmtx-hlb.out", "w+");
#elifdef SMTX
  FILE* data = fopen("data/ldu-smtx-hlb.data", "w+");
  FILE* mout = fopen("data/smtx-hlb.out", "w+");
#endif

  for (int k = 3; k <= K; ++k) {
#ifdef DMTX
    struct dmtx* ap = dmtx_new(k);
#elifdef SMTX
    struct smtx* ap = smtx_new(k, k * (k - 1) / 2);
#endif

    struct vec* yp = vec_new(k);  // x*
    struct vec* xp = vec_new(k);  // x^k
    struct vec* fp = vec_new(k);  // f^k

    mtx_hlb(ap);  // Hilbert -> A^k
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
                //
    for (int i = 0; i < k; ++i)
      fp->v[i] = yp->v[i] - xp->v[i];  // f^k = x* - x^k

    vec_nrm(xp, &xn);  // ||x^k||
    vec_nrm(fp, &dn);  // ||x* - x^k||

    fprintf(data, "%d\t%.4e\t%.4e\n", k, xn, dn);

    mtx_free(ap);
    vec_free(yp);
    vec_free(xp);
    vec_free(fp);
  }

  fclose(data);
  fclose(mout);

  return 0;
}
