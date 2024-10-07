#ifndef MTX_H
#define MTX_H

#include <dmtx.h>
#include <smtx.h>
#include <pmtx.h>

#define mtx_fget(f, X) _Generic((X),    \
    struct dmtx *: dmtx_fget,           \
    struct smtx *: smtx_fget,           \
    struct pmtx *: pmtx_fget            \
    ) (f, X)

#define mtx_fput(f, X) _Generic((X),    \
    struct dmtx *: dmtx_fput,           \
    struct smtx *: smtx_fput            \
    ) (f, X)

#define mtx_ddm(X, k) _Generic((X),     \
    struct dmtx *: dmtx_ddm,            \
    struct smtx *: smtx_ddm             \
    ) (X, k)

#define mtx_hlb(X) _Generic((X),        \
    struct dmtx *: dmtx_hlb,            \
    struct smtx *: smtx_hlb             \
    ) (X)

#define mtx_vmlt(X, b, r) _Generic((X), \
    struct dmtx *: dmtx_vmlt,           \
    struct smtx *: smtx_vmlt,           \
    struct pmtx *: pmtx_vmlt            \
    ) (X, b, r)

#define mtx_free(X) _Generic((X),       \
    struct dmtx *: dmtx_free,           \
    struct smtx *: smtx_free,           \
    struct pmtx *: pmtx_free            \
    ) (X)

#endif  // MTX_H
