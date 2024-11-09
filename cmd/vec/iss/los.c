#include <stdio.h>

#include <iss_csj.h>

void func(int k, double res) {
  printf("\rIt: %d; Res: %lf", k, res);
}

int main() {
  struct iss_csj_fio f = {
    .f = {
      .pps = fopen("los.pps", "r"),
      .x = fopen("los-x.vec", "w"),
      .f = fopen("los-f.vec", "w+"),
    },
    .m = {
      .pps = fopen("los-csj-mtx.pps", "r"),
      .lr = fopen("los-lr-csj.mtx", "r"),
      .ur = fopen("los-ur-csj.mtx", "r"),
      .dr = fopen("los-dr-csj.mtx", "r"),
      .il = fopen("los-il-csj.mtx", "r"),
      .jl = fopen("los-jl-csj.mtx", "r"),
      .iu = fopen("los-iu-csj.mtx", "r"),
      .ju = fopen("los-ju-csj.mtx", "r"),
  }};

  struct iss_pps iss_pps;
  struct mtx_csj_pps mtx_pps;

  iss_pps_fget(&f.f, &iss_pps);
  mtx_csj_pps_fget(&f.m, &mtx_pps);

  struct mtx_csj* mp = mtx_csj_new(mtx_pps);
  struct vec* xp = vec_new(mp->pps.n);
  struct vec* fp = vec_new(mp->pps.n);

  mtx_csj_fget(&f.m, mp);
  vec_fget(f.f.x, xp);

  mtx_csj_vmlt(mp, xp, fp);
  vec_cls(xp);

  iss_csj_ilu_los_solve(mp, xp, fp, &iss_pps, &func);

  mtx_csj_free(mp);
  iss_csj_fio_close(&f);

  return 0;
}
