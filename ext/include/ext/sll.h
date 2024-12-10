#ifndef EXT_SLL_H
#define EXT_SLL_H

#include <ext/type.h>

struct isll;

struct isln {
  int e;

  struct isln *next;
};

int isll_ini(struct isll** h);
int isll_cls(struct isll** h);

int isll_cmp(struct isll* l, int (*cmp)(int, int));

int isll_next(struct isll* l, struct isln** i);

int isll_add(struct isll* l, int e);
int isll_ins(struct isll* l, struct isln** h, int e);
int isll_pop(struct isll* l, struct isln** h, int* e);
int isll_srh(struct isll* l, struct isln** h, int e);

uint isll_len(struct isll* l);

struct psln;
struct psll;

int psll_ini(struct psll** h);
int psll_cls(struct psll** h);

int psll_cmp(struct psll* l, int (*cmp)(void*, void*));
int psll_ctl(struct psll* l, bool ctl);

int psll_next(struct psll* l, struct psln** i);

uint psll_len(struct psll* l);

int psll_add(struct psll* l, void* e);
int psll_ins(struct psll* l, struct psln** h, void* e);
int psll_pop(struct psll* l, struct psln** h, void** e);
int psll_get(struct psln* n, void** e);
int psll_srh(struct psll* l, struct psln** h, void* e);

#define sll_ini(X) _Generic((X),  \
    struct isll**: isll_ini,      \
    struct psll**: psll_ini       \
    )(X)

#define sll_cls(X) _Generic((X),  \
    struct isll**: isll_cls,      \
    struct psll**: psll_cls       \
    )(X)

#define sll_cmp(X, c) _Generic((X), \
    struct isll*: isll_cmp,         \
    struct psll*: psll_cmp          \
    )(X, c)

#define sll_ctl(X, c) _Generic((X), \
    struct psll*: psll_ctl          \
    )(X, c)

#define sll_next(X, i) _Generic((X), \
    struct isll*: isll_next,         \
    struct psll*: psll_next          \
    )(X, i)

#define sll_len(X) _Generic((X),  \
    struct isll*: isll_len,       \
    struct psll*: psll_len        \
    )(X)

#define sll_add(X, e) _Generic((X), \
    struct isll*: isll_add,         \
    struct psll*: psll_add          \
    )(X, e)

#define sll_ins(X, h, e) _Generic((X),  \
    struct isll*: isll_ins,             \
    struct psll*: psll_ins              \
    )(X, h, e)

#define sll_pop(X, h, e) _Generic((X),  \
    struct isll*: isll_pop,             \
    struct psll*: psll_pop              \
    )(X, h, e)

#define sll_get(X, e) _Generic((X), \
    struct isln*: isll_get,         \
    struct psln*: psll_get          \
    )(X, e)

#define sll_srh(X, h, e) _Generic((X),  \
    struct isll*: isll_srh,             \
    struct psll*: psll_srh              \
    )(X, h, e)

#endif  // EXT_SLL_H
