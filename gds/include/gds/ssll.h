#ifndef GDS_SSLL_H
#define GDS_SSLL_H

struct isln {
  int e;

  struct isln* next;
};

struct issll {
  unsigned len;

  int (*cmp)(int, int);

  struct isln* beg;
  struct isln* end;
};

int issll_new(struct issll** h);
int issll_add(struct issll* l, int e);
int issll_ins(struct issll* l, struct isln* p, int e);
int issll_srh(struct issll* l, struct isln** h, int e);
int issll_cls(struct issll* l);

struct psln {
  void* e;

  struct isln* next;
};

struct pssll {
  unsigned len;

  int (*cmp)(void*, void*);

  struct psln* beg;
  struct psln* end;
};

int pssll_new(struct pssll** h);
int pssll_add(struct pssll* l, void* e);
int pssll_ins(struct pssll* l, struct psln* p, void* e);
int pssll_srh(struct pssll* l, struct psln** h, void* e);
int pssll_cls(struct pssll* l);

#define ssll_new(X) _Generic((X), \
    struct issll**: issll_new,    \
    struct pssll**: pssll_new     \
    )(X)

#define ssll_add(X, e) _Generic((X),  \
    struct issll*: issll_add,         \
    struct pssll*: pssll_add          \
    )(X, e)

#define ssll_ins(X, p, e) _Generic((X), \
    struct issll*: issll_ins,           \
    struct pssll*: pssll_ins            \
    )(X, p, e)

#define ssll_srh(X, h, e) _Generic((X), \
    struct issll*: issll_srh,           \
    struct pssll*: pssll_srh            \
    )(X, h, e)

#define ssll_cls(X) _Generic((X), \
    struct issll*: issll_cls,     \
    struct pssll*: pssll_cls      \
    )(X)

#endif  // GDS_SSLL_H
