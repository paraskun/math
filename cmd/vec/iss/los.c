#include <stdio.h>

#include <iss_csr.h>

int main(int argc, char* argv[argc]) {
  FILE* fpi = fopen("los.pps", "r");
  FILE* fmi = fopen(argv[1], "r");

  struct iss_pps pps;
  int n, ne;

  fscanf(fpi, "%d %d", &n, &ne);
  iss_pps_fget(fpi, &pps);

  struct mtx_csr* mp = mtx_csr_new(n, ne);

  mtx_csr_fget(fmi, mp);

  mtx_csr_ilu(mp, mp);

  mtx_csr_fput(stdout, mp, 1);

  mtx_csr_free(mp);

  return 0;
}
