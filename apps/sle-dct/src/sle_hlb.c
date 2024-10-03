#include <mtx.h>
#include <sle.h>
#include <vec.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define K 10

int main() {
  srand(time(NULL));

#ifdef DMTX
  FILE* out = fopen("gauss-hlb.data", "w+");
#elifdef SMTX
  FILE* out = fopen("ldu-hlb.data", "w+");
#endif

  for (int k = 3; k <= K; ++k) {
#ifdef DMTX
    struct dmtx* ap = dmtx_new(k);
#elifdef SMTX
    struct smtx* ap = smtx_new(k, k * (k - 1) / 2);
#endif

    struct vec* xxp = vec_new(k);
    struct vec* xp = vec_new(k);
    struct vec* fp = vec_new(k);

    mtx_hlb(ap);

    vec_seq(xxp);
    mtx_vmlt(ap, xxp, fp);

#ifdef DMTX
    dsle_gauss(ap, xp, fp);
#elifdef SMTX
    ssle_ldu(ap, xp, fp);
#endif

    fprintf(out, "%d\t", k);

    vec_fput(out, xp);
    fputc('\t', out);

    for (int i = 0; i < k; ++i)
      fprintf(out, "%.3e ", xxp->v[i] - xp->v[i]);

    fputc('\n', out);

    mtx_free(ap);
    vec_free(xxp);
    vec_free(xp);
    vec_free(fp);
  }

  fclose(out);

  return 0;
}
