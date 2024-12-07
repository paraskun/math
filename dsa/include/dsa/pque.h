#ifndef DSA_PQUE_H
#define DSA_PQUE_H

#include <dsa/type.h>

struct ipque;

int ipque_ini(struct ipque** h);
int ipque_cls(struct ipque** h);

int ipque_new(struct ipque* q, uint cap);
int ipque_cov(struct ipque* q, int* s, uint cap);

int ipque_cmp(struct ipque* q, int (*cmp)(int, int));

uint ipque_cap(struct ipque* q);
uint ipque_len(struct ipque* q);

int ipque_fix(struct ipque* q);

int ipque_fixu(struct ipque* q, uint i);
int ipque_fixd(struct ipque* q, uint i);

int ipque_ins(struct ipque* q, int e);
int ipque_ext(struct ipque* q, int* e);
int ipque_rst(struct ipque* q);
int ipque_srt(struct ipque* q);

struct upque;

int upque_ini(struct upque** h);
int upque_cls(struct upque** h);

int upque_new(struct upque* q, uint cap);
int upque_cov(struct upque* q, uint* s, uint cap);

int upque_cmp(struct upque* q, int (*cmp)(uint, uint));

uint upque_cap(struct upque* q);
uint upque_len(struct upque* q);

int upque_fix(struct upque* q);

int upque_fixu(struct upque* q, uint i);
int upque_fixd(struct upque* q, uint i);

int upque_ins(struct upque* q, uint e);
int upque_ext(struct upque* q, uint* e);
int upque_rst(struct upque* q);
int upque_srt(struct upque* q);

struct dpque;

int dpque_ini(struct dpque** h);
int dpque_cls(struct dpque** h);

int dpque_new(struct dpque* q, uint cap);
int dpque_cov(struct dpque* q, double* s, uint cap);

int dpque_cmp(struct dpque* q, int (*cmp)(double, double));

uint dpque_cap(struct dpque* q);
uint dpque_len(struct dpque* q);

int dpque_fix(struct dpque* q);

int dpque_fixu(struct dpque* q, uint i);
int dpque_fixd(struct dpque* q, uint i);

int dpque_ins(struct dpque* q, double e);
int dpque_ext(struct dpque* q, double* e);
int dpque_rst(struct dpque* q);
int dpque_srt(struct dpque* q);

struct ppque;

int ppque_ini(struct ppque** h);
int ppque_cls(struct ppque** h);

int ppque_new(struct ppque* q, uint cap);
int ppque_cov(struct ppque* q, void** s, uint cap);

int ppque_cmp(struct ppque* q, int (*cmp)(void*, void*));

uint ppque_cap(struct ppque* q);
uint ppque_len(struct ppque* q);

int ppque_fix(struct ppque* q);

int ppque_fixu(struct ppque* q, uint i);
int ppque_fixd(struct ppque* q, uint i);

int ppque_ins(struct ppque* q, void* e);
int ppque_ext(struct ppque* q, void** e);
int ppque_rst(struct ppque* q);
int ppque_srt(struct ppque* q);

// pque_ini initializes the queue.
#define pque_ini(X) _Generic((X), \
    struct ipque**: ipque_ini,    \
    struct upque**: upque_ini,    \
    struct dpque**: dpque_ini,    \
    struct ppque**: ppque_ini     \
    )(X)

// pque_cls cleans up the queue.
#define pque_cls(X) _Generic((X), \
    struct ipque**: ipque_cls,    \
    struct upque**: upque_cls,    \
    struct dpque**: dpque_cls,    \
    struct ppque**: ppque_cls     \
    )(X)

// pque_new allocates new internal array 
// of the given capacity and zero length.
#define pque_new(X, cap) _Generic((X),  \
    struct ipque*: ipque_new,           \
    struct upque*: upque_new,           \
    struct dpque*: dpque_new,           \
    struct ppque*: ppque_new            \
    )(X, cap)

// pque_cov sets the given array as an 
// internal array with the given capacity 
// as both capacity and length.
#define pque_cov(X, s, cap) _Generic((X), \
    struct ipque*: ipque_cov,             \
    struct upque*: upque_cov,             \
    struct dpque*: dpque_cov,             \
    struct ppque*: ppque_cov              \
    )(X, s, cap)

// pque_cmp sets the given function as 
// the comparison function for the queue.
#define pque_cmp(X, cmp) _Generic((X),  \
    struct ipque*: ipque_cmp,           \
    struct upque*: upque_cmp,           \
    struct dpque*: dpque_cmp,           \
    struct ppque*: ppque_cmp            \
    )(X, cmp)

// pque_cap gets capacity of the
// internal array.
#define pque_cap(X) _Generic((X), \
    struct ipque*: ipque_cap,     \
    struct upque*: upque_cap,     \
    struct dpque*: dpque_cap,     \
    struct ppque*: ppque_cap      \
    )(X)

// pque_len gets current length of 
// the queue.
#define pque_len(X) _Generic((X), \
    struct ipque*: ipque_len,     \
    struct upque*: upque_len,     \
    struct dpque*: dpque_len,     \
    struct ppque*: ppque_len      \
    )(X)

// pque_fix fixes the queue structure.
#define pque_fix(X) _Generic((X), \
    struct ipque*: ipque_fix,     \
    struct upque*: upque_fix,     \
    struct dpque*: dpque_fix,     \
    struct ppque*: ppque_fix      \
    )(X)

// pque_fixu fixes the queue structure 
// in case the priority of the i'th 
// element is increased.
#define pque_fixu(X, i) _Generic((X), \
    struct ipque*: ipque_fixu,        \
    struct upque*: upque_fixu,        \
    struct dpque*: dpque_fixu,        \
    struct ppque*: ppque_fixu         \
    )(X, i)

// pque_fixu fixes the queue structure 
// in case the priority of the i'th 
// element is decreased.
#define pque_fixd(X, i) _Generic((X), \
    struct ipque*: ipque_fixd,        \
    struct upque*: upque_fixd,        \
    struct dpque*: dpque_fixd,        \
    struct ppque*: ppque_fixd         \
    )(X, i)

// pque_ins inserts a new element into
// the queue.
#define pque_ins(X, e) _Generic((X),  \
    struct ipque*: ipque_ins,         \
    struct upque*: upque_ins,         \
    struct dpque*: dpque_ins,         \
    struct ppque*: ppque_ins          \
    )(X, e)

// pque_ext fetches the element with
// the highest priority.
#define pque_ext(X, e) _Generic((X),  \
    struct ipque*: ipque_ext,         \
    struct upque*: upque_ext,         \
    struct dpque*: dpque_ext,         \
    struct ppque*: ppque_ext          \
    )(X, e)

// pque_rst resets queue length to
// match capacity.
#define pque_rst(X) _Generic((X), \
    struct ipque*: ipque_rst,     \
    struct upque*: upque_rst,     \
    struct dpque*: dpque_rst,     \
    struct ppque*: ppque_rst      \
    )(X)

// pque_srt sorts the queue in the 
// order determined by the provided 
// comparison function.
#define pque_srt(X) _Generic((X), \
    struct ipque*: ipque_srt,     \
    struct upque*: upque_srt,     \
    struct dpque*: dpque_srt,     \
    struct ppque*: ppque_srt      \
    )(X)

#endif  // DSA_PQUE_H
