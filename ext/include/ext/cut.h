#ifndef EXT_CUT_H
#define EXT_CUT_H

#include <ext/type.h>

struct icut;

int icut_ini(struct icut **h);
int icut_cls(struct icut **h);

int icut_new(struct icut *c, uint cap);
int icut_cov(struct icut *c, int *s, uint cap);
int icut_mov(struct icut *c, int *s, uint cap);
int icut_exp(struct icut *c, uint cap);
int icut_shr(struct icut *c);

int icut_add(struct icut *c, int e);
int icut_set(struct icut *c, uint i, int e);
int icut_ins(struct icut *c, uint i, int e);

int icut_get(struct icut *c, uint i, int *h);
int icut_srt(struct icut *c, int (*cmp)(int, int));

uint icut_len(struct icut *c);
uint icut_cap(struct icut *c);

#endif  // EXT_CUT_H
