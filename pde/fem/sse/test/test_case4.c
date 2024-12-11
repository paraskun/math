#include <munit.h>
#include <string.h>
#include <math.h>

#include <fem/sse/fem.h>

static double u(struct vtx* v) {
  double x = v->x;

  return 5 * x * x;
}

static double f(struct vtx* v) {
  double x = v->x;

  return -50 + 2 * x * x;
}

static double tta1(struct vtx* v) {
  double x = v->x;

  return -50 * x;
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

  return 2000 + 50 * x;
}

static double tmp2(struct vtx* v) {
  double x = v->x;

  return 5 * x * x;
}

static double tmp3(struct vtx* v) {
  double x = v->x;

  return 5 * x * x;
}

static double (*fun[7])(struct vtx*) = {
  &f, &tta1, &tta2, &tta3, &tmp1, &tmp2, &tmp3
};

static char* meshes[] = {"mesh1", NULL};

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

  strcpy(buf, "rep/case4_");
  strcat(buf, pps[0].value);

  FILE* o = fopen(buf, "w+");

  fem_ini(&fem);

  fem->pps.fun = fun;

  fem_fget(fem, f);
  fem_evo(fem);
  fem_asm(fem);

  vec = vec_new(fem->vs);

  fem_slv(fem, vec);

  for (int i = 0; i < fem->vs; ++i)
    fprintf(o, "%.7e %.7e %.7e\n", 
        u(fem->vtx[i]), 
        vec->vp[i], 
        fabs(u(fem->vtx[i]) - vec->vp[i]));

  fclose(f);
  fclose(o);

  fem_cls(&fem);
  vec_cls(vec);

  return MUNIT_OK;
}

static MunitTest tests[] = {
    {"/approx", test, NULL, NULL, MUNIT_TEST_OPTION_NONE, pps},
    {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}};

static MunitSuite suites[] = {
    {"/case4", tests, NULL, 1, MUNIT_SUITE_OPTION_NONE},
    {NULL, NULL, NULL, 1, MUNIT_SUITE_OPTION_NONE},
};

int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
  return munit_suite_main(&suites[0], NULL, argc, argv);
}
