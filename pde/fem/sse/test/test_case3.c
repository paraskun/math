#include <munit.h>
#include <string.h>
#include <math.h>

#include <fem/sse/fem.h>

static double f1(struct vtx* v) {
  double x = v->x;
  double y = v->y;
  double z = v->z;

  return 8 * x * (y * z + z + y + 1);
}

static double f2(struct vtx* v) {
  double x = v->x;
  double y = v->y;
  double z = v->z;

  return x * 0 + y * 0 + z * 0;
}

static double f3(struct vtx* v) {
  double x = v->x;
  double y = v->y;
  double z = v->z;

  return 3 * x * z * (y + 1);
}

static double tmp0(struct vtx* v) {
  double x = v->x;
  double y = v->y;
  double z = v->z;

  return x * 0 + y * 0 + z * 0;
}

static double tta1(struct vtx* v) {
  double x = v->x;
  double z = v->z;

  return -2 * x * (z + 1);
}

static double tta2(struct vtx* v) {
  double x = v->x;
  double z = v->z;

  return -2 * x * z;
}

static double tta3(struct vtx* v) {
  double x = v->x;
  double z = v->z;

  return -2 * (1 + x + z + x * z);
}

static double tta4(struct vtx* v) {
  double y = v->y;
  double z = v->z;

  return 2 * (1 + y + z + y * z);
}

static double tta5(struct vtx* v) {
  double x = v->x;
  double y = v->y;

  return 2 * (1 + x + y + x * y);
}

static double tta6(struct vtx* v) {
  double y = v->y;
  double z = v->z;

  return 2 * z * (1 + y);
}

static double tta7(struct vtx* v) {
  double x = v->x;
  double y = v->y;

  return 2 * x * (1 + y);
}

static double tta8(struct vtx* v) {
  double x = v->x;
  double y = v->y;

  return -2 * x * (1 + y);
}

static double tta9(struct vtx* v) {
  double x = v->x;
  double y = v->y;

  return -2 * (1 + x + y + x * y);
}

static double tmp1(struct vtx* v) {
  double x = v->x;
  double z = v->z;

  return 8 * x * (z + 1);
}

static double tmp2(struct vtx* v) {
  double x = v->x;
  double z = v->z;

  return 11.5 * x * z;
}

static double tmp3(struct vtx* v) {
  double x = v->x;
  double z = v->z;

  return 4.5 * (1 + x + z + x * z);
}

static double (*fun[16])(struct vtx*) = {
  &f1, &f2, &f3, &tmp0,
  &tta1, &tta2, &tta3, &tta4, &tta5, &tta6, &tta7, &tta8, &tta9, 
  &tmp1, &tmp2, &tmp3
};

static char* meshes[] = {"mesh6"};

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

  strcpy(buf, "rep/case3_");
  strcat(buf, pps[0].value);

  FILE* o = fopen(buf, "w+");

  fem_ini(&fem);

  fem->pps.fun = fun;

  fem_fget(fem, f);
  fem_evo(fem);
  fem_asm(fem);

  vec = vec_new(fem->vs);

  fem_slv(fem, vec);

  double ans[] = {
    0, 2, 6, 0, 7, 21, 0, 6, 
    18, 0, 21, 63, 0, 21, 0, 73.5
  };

  for (int i = 0; i < fem->vs; ++i)
    fprintf(o, "%.7e %.7e %.7e\n", 
        ans[i], 
        vec->vp[i], 
        fabs(ans[i] - vec->vp[i]));

  fclose(f);
  fclose(o);

  for (int i = 0; i < vec->n; ++i)
    munit_assert_double_equal(ans[i], vec->vp[i], 5);

  fem_cls(&fem);
  vec_cls(vec);

  return MUNIT_OK;
}

static MunitTest tests[] = {
    {"/rupt", test, NULL, NULL, MUNIT_TEST_OPTION_NONE, pps},
    {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}};

static MunitSuite suites[] = {
    {"/case3", tests, NULL, 1, MUNIT_SUITE_OPTION_NONE},
    {NULL, NULL, NULL, 1, MUNIT_SUITE_OPTION_NONE},
};

int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
  return munit_suite_main(&suites[0], NULL, argc, argv);
}
