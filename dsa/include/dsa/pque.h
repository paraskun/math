#ifndef GDS_PQUE_H
#define GDS_PQUE_H

#define PQUE_L(i) (i * 2)
#define PQUE_R(i) (i * 2 + 1)
#define PQUE_P(i) (i / 2)

typedef unsigned uint;

struct ipque {
  uint len;
  uint cap;

  int (*cmp)(int, int);

  int* data;
};

int ipque_new(struct ipque* q, uint cap);
int ipque_fix(struct ipque* q);

int ipque_fixu(struct ipque* q, uint i);
int ipque_fixd(struct ipque* q, uint i);

int ipque_ins(struct ipque* q, int e);
int ipque_ext(struct ipque* q, int* e);
int ipque_srt(struct ipque* q);
int ipque_cls(struct ipque* q);

struct ppque {
  uint len;
  uint cap;

  int (*cmp)(void*, void*);

  void** data;
};

int ppque_new(struct ppque* q, uint cap);
int ppque_fix(struct ppque* q);

int ppque_fixu(struct ppque* q, uint i);
int ppque_fixd(struct ppque* q, uint i);

int ppque_ins(struct ppque* q, void* e);
int ppque_ext(struct ppque* q, void** e);
int ppque_srt(struct ppque* q);
int ppque_cls(struct ppque* q);

#define pque_new(X, c) _Generic((X), \
    struct ipque*: ipque_new,        \
    struct ppque*: ppque_new         \
    )(X, c)

#define pque_fix(X) _Generic((X),     \
    struct ipque*: ipque_fix,         \
    struct ppque*: ppque_fix          \
    )(X)

#define pque_fixu(X, i) _Generic((X), \
    struct ipque*: ipque_fixu,        \
    struct ppque*: ppque_fixu         \
    )(X, i)

#define pque_fixd(X, i) _Generic((X), \
    struct ipque*: ipque_fixd,        \
    struct ppque*: ppque_fixd         \
    )(X, i)

#define pque_ins(X, e) _Generic((X),  \
    struct ipque*: ipque_ins,         \
    struct ppque*: ppque_ins          \
    )(X, e)

#define pque_ext(X, e) _Generic((X),  \
    struct ipque*: ipque_ext,         \
    struct ppque*: ppque_ext          \
    )(X, e)

#define pque_srt(X) _Generic((X),     \
    struct ipque*: ipque_srt,         \
    struct ppque*: ppque_srt          \
    )(X)

#define pque_cls(X) _Generic((X), \
    struct ipque*: ipque_cls,     \
    struct ppque*: ppque_cls      \
    )(X)

#endif  // GDS_PQUE_H
