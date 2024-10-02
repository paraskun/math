#ifndef SLE_H
#define SLE_H

#ifdef DMTX

#define sle dsle_gauss
#include <dsle.h>

#elifdef SMTX

#define sle ssle_ldu
#include <ssle.h>

#endif

#endif  // SLE_H
