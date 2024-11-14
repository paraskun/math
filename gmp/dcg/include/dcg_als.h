#ifndef DCG_ALS_H
#define DCG_ALS_H

#include <stdio.h>

struct edge {
  int vtx;
  int wgt;

  struct edge* next;
};

struct list {
  int n;

  struct edge* top;
  struct edge* bot;
};

struct list* list_new();

int list_add(struct list* l, int vtx, int wgt);
int list_cls(struct list* l);

struct dcg_als {
  int n;

  struct list** als;
};

struct dcg_als* dcg_als_new(int n);

int dcg_als_get(FILE* f, struct dcg_als* g);
int dcg_als_map(FILE* f, int n, int** len, int** map);

int dcg_als_add(struct dcg_als* g, int a, int b, int wgt);
int dcg_als_fwp(struct dcg_als* g, int** len, int** map);
int dcg_als_cls(struct dcg_als* g);

#endif  // DCG_ALS_H
