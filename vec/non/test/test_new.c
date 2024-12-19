#include <vec/non.h>
#include <munit.h>
#include <math.h>

static double f1(struct vec* v) {
  double x = v->data[0];
  double y = v->data[1];
  double z = v->data[2];

  return 3 * x - cos(y * z) - 0.5;
}

static double f2(struct vec* v) {
  double x = v->data[0];
  double y = v->data[1];
  double z = v->data[2];

  return x * x - 81 * (y + 0.1) * (y + 0.1) + sin(z) + 1.06;
}

static double f3(struct vec* v) {
  double x = v->data[0];
  double y = v->data[1];
  double z = v->data[2];

  return exp(-x * y) + 20 * z + (10 * M_PI - 3) / 3;
}

static MunitResult test_new(const MunitParameter[], void*) {
  double (*f[3])(struct vec*) = {&f1, &f2, &f3};

  struct vec* x;

  vec_new(&x, 3);
  
  x->data[0] = 0.1;
  x->data[1] = 0.1;
  x->data[2] = -0.1;

  non_new_slv(3, f, x, (struct non_pps){
      .res = nullptr,
      .cbk = nullptr,
      .hop = 0.001,
      .eps = 0.001,
      .hem = 1000
  });

  munit_assert_double_equal(0.5, x->data[0], 3);
  munit_assert_double_equal(0.0, x->data[1], 3);
  munit_assert_double_equal(-0.523598, x->data[2], 3);

  vec_cls(&x);

  return MUNIT_OK;
}

static MunitTest itests[] = {
  {"/new", test_new, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
};

static const MunitSuite suite = {
  "/non", itests, NULL, 1, MUNIT_SUITE_OPTION_NONE};

int main(int argc, char** argv) {
  return munit_suite_main(&suite, NULL, argc, argv);
}
