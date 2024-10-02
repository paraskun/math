#ifndef MTX_H
#define MTX_H

#ifdef DMTX

typedef struct dmtx mtx;

#define mtx_fget dmtx_fget
#define mtx_fput dmtx_fput
#define mtx_ddm dmtx_ddm
#define mtx_hlb dmtx_hlb
#define mtx_vmlt dmtx_vmlt
#define mtx_free dmtx_free

#include <dmtx.h>

#elifdef SMTX

typedef struct smtx mtx;

#define mtx_fget smtx_fget
#define mtx_fput smtx_fput
#define mtx_ddm smtx_ddm
#define mtx_hlb smtx_hlb
#define mtx_vmlt smtx_vmlt
#define mtx_free smtx_free

#include <smtx.h>

#endif

#endif  // MTX_H
