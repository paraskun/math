#include <mtx_cds.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[argc]) {
  int n = strtold(argv[1], 0);
  int c = strtod(argv[2], 0);
  int m = strtod(argv[3], 0);
  int k = strtod(argv[4], 0);

  struct mtx_cds* mp = mtx_cds_new(n, c);

  FILE* fmo = fopen("ddm.cds.mtx", "w+");

  mtx_cds_ddm(mp, m, k);
  mtx_cds_fput(fmo, mp);

  fclose(fmo);
  mtx_cds_free(mp);

  return 0;
}
