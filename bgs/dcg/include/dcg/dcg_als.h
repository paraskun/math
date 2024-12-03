#ifndef DCG_ALS_H
#define DCG_ALS_H

#include <stdio.h>

// dcg_edg is a node for dcg_sll.
struct dcg_edg {
  int vtx;
  int wgt;

  struct dcg_edg* next;
};

// dcg_sll is a sorted linked list.
struct dcg_sll {
  int s;

  struct dcg_edg* beg;
  struct dcg_edg* end;
};

struct dcg_sll* dcg_sll_new();

int dcg_sll_add(struct dcg_sll* l, int vtx, int wgt);
int dcg_sll_ins(struct dcg_sll* l, struct dcg_edg** e, int vtx, int wgt);
int dcg_sll_srh(struct dcg_sll* l, struct dcg_edg** e, int vtx);

int dcg_sll_cls(struct dcg_sll* l);

struct dcg_als {
  int s;

  struct dcg_sll** als;
};

struct dcg_als* dcg_als_new(int s);

int dcg_als_fget(FILE* f, struct dcg_als* g, int w);
int dcg_als_fmap(FILE *f, struct dcg_als* g, int** map);

int dcg_als_add(struct dcg_als* g, int f, int t, int wgt);
int dcg_als_fwp(struct dcg_als* g, int **map);

int dcg_als_ssp_dij(struct dcg_als* g, int **map);
int dcg_als_asp_dij(struct dcg_als* g, int **map);

int dcg_als_nrm(struct dcg_als* g, double *nrm);

int dcg_als_cls(struct dcg_als* g);

#endif  // DCG_ALS_H
