#ifndef GDS_HSET_H
#define GDS_HSET_H

#include <stdbool.h>
#include <dsa/type.h>

struct ihset;

int ihset_ini(struct ihset** h);
int ihset_cls(struct ihset** h);

int ihset_new(struct ihset* s, uint cap);
int ihset_hash(struct ihset* s, uint (*hash)(int e, uint cap));

uint ihset_len(struct ihset* s);
bool ihset_has(struct ihset* s, int e);

int ihset_ins(struct ihset* s, int e);
int ihset_pop(struct ihset* s, int e);
int ihset_rst(struct ihset* s);

struct uhset;

int uhset_ini(struct uhset** h);
int uhset_cls(struct uhset** h);

int uhset_new(struct uhset* s, uint cap);
int uhset_hash(struct uhset* s, uint (*hash)(uint e, uint cap));

uint uhset_len(struct uhset* s);
bool uhset_has(struct uhset* s, uint e);

int uhset_ins(struct uhset* s, uint e);
int uhset_pop(struct uhset* s, uint e);
int uhset_rst(struct uhset* s);

#define hset_ini(X) _Generic((X), \
    struct ihset**: ihset_ini,    \
    struct uhset**: uhset_ini     \
    )(X)

#define hset_cls(X) _Generic((X), \
    struct ihset**: ihset_cls,    \
    struct uhset**: uhset_cls     \
    )(X)

#define hset_new(X, c) _Generic((X),  \
    struct ihset*: ihset_new,         \
    struct uhset*: uhset_new          \
    )(X, c)

#define hset_hash(X, h) _Generic((X), \
    struct ihset*: ihset_hash,        \
    struct uhset*: uhset_hash         \
    )(X, h)

#define hset_len(X) _Generic((X), \
    struct ihset*: ihset_len,     \
    struct uhset*: uhset_len      \
    )(X)

#define hset_has(X, e) _Generic((X),  \
    struct ihset*: ihset_has,         \
    struct uhset*: uhset_has          \
    )(X, e)

#define hset_ins(X, e) _Generic((X),  \
    struct ihset*: ihset_ins,         \
    struct uhset*: uhset_ins          \
    )(X, e)

#define hset_pop(X, e) _Generic((X),  \
    struct ihset*: ihset_pop,         \
    struct uhset*: uhset_pop          \
    )(X, e)

#define hset_rst(X) _Generic((X), \
    struct ihset*: ihset_rst,     \
    struct uhset*: uhset_rst      \
    )(X)

#endif  // GDS_HSET_H
