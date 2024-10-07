#include <mtx.h>
#include <sle.h>

#include <stdio.h>

int main() {
  FILE* fdms = fopen("data/dms.in", "r");

#ifdef SMTX
  FILE* fmtx = fopen("smtx.in", "r");
  FILE* fmo = fopen("smtx.out", "w+");
#elifdef DMTX
  FILE* fmtx = fopen("data/dmtx.in", "r");
  FILE* fmo = fopen("data/dmtx.out", "w+");
#endif

  FILE* fb = fopen("data/b.in", "r");
  FILE* fx = fopen("data/x.out", "w+");

#ifdef SMTX
  int n, s;
  fscanf(fdms, "%d %d", &n, &s);
  struct smtx* mp = smtx_new(n, s);
#elifdef DMTX
  int n;
  fscanf(fdms, "%d", &n);
  struct dmtx* mp = dmtx_new(n);
#endif

  struct vec* bp = vec_new(n);
  struct vec* xp = vec_new(n);

  mtx_fget(fmtx, mp);
  vec_fget(fb, bp);

#ifdef DMTX
  dsle_gauss(mp, xp, bp);
#elifdef SMTX
  ssle_ldu(mp, xp, bp);
#endif

  mtx_fput(fmo, mp);
  vec_fput(fx, xp);

  mtx_free(mp);
  vec_free(bp);
  vec_free(xp);

  fclose(fdms);
  fclose(fmtx);
  fclose(fb);
  fclose(fx);
  fclose(fmo);
}
