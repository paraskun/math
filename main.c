#include <stdlib.h>

#include <mtx/mtx_pfl.h>
#include <sle/sle.h>
#include <vec/vec.h>

int main(int argc, char* argv[argc]) {
  if (argc < 2)
    exit(-1);

  size_t n = strtol(argv[1], NULL, 10);
  size_t s = strtol(argv[2], NULL, 10);

  struct mtx* m = mtx_new(n, s);
  struct vec* b = vec_new(n);

  mtx_fget("mtx", m);
  vec_fget("vec-b", b);

  sle_gauss(m, b, b);

  mtx_fput("mtx-ldu", m);
  vec_fput("vec-x", b);

  mtx_free(m);
  vec_free(b);

  return 0;
}
