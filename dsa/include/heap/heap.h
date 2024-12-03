#ifndef HEAP_H
#define HEAP_H

typedef int (*heap_fun_cmp)(void*, void*);

struct heap {
  int cap;
  int len;

  heap_fun_cmp cmp;

  void** dat;
};

struct heap* heap_new(int cap, heap_fun_cmp cmp);

int heap_fixu(struct heap* h, int i);
int heap_fixd(struct heap* h, int i);

int heap_ins(struct heap* h, void* e);
int heap_ext(struct heap* h, void** e);

int heap_cls(struct heap* h);

#endif  // HEAP_H
