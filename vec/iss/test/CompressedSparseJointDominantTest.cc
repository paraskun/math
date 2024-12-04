#include <gtest/gtest.h>

extern "C" {
#include <vec/iss_csj.h>

#include <math.h>
#include <stdio.h>
#include <time.h>
}

class Env : public testing::Environment {
 public:
  static struct mtx_csj* mp;
  static struct vec* fp;

  static double xn;

  virtual void SetUp() {
    mp = mtx_csj_new(10, 17);
    fp = vec_new(10);

    struct mtx_csj_pkt pkt = {
      .pps = 0,
      .dr = fopen("mtx/ddm/dr.csj.mtx", "r"),
      .lr = fopen("mtx/ddm/lr.csj.mtx", "r"),
      .ur = fopen("mtx/ddm/ur.csj.mtx", "r"),
      .ia = fopen("mtx/ddm/ia.csj.mtx", "r"),
      .ja = fopen("mtx/ddm/ja.csj.mtx", "r"),
    };

    struct vec* xp = vec_new(10);

    vec_seq(xp, 1);
    vec_nrm(xp, &xn);

    mtx_csj_fget(&pkt, mp);
    mtx_csj_vmlt(mp, xp, fp);

    vec_cls(xp);
    mtx_csj_pkt_cls(&pkt);
  }

  virtual void TearDown() {
    mtx_csj_cls(mp);
    vec_cls(fp);
  }
};

struct mtx_csj* Env::mp;
struct vec* Env::fp;
double Env::xn;

class CompressedSparseJointDominantTest : public testing::Test {
 public:
  static struct iss_pps pps;

  static double diff_ms(struct timespec* beg, struct timespec* end) {
    double sec = end->tv_sec - beg->tv_sec;
    double nan = abs(end->tv_nsec - beg->tv_nsec);

    return sec * 1000 + nan / 1000000;
  }
};

struct iss_pps CompressedSparseJointDominantTest::pps = {1e-15, 200000};

TEST_F(CompressedSparseJointDominantTest, BiConjGradTest) {
  FILE* rep = fopen("out/bcg_ddm.rep", "w+");

  struct timespec beg;
  struct timespec end;

  struct iss_res res = {0, 0};
  struct vec* xp = vec_new(10);

  clock_gettime(CLOCK_MONOTONIC, &beg);
  iss_csj_bcg_slv(Env::mp, xp, Env::fp, &pps, &res, 0);
  clock_gettime(CLOCK_MONOTONIC, &end);

  EXPECT_GT(pps.eps, fabs(res.res));
  EXPECT_GT(pps.mk, res.k);

  double nrm;

  vec_nrm(xp, &nrm);

  fprintf(rep, "N: %d\n", res.k);
  fprintf(rep, "Residual: %.7e\n", res.res);
  fprintf(rep, "Target norm: %.7e\n", Env::xn);
  fprintf(rep, "Obtained norm: %.7e\n", nrm);
  fprintf(rep, "Norm difference: %.7e\n", fabs(nrm - Env::xn));
  fprintf(rep, "Time: %lf\n\n", diff_ms(&beg, &end));

  vec_put(rep, xp);
  vec_cls(xp);

  fclose(rep);
}

TEST_F(CompressedSparseJointDominantTest, BiConjGradILUTest) {
  FILE* rep = fopen("out/bcg_ilu_ddm.rep", "w+");

  struct timespec beg;
  struct timespec end;

  struct iss_res res = {0, 0};
  struct vec* xp = vec_new(10);
  struct mtx_csj* con = mtx_csj_new(Env::mp->pps.n, Env::mp->pps.ne);

  mtx_csj_ilu(Env::mp, con);

  clock_gettime(CLOCK_MONOTONIC, &beg);
  iss_csj_bcg_con_slv(Env::mp, con, xp, Env::fp, &pps, &res, 0);
  clock_gettime(CLOCK_MONOTONIC, &end);

  EXPECT_GT(pps.eps, fabs(res.res));
  EXPECT_GT(pps.mk, res.k);

  double nrm;

  vec_nrm(xp, &nrm);

  fprintf(rep, "N: %d\n", res.k);
  fprintf(rep, "Residual: %.7e\n", res.res);
  fprintf(rep, "Target norm: %.7e\n", Env::xn);
  fprintf(rep, "Obtained norm: %.7e\n", nrm);
  fprintf(rep, "Norm difference: %.7e\n", fabs(nrm - Env::xn));
  fprintf(rep, "Time: %lf\n\n", diff_ms(&beg, &end));

  vec_put(rep, xp);
  vec_cls(xp);
  mtx_csj_cls(con);

  fclose(rep);
}

TEST_F(CompressedSparseJointDominantTest, BiConjGradDiagTest) {
  FILE* rep = fopen("out/bcg_dgl_ddm.rep", "w+");

  struct timespec beg;
  struct timespec end;

  struct iss_res res = {0, 0};
  struct vec* xp = vec_new(Env::mp->pps.n);
  struct mtx_csj* con = mtx_csj_new(Env::mp->pps.n, 0);

  mtx_csj_dgl(Env::mp, con);

  clock_gettime(CLOCK_MONOTONIC, &beg);
  iss_csj_bcg_con_slv(Env::mp, con, xp, Env::fp, &pps, &res, 0);
  clock_gettime(CLOCK_MONOTONIC, &end);

  EXPECT_GT(pps.eps, fabs(res.res));
  EXPECT_GT(pps.mk, res.k);

  double nrm;

  vec_nrm(xp, &nrm);

  fprintf(rep, "N: %d\n", res.k);
  fprintf(rep, "Residual: %.7e\n", res.res);
  fprintf(rep, "Target norm: %.7e\n", Env::xn);
  fprintf(rep, "Obtained norm: %.7e\n", nrm);
  fprintf(rep, "Norm difference: %.7e\n", fabs(nrm - Env::xn));
  fprintf(rep, "Time: %lf\n\n", diff_ms(&beg, &end));

  vec_put(rep, xp);
  vec_cls(xp);
  mtx_csj_cls(con);

  fclose(rep);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  testing::AddGlobalTestEnvironment(new Env);

  return RUN_ALL_TESTS();
}
