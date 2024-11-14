#include <gtest/gtest.h>

extern "C" {
#include <iss_csj.h>

#include <math.h>
#include <stdio.h>
#include <time.h>
}

#define ALL(n) ((1 + (n - 1)) / 2 * (n - 1))

class Env : public testing::Environment {
 public:
  static struct mtx_csj_pps ddm_pps;
  static struct mtx_csj_pps big_sym_pps;
  static struct mtx_csj_pps big_asm_pps;

  static struct mtx_csj_pkt ddm_pos_pkt;
  static struct mtx_csj_pkt ddm_neg_pkt;
  static struct mtx_csj_pkt big_sym_pkt;
  static struct mtx_csj_pkt big_asm_pkt;

  static struct iss_pkt iss_big_sym_pkt;
  static struct iss_pkt iss_big_asm_pkt;

  virtual void SetUp() {
    struct mtx_csj* mp = mtx_csj_new(ddm_pps);

    mtx_csj_all(mp);

    mtx_csj_ddm(mp, 1);
    mtx_csj_put(&ddm_pos_pkt, mp);

    mtx_csj_ddm(mp, -1);
    mtx_csj_put(&ddm_neg_pkt, mp);

    mtx_csj_free(mp);
  }

  virtual void TearDown() {
    mtx_csj_pkt_close(&ddm_pos_pkt);
    mtx_csj_pkt_close(&ddm_neg_pkt);

    mtx_csj_pkt_close(&big_sym_pkt);
    mtx_csj_pkt_close(&big_asm_pkt);

    iss_pkt_close(&iss_big_sym_pkt);
    iss_pkt_close(&iss_big_asm_pkt);
  }
};

struct mtx_csj_pps Env::ddm_pps = {10, ALL(10), ALL(10)};
struct mtx_csj_pps Env::big_sym_pps = {4545, 46640, 46640};
struct mtx_csj_pps Env::big_asm_pps = {945, 9440, 9440};

struct mtx_csj_pkt Env::ddm_pos_pkt {
  .pps = 0, .lr = fopen("mtx/ddm-pos/lr.csj.mtx", "w+"),
  .ur = fopen("mtx/ddm-pos/ur.csj.mtx", "w+"),
  .dr = fopen("mtx/ddm-pos/dr.csj.mtx", "w+"),
  .il = fopen("mtx/ddm-pos/il.csj.mtx", "w+"),
  .jl = fopen("mtx/ddm-pos/jl.csj.mtx", "w+"),
  .iu = fopen("mtx/ddm-pos/iu.csj.mtx", "w+"),
  .ju = fopen("mtx/ddm-pos/ju.csj.mtx", "w+"),
};

struct mtx_csj_pkt Env::ddm_neg_pkt {
  .pps = 0, .lr = fopen("mtx/ddm-neg/lr.csj.mtx", "w+"),
  .ur = fopen("mtx/ddm-neg/ur.csj.mtx", "w+"),
  .dr = fopen("mtx/ddm-neg/dr.csj.mtx", "w+"),
  .il = fopen("mtx/ddm-neg/il.csj.mtx", "w+"),
  .jl = fopen("mtx/ddm-neg/jl.csj.mtx", "w+"),
  .iu = fopen("mtx/ddm-neg/iu.csj.mtx", "w+"),
  .ju = fopen("mtx/ddm-neg/ju.csj.mtx", "w+"),
};

struct mtx_csj_pkt Env::big_sym_pkt {
  .pps = 0, .lr = fopen("mtx/big-sym/lr.csj.mtx", "r"),
  .ur = fopen("mtx/big-sym/ur.csj.mtx", "r"),
  .dr = fopen("mtx/big-sym/dr.csj.mtx", "r"),
  .il = fopen("mtx/big-sym/il.csj.mtx", "r"),
  .jl = fopen("mtx/big-sym/jl.csj.mtx", "r"),
  .iu = fopen("mtx/big-sym/iu.csj.mtx", "r"),
  .ju = fopen("mtx/big-sym/ju.csj.mtx", "r"),
};

