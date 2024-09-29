#include <mtx.h>
#include <sle.h>
#include <vec.h>

#define N 3
#define S 3
#define K 1

int main() {
  struct mtx* mp = mtx_new(N, S);
  struct vec* xp = vec_new(N);

  mtx_ddm(mp, K);
  vec_seq(xp);

  return 0;
}
