#ifndef SLE_H
#define SLE_H

#include <sle_dns.h>
#include <sle_pfl.h>

#define sle_ldu(X, yp, fp) _Generic((X), struct mtx_pfl *: ssle_ldu, )(X, yp, fp)

#define sle_gauss(X, yp, fp) \
  _Generic((X), struct mtx_dns *: dsle_gauss, )(X, yp, fp)

#endif  // SLE_H
