#include <bas/mas.h>
#include <errno.h>

int pdif(double (*f)(struct vec*), uint i, double h, struct vec* p, double* r) {
  if (!f || !p || !r || i >= p->dim) {
    errno = EINVAL;
    return -1;
  }

  p->data[i] += h;
  double f1 = f(p);

  p->data[i] -= 2 * h;
  double f2 = f(p);

  p->data[i] += h;
  *r = (f1 - f2) / (2 * h);

  return 0;
}
