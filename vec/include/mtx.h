#ifndef MTX_H
#define MTX_H

#include <mtx_dns.h>
#include <mtx_pfl.h>
#include <mtx_pie.h>

#define mtx_fget(f, X)          \
  _Generic((X),                 \
      struct mtx_dns *: mtx_dns_fget, \
      struct mtx_pfl *: mtx_pfl_fget, \
      struct mtx_pie *: mtx_pie_fget)(f, X)

#define mtx_fput(f, X)          \
  _Generic((X),                 \
      struct mtx_dns *: mtx_dns_fput, \
      struct mtx_pfl *: mtx_pfl_fput, \
      struct mtx_pie *: mtx_pie_fput)(f, X)

#define mtx_ddm(X, k) \
  _Generic((X), struct mtx_dns *: mtx_dns_ddm, struct mtx_pfl *: mtx_pfl_ddm)(X, k)

#define mtx_hlb(X) \
  _Generic((X), struct mtx_dns *: mtx_dns_hlb, struct mtx_pfl *: mtx_pfl_hlb)(X)

#define mtx_vmlt(X, b, r)       \
  _Generic((X),                 \
      struct mtx_dns *: mtx_dns_vmlt, \
      struct mtx_pfl *: mtx_pfl_vmlt, \
      struct mtx_pie *: mtx_pie_vmlt)(X, b, r)

#define mtx_free(X)             \
  _Generic((X),                 \
      struct mtx_dns *: mtx_dns_free, \
      struct mtx_pfl *: mtx_pfl_free, \
      struct mtx_pie *: mtx_pie_free)(X)

#endif  // MTX_H