struct mtx_csj_pkt Env::big_asm_pkt {
  .pps = 0, .lr = fopen("mtx/big-asm/lr.csj.mtx", "r"),
  .ur = fopen("mtx/big-asm/ur.csj.mtx", "r"),
  .dr = fopen("mtx/big-asm/dr.csj.mtx", "r"),
  .il = fopen("mtx/big-asm/il.csj.mtx", "r"),
  .jl = fopen("mtx/big-asm/jl.csj.mtx", "r"),
  .iu = fopen("mtx/big-asm/iu.csj.mtx", "r"),
  .ju = fopen("mtx/big-asm/ju.csj.mtx", "r"),
};

struct iss_pkt Env::iss_big_sym_pkt {
  .pps = 0, .x = fopen("mtx/big-sym/x.vec", "w+"),
  .f = fopen("mtx/big-sym/f.vec", "r"),
};

struct iss_pkt Env::iss_big_asm_pkt {
  .pps = 0, .x = fopen("mtx/big-asm/x.vec", "w+"),
  .f = fopen("mtx/big-asm/f.vec", "r"),
};

class IssCsjTest : public testing::Test {
 public:
  static const double EPS;
  static const double EPS_X;

  static double diff_ms(struct timespec* beg, struct timespec* end) {
    double sec = end->tv_sec - beg->tv_sec;
    double nan = abs(end->tv_nsec - beg->tv_nsec);

    return sec * 1000 + nan / 1000000;
  }

  void ctx_smp(FILE* rep, fun_iss_csj_slv slv);

  void ctx_hlb(FILE* rep, fun_iss_csj_slv slv);

  void ctx_ddm_pos(FILE* rep, fun_iss_csj_slv slv);

  void ctx_ddm_neg(FILE* rep, fun_iss_csj_slv slv);

  void ctx_big_sym(FILE* rep, fun_iss_csj_slv slv);

  void ctx_big_asm(FILE* rep, fun_iss_csj_slv slv);
};

const double IssCsjTest::EPS = 1e-10;
const double IssCsjTest::EPS_X = 1e-4;

void IssCsjTest::ctx_smp(FILE* rep, fun_iss_csj_slv slv) {
  struct timespec beg;
  struct timespec end;

  struct mtx_csj_pps mpp = {3, 3, 3};
  struct iss_pps spp = {IssCsjTest::EPS, 10000};
  struct iss_res res = {0, 0};

  //  1  2  3
  // -4  5 -6
  //  7  8  9
  struct mtx_csj* mp = mtx_csj_new(mpp);
  struct vec* xp = vec_new(mpp.n);
  struct vec* fp = vec_new(mpp.n);

  mp->dr[0] = 1;
  mp->dr[1] = 5;
  mp->dr[2] = 9;

  mp->lr[0] = -4;
  mp->lr[1] = 7;
  mp->lr[2] = 8;

  mp->ur[0] = 2;
  mp->ur[1] = 3;
  mp->ur[2] = -6;

  mp->il[0] = 0;
  mp->il[1] = 0;
  mp->il[2] = 1;
  mp->il[3] = 3;

  mp->ju[0] = 0;
  mp->ju[1] = 0;
  mp->ju[2] = 1;
  mp->ju[3] = 3;

  mp->jl[0] = 0;
  mp->jl[1] = 0;
  mp->jl[2] = 1;

  mp->iu[0] = 0;
  mp->iu[1] = 0;
  mp->iu[2] = 1;

  double pre;
  double pst;

  vec_seq(xp, 1);
  vec_nrm(xp, &pre);

  mtx_csj_vmlt(mp, xp, fp);

  vec_cls(xp);

  clock_gettime(CLOCK_MONOTONIC, &beg);
  slv(mp, xp, fp, &spp, &res, 0);
  clock_gettime(CLOCK_MONOTONIC, &end);

  vec_nrm(xp, &pst);

  EXPECT_GT(spp.eps, fabs(res.res));
  EXPECT_GT(spp.mk, res.k);
  EXPECT_NEAR(pre, pst, IssCsjTest::EPS_X);

  fprintf(rep, "Iterations: %d\nResidual: %.7e\nTime: %lf ms", res.k, res.res,
          IssCsjTest::diff_ms(&beg, &end));

  mtx_csj_free(mp);
  vec_free(xp);
  vec_free(fp);
}

