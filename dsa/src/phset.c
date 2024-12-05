#include <dsa/hset.h>

#include <errno.h>
#include <stdlib.h>

static uint hash(void* e, uint cap) {
  return (unsigned long) e % cap;
}

int phset_new(struct phset* s, uint cap) {
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

int phset_ins(struct phset* s, void* e) {
  if (!s) {
    errno = EINVAL;
    return -1;
  }

  if (phset_has(s, e))
    return 0;

  if (s->len == s->cap) {
    errno = EDQUOT;
    return -1;
  }

  s->data[s->hash(e, s->cap)] = true;
  s->len += 1;

  return 0;
}

bool phset_has(struct phset* s, void* e) {
  if (!s) {
    errno = EINVAL;
    return false;
  }

  return s->data[s->hash(e, s->cap)];
}

int phset_del(struct phset* s, void* e) {
  if (!s) {
    errno = EINVAL;
    return -1;
  }

  if (!phset_has(s, e))
    return 0;

  s->data[s->hash(e, s->cap)] = false;
  s->len -= 1;

  return 0;
}

int phset_cls(struct phset* s) {
  if (!s) {
    errno = EINVAL;
    return -1;
  }

  free(s->data);

  return 0;
}
