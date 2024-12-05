#ifndef GDS_SSLL_H
#define GDS_SSLL_H

typedef unsigned uint;

struct islln {
  int e;

  struct islln* next;
};

struct isll {
  uint len;

  int (*cmp)(int, int);

  struct islln* beg;
  struct islln* end;
};

int isll_new(struct isll* l);
int isll_add(struct isll* l, int e);
int isll_ins(struct isll* l, struct islln** h, int e);
int isll_srh(struct isll* l, struct islln** h, int e);
int isll_cls(struct isll* l);

struct pslln {
  void* e;

  struct pslln* next;
};

struct psll {
  uint len;

  int (*cmp)(void*, void*);

  struct pslln* beg;
  struct pslln* end;
};

int psll_new(struct psll* l);
int psll_add(struct psll* l, void* e);
int psll_ins(struct psll* l, struct pslln** h, void* e);
int psll_srh(struct psll* l, struct pslln** h, void* e);
int psll_cls(struct psll* l);

#define sll_new(X) _Generic((X), \
    struct isll*: isll_new,     \
    struct psll*: psll_new      \
    )(X)

#define sll_add(X, e) _Generic((X),  \
    struct isll*: isll_add,         \
    struct psll*: psll_add          \
    )(X, e)

#define sll_ins(X, h, e) _Generic((X), \
    struct isll*: isll_ins,           \
    struct psll*: psll_ins            \
    )(X, h, e)

#define sll_srh(X, h, e) _Generic((X), \
    struct isll*: isll_srh,           \
    struct psll*: psll_srh            \
    )(X, h, e)

#define sll_cls(X) _Generic((X), \
    struct isll*: isll_cls,     \
    struct psll*: psll_cls      \
    )(X)

#endif  // GDS_SSLL_H