void IssCsjTest::ctx_hlb(FILE* rep, fun_iss_csj_slv slv) {
  struct timespec beg;
  struct timespec end;

  struct iss_pps spp = {EPS, 10000};

  for (int n = 3; n <= 10; ++n) {
    struct mtx_csj_pps mpp = {n, ALL(n), ALL(n)};
    struct iss_res res = {0, 0};

    struct mtx_csj* mp = mtx_csj_new(mpp);
    struct vec* xp = vec_new(mpp.n);
    struct vec* fp = vec_new(mpp.n);

    mtx_csj_all(mp);
    mtx_csj_hlb(mp);

    double pre;
    double pst;

    vec_seq(xp, 1);
    vec_nrm(xp, &pre);

    mtx_csj_vmlt(mp, xp, fp);

    vec_cls(xp);

    clock_gettime(CLOCK_MONOTONIC, &beg);
    slv(mp, xp, fp, &spp, &res, 0);
    clock_gettime(CLOCK_MONOTONIC, &end);

    vec_nrm(xp, &pst);

    EXPECT_GT(spp.eps, fabs(res.res));
    EXPECT_GT(spp.mk, res.k);
    EXPECT_NEAR(pre, pst, EPS_X);

    fprintf(rep, "N: %d\nIterations: %d\nResidual: %.7e\nTime: %lf ms\n\n", n,
            res.k, res.res, diff_ms(&beg, &end));

    mtx_csj_free(mp);
    vec_free(xp);
    vec_free(fp);
  }
}

void IssCsjTest::ctx_ddm_pos(FILE* rep, fun_iss_csj_slv slv) {
  struct timespec beg;
  struct timespec end;

  struct iss_pps spp = {EPS, 10000};
  struct iss_res res = {0, 0};

  struct mtx_csj* mp = mtx_csj_new(Env::ddm_pps);
  struct vec* xp = vec_new(mp->pps.n);
  struct vec* fp = vec_new(mp->pps.n);

  mtx_csj_get(&Env::ddm_pos_pkt, mp);

  double pre;
  double pst;

  vec_seq(xp, 1);
  vec_nrm(xp, &pre);

  mtx_csj_vmlt(mp, xp, fp);

  vec_cls(xp);

  clock_gettime(CLOCK_MONOTONIC, &beg);
  slv(mp, xp, fp, &spp, &res, 0);
  clock_gettime(CLOCK_MONOTONIC, &end);

  vec_nrm(xp, &pst);

  EXPECT_GT(spp.eps, fabs(res.res));
  EXPECT_GT(spp.mk, res.k);
  EXPECT_NEAR(pre, pst, EPS_X);

  fprintf(rep, "Iterations: %d\nResidual: %.7e\nTime: %lf ms", res.k, res.res,
          diff_ms(&beg, &end));

  mtx_csj_free(mp);
  vec_free(xp);
  vec_free(fp);
}

