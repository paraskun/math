#include <munit.h>
#include <dsa/pque.h>

[[maybe_unused]]
static int cmp_min(int a, int b) {
  if (a > b)
    return -1;

  if (a < b)
    return 1;

  return 0;
}

[[maybe_unused]]
static int cmp_max(int a, int b) {
  if (a < b)
    return -1;

  if (a > b)
    return 1;

  return 0;
}

static MunitResult test_isrt(const MunitParameter[], void*) {
  int mem[3] = {3, 1, 2};

  struct ipque* que [[gnu::cleanup(ipque_cls)]];

  pque_ini(&que);
  pque_cov(que, mem, 3);
  pque_cmp(que, &cmp_max);

  pque_fix(que);
  pque_srt(que);

  munit_assert_int(mem[0], ==, 1);
  munit_assert_int(mem[1], ==, 2);
  munit_assert_int(mem[2], ==, 3);

  return MUNIT_OK;
}

static MunitTest ipque_tests[] = {
  { "/test_srt", test_isrt, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static MunitSuite suites[] = {
  { "/ipque", ipque_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE },
  { NULL, NULL, NULL, 1, MUNIT_SUITE_OPTION_NONE },
};

static const MunitSuite suite = { "pque", NULL, suites, 1, MUNIT_SUITE_OPTION_NONE };

int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
  return munit_suite_main(&suite, NULL, argc, argv);
}
