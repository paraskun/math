#include <mtx_csj.h>

int main() {
  FILE* a = fopen("all.mtx", "r");
  struct mtx_csj_fio f = {
      .pps = fopen("pps.csj.mtx", "r"),
      .lr = fopen("lr.csj.mtx", "w+"),
      .ur = fopen("ur.csj.mtx", "w+"),
      .dr = fopen("dr.csj.mtx", "w+"),
      .il = fopen("il.csj.mtx", "w+"),
      .jl = fopen("jl.csj.mtx", "w+"),
      .iu = fopen("iu.csj.mtx", "w+"),
      .ju = fopen("ju.csj.mtx", "w+"),
  };

  struct mtx_csj_pps pps;

  mtx_csj_pps_fget(&f, &pps);

  struct mtx_csj* mp = mtx_csj_new(pps);

  mtx_csj_fget_all(a, mp);
  mtx_csj_fput(&f, mp);

  mtx_csj_fio_close(&f);

  return 0;
}
