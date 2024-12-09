#include <math.h>
#include <munit.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <bgs/dcg.h>

struct fixture {
  FILE* rep;

  struct dcg* g;
  struct path*** map;
};

static char* cases[] = {"case1", "case2", "case3"};

static MunitParameterEnum pps[] = {{"case", cases}, {NULL, NULL}};

static void* setup(const MunitParameter pps[], void*) {
  struct fixture* fix = malloc(sizeof(struct fixture));

  char buf[255];
  uint cap;
  uint e;

  strcpy(buf, "rep/");
  strcat(buf, pps[0].value);

  fix->rep = fopen(buf, "w+");

  strcpy(buf, "dat/");
  strcat(buf, pps[0].value);

  FILE* f = fopen(buf, "r");

  fscanf(f, "%u %u", &cap, &e);

  dcg_ini(&fix->g);
  dcg_new(fix->g, cap);

  for (uint i = 0; i < e; ++i) {
    uint src;
    uint dst;
    double wgt;

    fscanf(f, "%u %u %lf", &src, &dst, &wgt);

    dcg_add(fix->g, src - 1, dst - 1, wgt);
  }

  fclose(f);

  fix->map = malloc(sizeof(struct path**) * cap);

  for (uint i = 0; i < cap; ++i) {
    fix->map[i] = malloc(sizeof(struct path*) * cap);

    for (uint j = 0; j < cap; ++j)
      fix->map[i][j] = malloc(sizeof(struct path));
  }

  return fix;
}

static void tear_down(void* data) {
  struct fixture* fix = (struct fixture*)data;

  uint cap = dcg_cap(fix->g);

  for (uint i = 0; i < cap; ++i)
    for (uint j = 0; j < cap; ++j) {
      struct path* p = fix->map[i][j];

      if (p->wgt < INFINITY) {
        fprintf(fix->rep, "%u -> %u [%lf]: %u", i + 1, j + 1, p->wgt, i + 1);

        uint cur = i;
        uint hop = p->hop;

        while (hop != cur) {
          fprintf(fix->rep, " - %u", hop + 1);

          cur = hop;
          hop = fix->map[hop][j]->hop;
        }

        fprintf(fix->rep, " - %u\n", j + 1);
      }
    }

  fclose(fix->rep);

  for (uint i = 0; i < dcg_cap(fix->g); ++i) {
    for (uint j = 0; j < dcg_cap(fix->g); ++j)
      free(fix->map[i][j]);

    free(fix->map[i]);
  }

  free(fix->map);
  dcg_cls(&fix->g);
  free(fix);
}

static MunitResult test(const MunitParameter[], void* data) {
  struct fixture* fix = (struct fixture*) data;

  dcg_asp(fix->g, fix->map);

  return MUNIT_OK;
}

static MunitTest dcg_tests[] = {
    {"/asp", test, setup, tear_down, MUNIT_TEST_OPTION_NONE, pps},
    {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}};

static MunitSuite suites[] = {
    {"/dcg", dcg_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE},
    {NULL, NULL, NULL, 1, MUNIT_SUITE_OPTION_NONE},
};

int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
  return munit_suite_main(&suites[0], NULL, argc, argv);
}
