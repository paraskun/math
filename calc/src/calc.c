#include <calc.h>
#include <errno.h>

int pdif(mfun f, uint i, double hop, struct vec* p, double* r) {
  if (!f || !p || !r || i >= p->dim) {
    errno = EINVAL;
    return -1;
  }

  p->data[i] += hop;
  double f1 = f(p);

  p->data[i] -= 2 * hop;
  double f2 = f(p);

  p->data[i] += hop;
  *r = (f1 - f2) / (2 * hop);

  return 0;
}
