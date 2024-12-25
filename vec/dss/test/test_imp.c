#include <errno.h>
#include <munit.h>
#include <stdio.h>
#include <string.h>
#include <vec/dss.h>

static MunitResult test_red(const MunitParameter[], void*) {
  struct imtx* m;
  struct vec*  x;
  struct vec*  f;

  struct ipps pps = {3, 3};

  mtx_new(&m, pps);
  vec_new(&x, 3);
  vec_new(&f, 3);

  m->data[0][0] = 4;
  m->data[0][1] = 3;
  m->data[0][2] = 5;
  m->data[1][0] = 0;
  m->data[1][1] = 0;
  m->data[1][2] = 1;
  m->data[2][0] = 2;
  m->data[2][1] = -1;
  m->data[2][2] = 2;

  f->data[0] = 25;
  f->data[1] = 3;
  f->data[2] = 6;

  if (dss_red_slv(m, x, f))
    fprintf(stderr, "err: %s\n", strerror(errno));

  munit_assert_double_equal(1.0, x->data[0], 3);
  munit_assert_double_equal(2.0, x->data[1], 3);
  munit_assert_double_equal(3.0, x->data[2], 3);

  mtx_cls(&m);
  vec_cls(&x);
  vec_cls(&f);

  return MUNIT_OK;
}

static MunitTest itests[] = {
  {"/red", test_red, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
};

static const MunitSuite suite = {"/imp", itests, NULL, 1, MUNIT_SUITE_OPTION_NONE};

int main(int argc, char** argv) {
  return munit_suite_main(&suite, NULL, argc, argv);
}
