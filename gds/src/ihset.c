#include <gds/hset.h>

#include <errno.h>
#include <limits.h>
#include <stdlib.h>

static uint hash(int e, uint cap) {
  uint u = e < 0 ? (UINT_MAX / 2 - e) : (uint)e;
  return u % cap;
}

int ihset_new(struct ihset* s, uint cap) {
  if (!s) {
    errno = EINVAL;
    return -1;
  }

  s->len = 0;
  s->cap = cap;
  s->hash = &hash;
  s->data = malloc(sizeof(bool) * cap);

  if (!s->data) {
    errno = ENOMEM;
    return -1;
  }

  return 0;
}

int ihset_ins(struct ihset* s, int e) {
  if (!s) {
    errno = EINVAL;
    return -1;
  }

  if (ihset_has(s, e))
    return 0;

  if (s->len == s->cap) {
    errno = EDQUOT;
    return -1;
  }

  s->data[s->hash(e, s->cap)] = true;
  s->len += 1;

  return 0;
}

bool ihset_has(struct ihset* s, int e) {
  if (!s) {
    errno = EINVAL;
    return false;
  }

  return s->data[s->hash(e, s->cap)];
}

int ihset_del(struct ihset* s, int e) {
  if (!s) {
    errno = EINVAL;
    return -1;
  }

  if (!ihset_has(s, e))
    return 0;

  s->data[s->hash(e, s->cap)] = false;
  s->len -= 1;

  return 0;
}

int ihset_cls(struct ihset* s) {
  if (!s) {
    errno = EINVAL;
    return -1;
  }

  free(s->data);

  return 0;
}
