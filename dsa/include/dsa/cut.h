#ifndef DSA_CUT_H
#define DSA_CUT_H

#include <dsa/type.h>

struct icut;

int icut_ini(struct icut** h);
int icut_cls(struct icut** h);

int icut_new(struct icut* c, uint cap);
int icut_cov(struct icut* c, int* s, uint cap);
int icut_shr(struct icut* c);
int icut_exp(struct icut* c);
int icut_add(struct icut* c, int e);
int icut_set(struct icut* c, uint i, int e);
int icut_srt(struct icut* c, int (*cmp)(int, int));

uint icut_len(struct icut* c);
uint icut_cap(struct icut* c);

int icut_get(struct icut* c, uint i);

#endif  // DSA_CUT_H
