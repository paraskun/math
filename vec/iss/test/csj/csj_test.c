#include <munit.h>
#include <string.h>

#include <vec/mtx_csj.h>
#include <vec/iss_csj.h>

struct args {
  FILE *rep;

  struct mtx_csj* mp;

  struct vec* xp;
  struct vec* fp;

  struct iss_pps pps;
  struct iss_res res;

  double tnrm;
};

static void* ddm_setup(const MunitParameter[], void*) {
  struct args* args = malloc(sizeof(struct args));

  args->pps.mk = 200000;
  args->pps.eps = 1e-10;

  args->rep = fopen("out/ddm.rep", "a+");

  struct mtx_csj_pkt pkt = {
    .pps = 0,
    .dr = fopen("mtx/ddm/dr.csj.mtx", "r"),
    .lr = fopen("mtx/ddm/lr.csj.mtx", "r"),
    .ur = fopen("mtx/ddm/ur.csj.mtx", "r"),
    .ia = fopen("mtx/ddm/ia.csj.mtx", "r"),
    .ja = fopen("mtx/ddm/ja.csj.mtx", "r"),
  };

  args->mp = mtx_csj_new(10, 17);
  args->fp = vec_new(10);
  args->xp = vec_new(10);

  vec_seq(args->xp, 1);

  mtx_csj_fget(&pkt, args->mp);
  mtx_csj_vmlt(args->mp, args->xp, args->fp);
  
  vec_zer(args->xp);
  mtx_csj_pkt_cls(&pkt);

  return args;

}

static void ddm_tear_down(void *data) {
  struct args* args = (struct args*) data;

  mtx_csj_cls(args->mp);
  vec_cls(args->xp);
  vec_cls(args->fp);

  fclose(args->rep);
  free(args);
}

static MunitResult test_ddm_bcg(const MunitParameter[], void* data) {
  struct args* args = (struct args*) data;

  iss_csj_bcg_slv(args->mp, args->xp, args->fp, &args->pps, &args->res, NULL);
  fprintf(args->rep, "bcg: %d, %.7e\n", args->res.k, args->res.res);
  
  vec_put(args->rep, args->xp);

  return MUNIT_OK;
}

static MunitResult test_ddm_bcg_ilu(const MunitParameter[], void*) {
  return MUNIT_OK;
}

static MunitResult test_ddm_bcg_dgl(const MunitParameter[], void*) {
  return MUNIT_OK;
}

static MunitTest ddm_tests[] = {
  { "/bcg", test_ddm_bcg, ddm_setup, ddm_tear_down, MUNIT_TEST_OPTION_NONE, NULL },
  { "/bcg_ilu", test_ddm_bcg_ilu, ddm_setup, ddm_tear_down, MUNIT_TEST_OPTION_NONE, NULL },
  { "/bcg_dgl", test_ddm_bcg_dgl, ddm_setup, ddm_tear_down, MUNIT_TEST_OPTION_NONE, NULL },
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static MunitResult test_hlb_bcg(const MunitParameter[], void*) {
  return MUNIT_OK;
}

static MunitResult test_hlb_bcg_ilu(const MunitParameter[], void*) {
  return MUNIT_OK;
}

static MunitResult test_hlb_bcg_dgl(const MunitParameter[], void*) {
  return MUNIT_OK;
}

static MunitTest hlb_tests[] = {
  { "/bcg", test_hlb_bcg, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  { "/bcg_ilu", test_hlb_bcg_ilu, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  { "/bcg_dgl", test_hlb_bcg_dgl, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static void* big_setup(const MunitParameter[], void*) {
  struct args* args = malloc(sizeof(struct args));

  args->pps.mk = 200000;
  args->pps.eps = 1e-10;

  args->rep = fopen("out/big2.rep", "a+");

  struct mtx_csj_pkt pkt = {
    .pps = 0,
    .dr = fopen("mtx/big2/dr.csj.mtx", "r"),
    .lr = fopen("mtx/big2/lr.csj.mtx", "r"),
    .ur = fopen("mtx/big2/ur.csj.mtx", "r"),
    .ia = fopen("mtx/big2/ia.csj.mtx", "r"),
    .ja = fopen("mtx/big2/ja.csj.mtx", "r"),
  };

  FILE* ff = fopen("mtx/big2/f.vec", "r");

  args->mp = mtx_csj_new(945, 9440);
  args->fp = vec_new(945);
  args->xp = vec_new(945);

  mtx_csj_fget(&pkt, args->mp);
  vec_get(ff, args->fp);

  mtx_csj_pkt_cls(&pkt);
  fclose(ff);

  return args;
}

static void big_tear_down(void* data) {
  struct args* args = (struct args*) data;

  mtx_csj_cls(args->mp);
  vec_cls(args->fp);

  fclose(args->rep);
  free(args);
}

static MunitResult test_big_bcg(const MunitParameter[], void* data) {
  struct args* args = (struct args*) data;

  iss_csj_bcg_slv(args->mp, args->xp, args->fp, &args->pps, &args->res, NULL);
  fprintf(args->rep, "bcg: %d, %.7e\n", args->res.k, args->res.res);

  return MUNIT_OK;
}

static MunitResult test_big_bcg_ilu(const MunitParameter[], void* data) {
  struct args* args = (struct args*) data;

  struct mtx_csj* con = mtx_csj_new(args->mp->pps.n, args->mp->pps.ne);

  mtx_csj_ilu(args->mp, con);
  iss_csj_bcg_con_slv(args->mp, con, args->xp, args->fp, &args->pps, &args->res, NULL);
  fprintf(args->rep, "bcg_ilu: %d, %.7e\n", args->res.k, args->res.res);

  mtx_csj_cls(con);

  return MUNIT_OK;
}

static MunitResult test_big_bcg_dgl(const MunitParameter[], void* data) {
  struct args* args = (struct args*) data;

  struct mtx_csj* con = mtx_csj_new(args->mp->pps.n, 0);

  mtx_csj_dgl(args->mp, con);
  iss_csj_bcg_con_slv(args->mp, con, args->xp, args->fp, &args->pps, &args->res, NULL);
  fprintf(args->rep, "bcg_dgl: %d, %.7e\n", args->res.k, args->res.res);

  mtx_csj_cls(con);

  return MUNIT_OK;
}

static MunitTest big_tests[] = {
  { "/bcg", test_big_bcg, big_setup, big_tear_down, MUNIT_TEST_OPTION_NONE, NULL },
  { "/bcg_ilu", test_big_bcg_ilu, big_setup, big_tear_down, MUNIT_TEST_OPTION_NONE, NULL },
  { "/bcg_dgl", test_big_bcg_dgl, big_setup, big_tear_down, MUNIT_TEST_OPTION_NONE, NULL },
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static MunitSuite suites[] = {
  { "/ddm", ddm_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE },
  { "/hlb", hlb_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE },
  { "/big", big_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE },
  { NULL, NULL, NULL, 1, MUNIT_SUITE_OPTION_NONE },
};

static const MunitSuite suite = {"vec/iss/csj", NULL, suites, 1, MUNIT_SUITE_OPTION_NONE};

int main(int argc, char** argv) {
  return munit_suite_main(&suite, NULL, argc, argv);
}
