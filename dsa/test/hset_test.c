#include <munit.h>
#include <dsa/hset.h>

static MunitResult test_iadd(const MunitParameter[], void*) {
  struct ihset* set [[gnu::cleanup(ihset_cls)]];

  hset_ini(&set);
  hset_new(set, 5);

  hset_ins(set, 1);
  hset_ins(set, 2);
  hset_ins(set, 3);

  munit_assert_true(hset_has(set, 1));
  munit_assert_true(hset_has(set, 2));
  munit_assert_true(hset_has(set, 3));

  munit_assert_false(hset_has(set, 4));

  return MUNIT_OK;
}

static MunitTest ihset_tests[] = {
  { "/test_add", test_iadd, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static MunitSuite suites[] = {
  { "/ihset", ihset_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE },
  { NULL, NULL, NULL, 1, MUNIT_SUITE_OPTION_NONE },
};

static const MunitSuite suite = { "hset", NULL, suites, 1, MUNIT_SUITE_OPTION_NONE };

int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
  return munit_suite_main(&suite, NULL, argc, argv);
}
