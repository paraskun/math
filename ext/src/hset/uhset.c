#include <ext/hset.h>

#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

struct uhset {
  uint len;
  uint cap;

  uint (*hash)(uint, uint);

  bool* data;
};

static uint hash(uint e, uint cap) {
  return e % cap;
}

int uhset_ini(struct uhset** h) {
  if (!h) {
    errno = EINVAL;
    return -1;
  }

  struct uhset* s = malloc(sizeof(struct uhset));

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

int uhset_cls(struct uhset** h) {
  if (!h || !(*h)) {
    errno = EINVAL;
    return -1;
  }

  if ((*h)->data)
    free((*h)->data);

  free(*h);

  return 0;
}

int uhset_new(struct uhset* s, uint cap) {
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

  memset(s->data, 0, sizeof(bool) * cap);

  return 0;
}

int uhset_hash(struct uhset* s, uint (*hash)(uint e, uint cap)) {
  if (!s) {
    errno = EINVAL;
    return -1;
  }

  s->hash = hash;

  return 0;
}

uint uhset_len(struct uhset* s) {
  if (!s || !s->data) {
    errno = EINVAL;
    return -1;
  }

  return s->len;
}

bool uhset_has(struct uhset* s, uint e) {
  if (!s || !s->data) {
    errno = EINVAL;
    return false;
  }

  return s->data[s->hash(e, s->cap)];
}

int uhset_ins(struct uhset* s, uint e) {
  if (!s || !s->data) {
    errno = EINVAL;
    return -1;
  }

  if (uhset_has(s, e))
    return 0;

  if (s->len == s->cap) {
    errno = EDQUOT;
    return -1;
  }

  s->data[s->hash(e, s->cap)] = true;
  s->len += 1;

  return 0;
}

int uhset_pop(struct uhset* s, uint e) {
  if (!s || !s->data) {
    errno = EINVAL;
    return -1;
  }

  if (!uhset_has(s, e))
    return 0;

  s->data[s->hash(e, s->cap)] = false;
  s->len -= 1;

  return 0;
}

int uhset_rst(struct uhset* s) {
  if (!s || !s->data) {
    errno = EINVAL;
    return -1;
  }

  memset(s->data, 0, sizeof(bool) * s->cap);

  return 0;
}