void IssCsjTest::ctx_ddm_neg(FILE* rep, fun_iss_csj_slv slv) {
  struct timespec beg;
  struct timespec end;

  struct iss_pps spp = {EPS, 10000};
  struct iss_res res = {0, 0};

  struct mtx_csj* mp = mtx_csj_new(Env::ddm_pps);
  struct vec* xp = vec_new(mp->pps.n);
  struct vec* fp = vec_new(mp->pps.n);

  mtx_csj_get(&Env::ddm_neg_pkt, mp);

  double pre;
  double pst;

  vec_seq(xp, 1);
  vec_nrm(xp, &pre);

  mtx_csj_vmlt(mp, xp, fp);

  vec_cls(xp);

  clock_gettime(CLOCK_MONOTONIC, &beg);
  slv(mp, xp, fp, &spp, &res, 0);
  clock_gettime(CLOCK_MONOTONIC, &end);

  vec_nrm(xp, &pst);

  EXPECT_GT(spp.eps, fabs(res.res));
  EXPECT_GT(spp.mk, res.k);
  EXPECT_NEAR(pre, pst, EPS_X);

  fprintf(rep, "Iterations: %d\nResidual: %.7e\nTime: %lf ms", res.k, res.res,
          diff_ms(&beg, &end));

  mtx_csj_free(mp);
  vec_free(xp);
  vec_free(fp);
}

void IssCsjTest::ctx_big_sym(FILE* rep, fun_iss_csj_slv slv) {
  struct timespec beg;
  struct timespec end;

  struct iss_pps spp = {EPS, 10000};
  struct iss_res res = {0, 0};

  struct mtx_csj* mp = mtx_csj_new(Env::big_sym_pps);
  struct vec* xp = vec_new(mp->pps.n);
  struct vec* fp = vec_new(mp->pps.n);

  mtx_csj_get(&Env::big_sym_pkt, mp);
  vec_get(Env::iss_big_sym_pkt.f, fp);

  vec_cls(xp);

  clock_gettime(CLOCK_MONOTONIC, &beg);
  slv(mp, xp, fp, &spp, &res, 0);
  clock_gettime(CLOCK_MONOTONIC, &end);

  EXPECT_GT(spp.eps, fabs(res.res));
  EXPECT_GT(spp.mk, res.k);

  fprintf(rep, "Iterations: %d\nResidual: %.7e\nTime: %lf ms", res.k, res.res,
          diff_ms(&beg, &end));

  vec_put(Env::iss_big_sym_pkt.x, xp);

  mtx_csj_free(mp);
  vec_free(xp);
  vec_free(fp);
}

void IssCsjTest::ctx_big_asm(FILE* rep, fun_iss_csj_slv slv) {
  struct timespec beg;
  struct timespec end;

  struct iss_pps spp = {EPS, 10000};
  struct iss_res res = {0, 0};

  struct mtx_csj* mp = mtx_csj_new(Env::big_asm_pps);
  struct vec* xp = vec_new(mp->pps.n);
  struct vec* fp = vec_new(mp->pps.n);

  mtx_csj_get(&Env::big_asm_pkt, mp);
  vec_get(Env::iss_big_asm_pkt.f, fp);

  vec_cls(xp);

  clock_gettime(CLOCK_MONOTONIC, &beg);
  slv(mp, xp, fp, &spp, &res, 0);
  clock_gettime(CLOCK_MONOTONIC, &end);

  EXPECT_GT(spp.eps, fabs(res.res));
  EXPECT_GT(spp.mk, res.k);

  fprintf(rep, "Iterations: %d\nResidual: %.7e\nTime: %lf ms", res.k, res.res,
          diff_ms(&beg, &end));

  vec_put(Env::iss_big_asm_pkt.x, xp);

  mtx_csj_free(mp);
  vec_free(xp);
  vec_free(fp);
}

TEST_F(IssCsjTest, los_dft_smp) {
  FILE* rep = fopen("report/los_dft_smp.rep", "w+");

  ctx_smp(rep, &iss_csj_los_slv);

  fclose(rep);
}

TEST_F(IssCsjTest, los_dft_ddm_pos) {
  FILE* rep = fopen("report/los_dft_ddm_pos.rep", "w+");

  ctx_ddm_pos(rep, &iss_csj_los_slv);

  fclose(rep);
}

TEST_F(IssCsjTest, los_dft_ddm_neg) {
  FILE* rep = fopen("report/los_dft_ddm_neg.rep", "w+");

  ctx_ddm_neg(rep, &iss_csj_los_slv);

  fclose(rep);
}

