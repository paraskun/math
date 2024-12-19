#ifndef EXT_DLL_H
#define EXT_DLL_H

#include <ext/type.h>

struct idll;

struct idln {
  int e;

  struct isln *prev;
  struct isln *next;
};

int idll_ini(struct idll **h);
int idll_cls(struct idll **h);

int idll_cmp(struct idll *l, int (*cmp)(int, int));

int idll_prev(struct idll *l, struct isln **i);
int idll_next(struct idll *l, struct isln **i);

int idll_add(struct idll *l, int e);
int idll_ins(struct idll *l, struct isln **h, int e);
int idll_pop(struct idll *l, struct isln **h, int *e);
int idll_srh(struct idll *l, struct isln **h, int e);

uint idll_len(struct idll *l);

#endif  // EXT_DLL_H
