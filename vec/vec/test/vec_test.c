#include <munit.h>
#include <stdio.h>
#include <string.h>
#include <numx/vec/vec.h>

static MunitResult test_new(const MunitParameter[], void*) {
  struct vec v;

  vec_new_ini(&v, 3, 1, 2, 3);

  munit_assert_double_equal(1, v.dat[0], 15);
  munit_assert_double_equal(2, v.dat[1], 15);
  munit_assert_double_equal(3, v.dat[2], 15);

  vec_cls(&v);
  
  return MUNIT_OK;
}

static MunitTest tests[] = {
  {"/new", test_new, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
};

static const MunitSuite suite = {"/vec", tests, NULL, 1, MUNIT_SUITE_OPTION_NONE};

int main(int argc, char** argv) {
  return munit_suite_main(&suite, NULL, argc, argv);
}