TEST_F(IssCsjTest, los_dft_hlb) {
  FILE* rep = fopen("report/los_dft_hlb.rep", "w+");

  ctx_hlb(rep, &iss_csj_los_slv);

  fclose(rep);
}

TEST_F(IssCsjTest, los_dft_big_sym) {
  FILE* rep = fopen("report/los_dft_big_sym.rep", "w+");

  ctx_big_sym(rep, &iss_csj_los_slv);

  fclose(rep);
}

TEST_F(IssCsjTest, los_dft_big_asm) {
  FILE* rep = fopen("report/los_dft_big_asm.rep", "w+");

  ctx_big_asm(rep, &iss_csj_los_slv);

  fclose(rep);
}

TEST_F(IssCsjTest, los_ilu_smp) {
  FILE* rep = fopen("report/los_ilu_smp.rep", "w+");

  ctx_smp(rep, &iss_csj_ilu_los_slv);

  fclose(rep);
}

TEST_F(IssCsjTest, los_ilu_ddm_pos) {
  FILE* rep = fopen("report/los_ilu_ddm_pos.rep", "w+");

  ctx_ddm_pos(rep, &iss_csj_ilu_los_slv);

  fclose(rep);
}

TEST_F(IssCsjTest, los_ilu_ddm_neg) {
  FILE* rep = fopen("report/los_ilu_ddm_neg.rep", "w+");

  ctx_ddm_neg(rep, &iss_csj_ilu_los_slv);

  fclose(rep);
}

TEST_F(IssCsjTest, los_ilu_hlb) {
  FILE* rep = fopen("report/los_ilu_hlb.rep", "w+");

  ctx_hlb(rep, &iss_csj_ilu_los_slv);

  fclose(rep);
}

TEST_F(IssCsjTest, los_ilu_big_sym) {
  FILE* rep = fopen("report/los_ilu_big_sym.rep", "w+");

  ctx_big_sym(rep, &iss_csj_ilu_los_slv);

  fclose(rep);
}

TEST_F(IssCsjTest, los_ilu_big_asm) {
  FILE* rep = fopen("report/los_ilu_big_asm.rep", "w+");

  ctx_big_asm(rep, &iss_csj_ilu_los_slv);

  fclose(rep);
}

TEST_F(IssCsjTest, los_dgl_smp) {
  FILE* rep = fopen("report/los_dgl_smp.rep", "w+");

  ctx_smp(rep, &iss_csj_dgl_los_slv);

  fclose(rep);
}

TEST_F(IssCsjTest, los_dgl_ddm_pos) {
  FILE* rep = fopen("report/los_dgl_ddm_pos.rep", "w+");

  ctx_ddm_pos(rep, &iss_csj_dgl_los_slv);

  fclose(rep);
}

TEST_F(IssCsjTest, los_dgl_ddm_neg) {
  FILE* rep = fopen("report/los_dgl_ddm_neg.rep", "w+");

  ctx_ddm_neg(rep, &iss_csj_dgl_los_slv);

  fclose(rep);
}

TEST_F(IssCsjTest, los_dgl_hlb) {
  FILE* rep = fopen("report/los_dgl_hlb.rep", "w+");

  ctx_hlb(rep, &iss_csj_dgl_los_slv);

  fclose(rep);
}

TEST_F(IssCsjTest, los_dgl_big_sym) {
  FILE* rep = fopen("report/los_dgl_big_sym.rep", "w+");

  ctx_big_sym(rep, &iss_csj_dgl_los_slv);

  fclose(rep);
}

TEST_F(IssCsjTest, los_dgl_big_asm) {
  FILE* rep = fopen("report/los_dgl_big_asm.rep", "w+");

  ctx_big_asm(rep, &iss_csj_dgl_los_slv);

  fclose(rep);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  testing::AddGlobalTestEnvironment(new Env);

  return RUN_ALL_TESTS();
}
