#ifndef DCG_ALS_H
#define DCG_ALS_H

#include <gds/ssll.h>
#include <limits.h>

#define INF UINT_MAX

typedef unsigned uint;

struct edge {
  uint vtx;
  uint wgt;
};

struct dcg {
  uint cap;

  struct pssll* data;
};

struct path {
  uint dst;
  uint hop;
  uint wgt;
};

int dcg_new(struct dcg* g, uint cap);
int dcg_add(struct dcg* g, uint src, uint dst, uint wgt);
int dcg_ssp(struct dcg* g, uint src, struct path** map);
int dcg_asp(struct dcg* g, struct path*** map);
int dcg_cls(struct dcg* g);

#endif  // DCG_ALS_H
