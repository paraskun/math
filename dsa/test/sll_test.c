#include <munit.h>
#include <dsa/sll.h>

[[maybe_unused]]
static int cmp_dsc(int a, int b) {
  if (a > b)
    return 1;

  if (a < b)
    return -1;

  return 0;
}

[[maybe_unused]]
static int cmp_asc(int a, int b) {
  if (a < b)
    return 1;

  if (a > b)
    return -1;

  return 0;
}

static MunitResult test_iadd(const MunitParameter[], void*) {
  struct isll* l [[gnu::cleanup(isll_cls)]];

  sll_ini(&l);
  
  sll_add(l, 1);
  sll_add(l, 2);
  sll_add(l, 3);

  struct isln* i = nullptr;
  int e;

  for (int j = 1; sll_next(l, &i); ++j) {
    if (sll_get(i, &e))
      return MUNIT_FAIL;

    munit_assert_int(j, ==, e);
  }

  return MUNIT_OK;
}

static MunitResult test_iadd_cmp(const MunitParameter[], void*) {
  struct isll* l [[gnu::cleanup(isll_cls)]];

  sll_ini(&l);
  sll_cmp(l, &cmp_dsc);
  
  sll_add(l, 1);
  sll_add(l, 2);
  sll_add(l, 3);

  struct isln* i = nullptr;
  int e;

  for (int j = 1; sll_next(l, &i); ++j) {
    if (sll_get(i, &e))
      return MUNIT_FAIL;

    munit_assert_int(4 - j, ==, e);
  }

  return MUNIT_OK;
}

static MunitTest isll_tests[] = {
  { "/test_add_cmp", test_iadd_cmp, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  { "/test_add", test_iadd, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static MunitSuite suites[] = {
  { "/isll", isll_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE },
  { NULL, NULL, NULL, 1, MUNIT_SUITE_OPTION_NONE },
};

static const MunitSuite suite = { "sll", NULL, suites, 1, MUNIT_SUITE_OPTION_NONE };

int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
  return munit_suite_main(&suite, NULL, argc, argv);
}
