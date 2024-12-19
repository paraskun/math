#include <bas/mas.h>
#include <munit.h>

static double fun(struct vec* v) {
  double x = v->data[0];
  double y = v->data[1];
  double z = v->data[2];

  return x * x + y * y + z * z;
}

static MunitResult test_pdif(const MunitParameter[], void*) {
  struct vec* v;

  vec_new(&v, 3);
  
  v->data[0] = 3;
  v->data[1] = 2;
  v->data[2] = 1;

  double r;

  pdif(&fun, 0, 0.001, v, &r);
  vec_cls(&v);

  munit_assert_double_equal(6.0, r, 3);

  return MUNIT_OK;
}

static MunitTest itests[] = {
  {"/pdif", test_pdif, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
};

static const MunitSuite suite = {
  "/mas", itests, NULL, 1, MUNIT_SUITE_OPTION_NONE};

int main(int argc, char** argv) {
  return munit_suite_main(&suite, NULL, argc, argv);
}
