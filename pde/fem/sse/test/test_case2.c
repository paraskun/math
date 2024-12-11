#include <munit.h>
#include <string.h>
#include <math.h>

#include <fem/sse/fem.h>

static double u(struct vtx* v) {
  double x = v->x;
  double y = v->y;
  double z = v->z;

  return x + y + z;
}

[[maybe_unused]]
static double f(struct vtx* v) {
  double x = v->x;
  double y = v->y;
  double z = v->z;

  return 0.4 * (x + y + z);
}

[[maybe_unused]]
static double g(struct vtx* v) {
  double x = v->x;
  double y = v->y;
  double z = v->z;

  return 0.4 * (0.5 * x * x + x * y + x * z);
}

static double tta1(struct vtx*) {
  return -5;
}

static double tta2(struct vtx*) {
  return 5;
}

static double tta3(struct vtx*) {
  return 5;
}

static double tmp1(struct vtx* v) {
  double y = v->y;
  double z = v->z;

  return 15.5 + y + z;
}

static double tmp2(struct vtx* v) {
  double y = v->y;
  double z = v->z;

  return 20.5 + y + z;
}

static double tmp3(struct vtx* v) {
  double x = v->x;
  double z = v->z;

  return -0.5 + x + z;
}

static double tmp4(struct vtx* v) {
  double x = v->x;
  double z = v->z;

  return 3.5 + x + z;
}

static double tmp5(struct vtx* v) {
  double x = v->x;
  double y = v->y;

  return -0.5 + x + y;
}

static double (*fun[9])(struct vtx*) = {
  &g, &tta1, &tta2, &tta3, &tmp1, &tmp2, &tmp3, &tmp4, &tmp5
};

static char* meshes[] = {"mesh5", NULL};

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

  strcpy(buf, "rep/case2_");
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
    {"/irreg", test, NULL, NULL, MUNIT_TEST_OPTION_NONE, pps},
    {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}};

static MunitSuite suites[] = {
    {"/case2", tests, NULL, 1, MUNIT_SUITE_OPTION_NONE},
    {NULL, NULL, NULL, 1, MUNIT_SUITE_OPTION_NONE},
};

int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
  return munit_suite_main(&suites[0], NULL, argc, argv);
}
