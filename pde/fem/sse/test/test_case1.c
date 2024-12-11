#include <munit.h>
#include <string.h>
#include <math.h>

#include <fem/sse/fem.h>

static double u(struct vtx* v) {
  double x = v->x;
  double y = v->y;
  double z = v->z;

  return 5 + 0.2 * x + y + 30 * z + 0.5 * x * y 
    + x * z + 10 * y * z + x * y * z;
}

static double f(struct vtx* v) {
  double x = v->x;
  double y = v->y;
  double z = v->z;

  return 0.4 * (5 + 0.2 * x + y + 30 * z + 0.5 * x * y 
      + x * z + 10 * y * z + x * y * z);
}

static double tta1(struct vtx* v) {
  double y = v->y;
  double z = v->z;

  return -1 - 2.5 * y - 5 * z - 5 * y * z;
}

static double tta2(struct vtx* v) {
  double x = v->x;
  double z = v->z;

  return 5 + 2.5 * x + 50 * z + 5 * x * z;
}

static double tta3(struct vtx* v) {
  double x = v->x;
  double y = v->y;

  return 150 + 5 * x + 50 * y + 5 * x * y;
}

static double tmp1(struct vtx* v) {
  double y = v->y;
  double z = v->z;

  return 9.1 + 11.25 * y + 50.5 * z + 30.5 * y * z;
}

static double tmp2(struct vtx* v) {
  double x = v->x;
  double z = v->z;

  return 4.5 - 0.05 * x + 25 * z + 0.5 * x * z;
}

static double tmp3(struct vtx* v) {
  double x = v->x;
  double y = v->y;

  return -10 - 0.3 * x - 4 * y;
}

static double (*fun[7])(struct vtx*) = {
  &f, &tta1, &tta2, &tta3, &tmp1, &tmp2, &tmp3
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

  strcpy(buf, "rep/case1_");
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

  for (int i = 0; i < vec->n; ++i)
    munit_assert_double_equal(u(fem->vtx[i]), vec->vp[i], 5);

  fem_cls(&fem);
  vec_cls(vec);

  return MUNIT_OK;
}

static MunitTest tests[] = {
    {"/reg", test, NULL, NULL, MUNIT_TEST_OPTION_NONE, pps},
    {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}};

static MunitSuite suites[] = {
    {"/case1", tests, NULL, 1, MUNIT_SUITE_OPTION_NONE},
    {NULL, NULL, NULL, 1, MUNIT_SUITE_OPTION_NONE},
};

int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
  return munit_suite_main(&suites[0], NULL, argc, argv);
}
