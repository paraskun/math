#ifndef DCG_ALS_H
#define DCG_ALS_H

#include <gds/ssll.h>

struct edge {
  unsigned vtx;
  int wgt;
};

struct dcg {
  unsigned cap;

  struct pssll** data;
};

int dcg_new(struct dcg** h, unsigned cap);
int dcg_add(struct dcg* g, unsigned src, unsigned dst, int wgt);
int dcg_ssp(struct dcg* g, unsigned src, struct edge* map);
int dcg_asp(struct dcg* g, struct edge** map);
int dcg_cls(struct dcg* g);

#endif  // DCG_ALS_H
