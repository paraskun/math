#ifndef DSS_H
#define DSS_H

#include <vec/mtx.h>
#include <vec/vec.h>

int idss_red_slv(struct imtx* m, struct vec* x, struct vec* f);

#define dss_red_slv(X, x, f) _Generic((X),  \
    struct imtx*: idss_red_slv              \
    )(X, x, f)

#endif  // DSS_H
