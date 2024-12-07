#ifndef DCG_ALS_H
#define DCG_ALS_H

#include <dsa/sll.h>
#include <limits.h>

struct dcg;

struct path {
  uint dst;
  uint hop;

  double wgt;
};

int dcg_ini(struct dcg** h);
int dcg_cls(struct dcg** h);

int dcg_new(struct dcg* g, uint cap);
int dcg_add(struct dcg* g, uint src, uint dst, double wgt);
int dcg_ssp(struct dcg* g, uint src, struct path** map);
int dcg_asp(struct dcg* g, struct path*** map);

#endif  // DCG_ALS_H
