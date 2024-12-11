#include <munit.h>
#include <string.h>
#include <math.h>

#include <fem/sse/fem.h>

static double u(struct vtx* v) {
  double x = v->x;

  return log(x);
}

static double f(struct vtx* v) {
  double x = v->x;

  return 5 / (x * x) * 0.4 * log(x);
}

static double tta1(struct vtx* v) {
  double x = v->x;

  return -5 / x;
}

static double tta2(struct vtx* v) {
  double x = v->x;

  return 0 * x;
}

static double tta3(struct vtx* v) {
  double x = v->x;

  return 0 * x;
}

static double tmp1(struct vtx* v) {
  double x = v->x;

  return log(20) + 0.5 / x;
}

static double tmp2(struct vtx* v) {
  double x = v->x;

  return log(x);
}

static double tmp3(struct vtx* v) {
  double x = v->x;

  return log(x);
}

static double (*fun[7])(struct vtx*) = {
  &f, &tta1, &tta2, &tta3, &tmp1, &tmp2, &tmp3
};

static struct vtx vtx[8] = {
  {12.5, 1, 0.5},
  {17.5, 1, 0.5},
  {12.5, 3, 0.5},
  {17.5, 3, 0.5},
  {12.5, 5, 0.5},
  {17.5, 5, 0.5},
  {12.5, 7, 0.5},
  {17.5, 7, 0.5},
};

static uint hex[4][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 1, 1, 1, 1},
  {0, 1, 0, 1, 2, 3, 2, 3},
  {0, 1, 2, 3, 4, 5, 6, 7},
};

static char* meshes[] = {"mesh1", "mesh2", "mesh3", "mesh4", NULL};

static MunitParameterEnum pps[] = {
  { "mesh", meshes },
  { NULL, NULL }
};

static MunitResult test(const MunitParameter pps[], void*) {
  char buf[255];

  strcpy(buf, "dat/");
  strcat(buf, pps[0].value);

  struct fem* fem;
  struct vec* vec;

  FILE* f = fopen(buf, "r");
  FILE* o = fopen("rep/case5", "a+");

  int n;

  if (strchr(pps[0].value, '1'))
    n = 0;
  else if (strchr(pps[0].value, '2'))
    n = 1;
  else if (strchr(pps[0].value, '3'))
    n = 2;
  else
    n = 3;

  fem_ini(&fem);

  fem->pps.fun = fun;

  fem_fget(fem, f);
  fem_evo(fem);
  fem_asm(fem);

  vec = vec_new(fem->vs);

  fem_slv(fem, vec);

  double t = 0;
  double s = 0;
  double r = 0;

  for (int i = 0; i < 8; ++i) {
    fem_get(fem, vec, &vtx[i], hex[n][i], &r);

    s += r;
    t += u(&vtx[i]);
  }

  fprintf(o, "%s: %lf\t\t%lf\n", pps[0].value, s, t);

  fclose(f);
  fclose(o);

  fem_cls(&fem);
  vec_cls(vec);

  return MUNIT_OK;
}

static MunitTest tests[] = {
    {"/conv", test, NULL, NULL, MUNIT_TEST_OPTION_NONE, pps},
    {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}};

static MunitSuite suites[] = {
    {"/case5", tests, NULL, 1, MUNIT_SUITE_OPTION_NONE},
    {NULL, NULL, NULL, 1, MUNIT_SUITE_OPTION_NONE},
};

int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
  return munit_suite_main(&suites[0], NULL, argc, argv);
}
