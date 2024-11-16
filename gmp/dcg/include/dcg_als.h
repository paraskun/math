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

// dcg_sll_add adds a new element to the list.
int dcg_sll_add(struct dcg_sll* l, int vtx, int wgt);

// dcg_sll_ins inserts a new element after e and assigns it
// to e. If e is NULL, assigns a new value to e and inserts
// it at the beginning.
int dcg_sll_ins(struct dcg_sll* l, struct dcg_edg** e, int vtx, int wgt);

// dcg_sll_srh looks for vtx in l. If the node was found,
// assigns it to e.
int dcg_sll_srh(struct dcg_sll* l, struct dcg_edg** e, int vtx);

int dcg_sll_cls(struct dcg_sll* l);

struct dcg_als {
  int s;
  int** map;

  struct dcg_sll** als;
};

struct dcg_als* dcg_als_new(int s);

// dcg_als_get searches for edges in f and writes them to g
// with pseudo-random weights.
int dcg_als_get(FILE* f, struct dcg_als* g);

int dcg_als_map(FILE* f, struct dcg_als* g);

int dcg_als_add(struct dcg_als* g, int f, int t, int wgt);
int dcg_als_fwp(struct dcg_als* g, int map);

int dcg_als_cls(struct dcg_als* g);

#endif  // DCG_ALS_H
