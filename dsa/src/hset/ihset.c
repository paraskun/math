#include <dsa/hset.h>

#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

struct ihset {
  uint len;
  uint cap;

  uint (*hash)(int, uint);

  bool* data;
};

static uint hash(int e, uint cap) {
  uint u = e < 0 ? (UINT_MAX / 2 - e) : (uint)e;
  return u % cap;
}

int ihset_ini(struct ihset** h) {
  if (!h) {
    errno = EINVAL;
    return -1;
  }

  struct ihset* s = malloc(sizeof(struct ihset));

  if (!s) {
    errno = ENOMEM;
    return -1;
  }

  s->cap = 0;
  s->len = 0;
  s->hash = &hash;
  s->data = nullptr;

  *h = s;

  return 0;
}

int ihset_cls(struct ihset** h) {
  if (!h || !(*h)) {
    errno = EINVAL;
    return -1;
  }

  if ((*h)->data)
    free((*h)->data);

  free(*h);

  return 0;
}

int ihset_new(struct ihset* s, uint cap) {
  if (!s) {
    errno = EINVAL;
    return -1;
  }

  s->len = 0;
  s->cap = cap;
  s->data = malloc(sizeof(bool) * cap);

  if (!s->data) {
    errno = ENOMEM;
    return -1;
  }

  return 0;
}

int ihset_hash(struct ihset* s, uint (*hash)(int e, uint cap)) {
  if (!s) {
    errno = EINVAL;
    return -1;
  }

  s->hash = hash;

  return 0;
}

uint ihset_len(struct ihset* s) {
  if (!s || !s->data) {
    errno = EINVAL;
    return -1;
  }

  return s->len;
}

bool ihset_has(struct ihset* s, int e) {
  if (!s || !s->data) {
    errno = EINVAL;
    return false;
  }

  return s->data[s->hash(e, s->cap)];
}

int ihset_ins(struct ihset* s, int e) {
  if (!s || !s->data) {
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

int ihset_pop(struct ihset* s, int e) {
  if (!s || !s->data) {
    errno = EINVAL;
    return -1;
  }

  if (!ihset_has(s, e))
    return 0;

  s->data[s->hash(e, s->cap)] = false;
  s->len -= 1;

  return 0;
}

int ihset_rst(struct ihset* s) {
  if (!s || !s->data) {
    errno = EINVAL;
    return -1;
  }

  memset(s->data, 0, sizeof(bool) * s->cap);

  return 0;
}
