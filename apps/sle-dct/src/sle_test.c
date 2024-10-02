#include <mtx.h>
#include <sle.h>

#include <stdio.h>

int main() {
#ifdef SMTX
  FILE* fdms = fopen("smtx-dms.in", "r");
  FILE* fmtx = fopen("smtx-mtx.in", "r");
  FILE* fmo = fopen("smtx-mtx.out", "w+");
#elifdef DMTX
  FILE* fdms = fopen("dmtx-dms.in", "r");
  FILE* fmtx = fopen("dmtx-mtx.in", "r");
  FILE* fmo = fopen("dmtx-mtx.out", "w+");
#endif

  FILE* fb = fopen("b.in", "r");
  FILE* fx = fopen("x.out", "w+");

#ifdef SMTX
  int n, s;
  fscanf(fdms, "%d %d", &n, &s);
  mtx* mp = smtx_new(n, s);
#elifdef DMTX
  int n;
  fscanf(fdms, "%d", &n);
  mtx* mp = dmtx_new(n);
#endif

  struct vec* bp = vec_new(n);
  struct vec* xp = vec_new(n);

  mtx_fget(fmtx, mp);
  vec_fget(fb, bp);

  sle(mp, xp, bp);

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
