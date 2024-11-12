#include <stdio.h>

#include <iss_csj.h>

void func(int k, double res) {
  printf("\rIt: %d; Res: %lf", k, res);
}

int main() {
  struct iss_csj_fio f = {
    .f = {
      .pps = fopen("los.pps", "r"),
      .x = fopen("x.vec", "r"),
      .f = fopen("f.vec", "w+"),
    },
    .m = {
      .pps = fopen("pps.csj.mtx", "r"),
      .lr = fopen("lr.csj.mtx", "r"),
      .ur = fopen("ur.csj.mtx", "r"),
      .dr = fopen("dr.csj.mtx", "r"),
      .il = fopen("il.csj.mtx", "r"),
      .jl = fopen("jl.csj.mtx", "r"),
      .iu = fopen("iu.csj.mtx", "r"),
      .ju = fopen("ju.csj.mtx", "r"),
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
  vec_fput(f.f.f, fp);
  vec_cls(xp);

  iss_csj_ilu_los_solve(mp, xp, fp, &iss_pps, &func);

  putchar('\n');
  vec_fput(stdout, xp);
  putchar('\n');

  mtx_csj_free(mp);
  iss_csj_fio_close(&f);

  return 0;
}
