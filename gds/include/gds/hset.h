#ifndef GDS_HSET_H
#define GDS_HSET_H

#include <stdbool.h>

struct ihset {
  unsigned len;
  unsigned cap;

  unsigned (*hash)(int, unsigned cap);

  bool* data;
};

int ihset_new(struct ihset** h, unsigned cap);
int ihset_ins(struct ihset* s, int e);

bool ihset_has(struct ihset* s, int e);

int ihset_del(struct ihset* s, int e);
int ihset_cls(struct ihset* s);

struct phset {
  unsigned len;
  unsigned cap;

  unsigned (*hash)(void*, unsigned cap);

  bool* data;
};

int phset_new(struct phset** h, unsigned cap);
int phset_ins(struct phset* s, void* e);

bool phset_has(struct phset* s, void* e);

int phset_del(struct phset* s, void* e);
int phset_cls(struct phset* s);

#define hset_new(X, c) _Generic((X),  \
    struct ihset**: ihset_new,        \
    struct phset**: phset_new         \
    )(X, c)

#define hset_ins(X, e) _Generic((X),  \
    struct ihset*: ihset_ins,         \
    struct phset*: phset_ins          \
    )(X, e)

#define hset_has(X, e) _Generic((X),  \
    struct ihset*: ihset_has,         \
    struct phset*: phset_has          \
    )(X, e)

#define hset_del(X, e) _Generic((X),  \
    struct ihset*: ihset_del,         \
    struct phset*: phset_del          \
    )(X, e)

#define hset_cls(X) _Generic((X), \
    struct ihset*: ihset_cls,     \
    struct phset*: phset_cls      \
    )(X)

#endif  // GDS_HSET_H
