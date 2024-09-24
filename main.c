#include <def.h>
#include <mtx/smtx.h>
#include <sle/sle.h>
#include "mtx/mtx.h"

#define N 3000

int main(int argc, char* argv[argc]) {
  struct mtx* m = mtx_newdd(3, 1);

  mtx_fput("mtx", m);
  mtx_free(m);

  return 0;
}
