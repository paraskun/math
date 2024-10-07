#include <mtx.h>
#include <sle.h>
#include <vec.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 10
#define K 100

int main() {
  srand(0);

#ifdef DMTX
  FILE* out = fopen("gauss-ddm.data", "w+");
  struct dmtx* ap = dmtx_new(N);
#elifdef SMTX
  FILE* out = fopen("ldu-ddm.data", "w+");
  struct smtx* ap = smtx_new(N, N * (N - 1) / 2);
#endif

  struct vec* xxp = vec_new(N);
  struct vec* xp = vec_new(N);
  struct vec* fp = vec_new(N);

  for (int k = 1; k <= K; ++k) {
    mtx_ddm(ap, k);

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

    for (int i = 0; i < N; ++i)
      fprintf(out, "%.3e ", xxp->v[i] - xp->v[i]);

    fputc('\n', out);
  }

  fclose(out);

  mtx_free(ap);
  vec_free(xxp);
  vec_free(xp);
  vec_free(fp);

  return 0;
}
