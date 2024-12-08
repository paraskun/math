#include <bgs/dcg.h>

#include <dsa/hset.h>
#include <dsa/pque.h>
#include <dsa/sll.h>

#include <errno.h>
#include <math.h>
#include <stdlib.h>

struct edge {
  uint dst;
  double wgt;
};

struct dcg {
  uint cap;

  struct psll** data;
};

static int cmp(void* a, void* b) {
  struct edge* ea = (struct edge*) a;
  struct edge* eb = (struct edge*) b;

  if (ea->dst < eb->dst)
    return 1;

  if (ea->dst > eb->dst)
    return -1;

  return 0;
}

int dcg_ini(struct dcg** h) {
  if (!h) {
    errno = EINVAL;
    return -1;
  }

  struct dcg* g = malloc(sizeof(struct dcg));

  if (!g) {
    errno = ENOMEM;
    return -1;
  }

  g->cap = 0;
  g->data = nullptr;

  *h = g;

  return 0;
}

int dcg_cls(struct dcg** h) {
  if (!h || !(*h)) {
    errno = EINVAL;
    return -1;
  }

  struct dcg* g = *h;

  if (g->data) {
    for (uint i = 0; i < g->cap; ++i)
      sll_cls(&g->data[i]);

    free(g->data);
  }

  free(g);
  *h = nullptr;

  return 0;
}

int dcg_new(struct dcg* g, uint cap) {
  if (!g) {
    errno = EINVAL;
    return -1;
  }

  g->cap = cap;
  g->data = malloc(sizeof(struct psll*) * cap);

  if (!g->data) {
    errno = ENOMEM;
    return -1;
  }

  for (uint i = 0; i < cap; ++i) {
    if (sll_ini(&g->data[i])) {
      for (uint j = 0; j < i; ++j)
        sll_cls(&g->data[j]);

      free(g->data);
      g->data = nullptr;

      return -1;
    }

    sll_cmp(g->data[i], &cmp);
    sll_ctl(g->data[i], true);
  }

  return 0;
}

int dcg_add(struct dcg* g, uint src, uint dst, double wgt) {
  if (!g || src >= g->cap || dst >= g->cap) {
    errno = EINVAL;
    return -1;
  }

  struct edge* e = malloc(sizeof(struct edge));

  if (!e) {
    errno = ENOMEM;
    return -1;
  }

  e->dst = dst;
  e->wgt = wgt;

  if (sll_add(g->data[src], e)) {
    free(e);
    return -1;
  }

  return 0;
}


uint dcg_cap(struct dcg* g) {
  if (!g) {
    errno = EINVAL;
    return -1;
  }

  return g->cap;
}

static int cmp_wgt(void* a, void* b) {
  struct path* pa = (struct path*)a;
  struct path* pb = (struct path*)b;

  if (pa->wgt < pb->wgt)
    return 1;

  if (pa->wgt > pb->wgt)
    return -1;

  return 0;
}

static int cmp_dst(void* a, void* b) {
  struct path* pa = (struct path*)a;
  struct path* pb = (struct path*)b;

  if (pa->dst > pb->dst)
    return 1;

  if (pa->dst < pb->dst)
    return -1;

  return 0;
}

int dcg_ssp(struct dcg* g, uint src, struct path** map) {
  if (!g || !map) {
    errno = EINVAL;
    return -1;
  }

  for (uint i = 0; i < g->cap; ++i) {
    map[i]->dst = i;

    if (i == src) {
      map[i]->hop = i;
      map[i]->wgt = 0;
    } else {
      map[i]->hop = 0;
      map[i]->wgt = INFINITY;
    }
  }

  struct ppque* que [[gnu::cleanup(ppque_cls)]];
  struct ihset* set [[gnu::cleanup(ihset_cls)]];

  pque_ini(&que);
  hset_ini(&set);

  pque_cmp(que, &cmp_wgt);
  pque_cov(que, (void**)map, g->cap);
  pque_fix(que);

  if (hset_new(set, g->cap))
    return -1;

  struct path* p = nullptr;

  while (pque_len(que) != 0) {
    pque_ext(que, (void**)&p);
    hset_ins(set, p->dst);

    struct psln* i = nullptr;
    struct edge* e = nullptr;

    while (sll_next(g->data[p->dst], &i)) {
      sll_get(i, (void**)&e);

      if (hset_has(set, e->dst))
        continue;

      for (uint i = 0; i < pque_len(que); ++i) {
        if (map[i]->dst == e->dst) {
          double alt = p->wgt + e->wgt;
          double wgt = map[i]->wgt;

          if (isinf(wgt) || alt < wgt) {
            map[i]->wgt = alt;
            map[i]->hop = p->dst;

            pque_fixu(que, i + 1);
          }

          break;
        }
      }
    }
  }

  pque_rst(que);
  pque_cmp(que, &cmp_dst);
  pque_fix(que);
  pque_srt(que);

  return 0;
}

int dcg_asp(struct dcg* g, struct path*** map) {
  if (!g || !map) {
    errno = EINVAL;
    return -1;
  }

#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (uint i = 0; i < g->cap; ++i)
    dcg_ssp(g, i, map[i]);

  return 0;
}
