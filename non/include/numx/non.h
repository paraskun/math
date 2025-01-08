#ifndef NON_H
#define NON_H

typedef double (*fun)(struct vec *);

int pdif(fun f, uint i, double hop, struct vec *p, double *r);

#endif // NON_H
