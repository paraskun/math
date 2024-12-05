#include <munit.h>
#include <bgs/dcg.h>

static MunitResult test_ssp(const MunitParameter[], void*) {
  struct dcg g;

  dcg_new(&g, 4);

  dcg_add(&g, 0, 1, 3);
  dcg_add(&g, 0, 3, 9);
  dcg_add(&g, 1, 2, 0);
  dcg_add(&g, 2, 3, 2);
  dcg_add(&g, 3, 1, 1);

  struct path** map = malloc(sizeof(struct path*) * 4);

  for (int i = 0; i < 4; ++i)
    map[i] = malloc(sizeof(struct path));

  dcg_ssp(&g, 0, map);

  for (int i = 0; i < 4; ++i) {
    munit_logf(MUNIT_LOG_INFO, "%d -> %d [%d]: %d", 1, map[i]->dst + 1, map[i]->wgt, map[i]->hop);
  }

  return MUNIT_OK;
}

static MunitResult test_asp(const MunitParameter[], void*) {
  return MUNIT_OK;
}

static MunitTest dcg_tests[] = {
  { "/test_ssp", test_ssp, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  { "/test_asp", test_asp, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static MunitSuite suites[] = {
  { "/dcg", dcg_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE },
  { NULL, NULL, NULL, 1, MUNIT_SUITE_OPTION_NONE },
};

int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
  return munit_suite_main(&suites[0], NULL, argc, argv);
}
