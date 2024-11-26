#include <gtest/gtest.h>

extern "C" {
#include <iss/iss_csj.h>

#include <math.h>
#include <stdio.h>
#include <time.h>
}

#define ALL(n) ((1 + (n - 1)) * (n - 1) / 2)

class Env : public testing::Environment {
 public:
  static struct mtx_csj_pps hlb_pps;
  static struct mtx_csj_pps ddm_pps;

  static struct mtx_csj_pps ddm_dft_pps;

  static struct mtx_csj_pps big_fst_pps;
  static struct mtx_csj_pps big_snd_pps;

  static struct iss_csj_pkt iss_hlb_pkt;

  static struct iss_csj_pkt iss_ddm_dft_pkt;
  static struct iss_csj_pkt iss_ddm_pos_pkt;
  static struct iss_csj_pkt iss_ddm_neg_pkt;

  static struct iss_csj_pkt iss_big_fst_pkt;
  static struct iss_csj_pkt iss_big_snd_pkt;

  void NewHilbert() {
    struct mtx_csj* mp = mtx_csj_new(hlb_pps);
    struct vec* xp = vec_new(mp->pps.n);
    struct vec* fp = vec_new(mp->pps.n);

    vec_seq(xp, 1);

    mtx_csj_all(mp);
    mtx_csj_hlb(mp);
    mtx_csj_vmlt(mp, xp, fp);
    mtx_csj_put(&iss_hlb_pkt.mtx, mp);

    vec_put(iss_hlb_pkt.pkt.f, fp);

    mtx_csj_cls(mp);
    vec_cls(xp);
    vec_cls(fp);
  }

  void NewDominant() {
    struct mtx_csj* mp = mtx_csj_new(ddm_pps);
    struct vec* xp = vec_new(mp->pps.n);
    struct vec* fp = vec_new(mp->pps.n);

    vec_seq(xp, 1);

    mtx_csj_all(mp);
    mtx_csj_ddm(mp, 1);
    mtx_csj_vmlt(mp, xp, fp);
    mtx_csj_put(&iss_ddm_pos_pkt.mtx, mp);

    vec_put(iss_ddm_pos_pkt.pkt.f, fp);

    for (int i = 1; i < mp->pps.n; ++i) {
      int ar0 = mp->ia[i];
      int ar1 = mp->ia[i + 1];

      for (int ar = ar0; ar < ar1; ++ar) {
        mp->lr[ar] *= -1;
        mp->ur[ar] *= -1;
      }
    }

    mtx_csj_vmlt(mp, xp, fp);
    mtx_csj_put(&iss_ddm_neg_pkt.mtx, mp);

    vec_put(iss_ddm_neg_pkt.pkt.f, fp);

    mtx_csj_cls(mp);
    vec_cls(xp);
    vec_cls(fp);
  }

  void GetRight() {
    struct mtx_csj* mp = mtx_csj_new(ddm_dft_pps);
    struct vec* xp = vec_new(mp->pps.n);
    struct vec* fp = vec_new(mp->pps.n);

    vec_seq(xp, 1);

    mtx_csj_get(&iss_ddm_dft_pkt.mtx, mp);
    mtx_csj_vmlt(mp, xp, fp);

    vec_put(iss_ddm_dft_pkt.pkt.f, fp);

    mtx_csj_cls(mp);
    vec_cls(xp);
    vec_cls(fp);
  }

  virtual void SetUp() {
    // NewHilbert();
    // NewDominant();
    // GetRight();
  }

  virtual void TearDown() {
    iss_csj_pkt_cls(&iss_hlb_pkt);
    iss_csj_pkt_cls(&iss_ddm_pos_pkt);
    iss_csj_pkt_cls(&iss_ddm_neg_pkt);
    iss_csj_pkt_cls(&iss_big_fst_pkt);
    iss_csj_pkt_cls(&iss_big_snd_pkt);
  }
};

struct mtx_csj_pps Env::hlb_pps = {3, ALL(3)};
struct mtx_csj_pps Env::ddm_pps = {10, ALL(10)};
struct mtx_csj_pps Env::ddm_dft_pps = {10, 17};
struct mtx_csj_pps Env::big_fst_pps = {4545, 46640};
struct mtx_csj_pps Env::big_snd_pps = {945, 9440};

struct iss_csj_pkt Env::iss_hlb_pkt {
  .pkt =
      {
          .pps = 0,
          .x = 0,
          .f = fopen("mtx/hlb/f.vec", "r"),
  },
  .mtx {
    .pps = 0, .dr = fopen("mtx/hlb/dr.csj.mtx", "r"),
    .lr = fopen("mtx/hlb/lr.csj.mtx", "r"),
    .ur = fopen("mtx/hlb/ur.csj.mtx", "r"),
    .ia = fopen("mtx/hlb/ia.csj.mtx", "r"),
    .ja = fopen("mtx/hlb/ja.csj.mtx", "r"),
  }
};

struct iss_csj_pkt Env::iss_ddm_dft_pkt {
  .pkt{
      .pps = 0,
      .x = 0,
      .f = fopen("mtx/ddm/f.vec", "r"),
  },
      .mtx {
    .pps = 0, .dr = fopen("mtx/ddm/dr.csj.mtx", "r"),
    .lr = fopen("mtx/ddm/lr.csj.mtx", "r"),
    .ur = fopen("mtx/ddm/ur.csj.mtx", "r"),
    .ia = fopen("mtx/ddm/ia.csj.mtx", "r"),
    .ja = fopen("mtx/ddm/ja.csj.mtx", "r"),
  }
};

struct iss_csj_pkt Env::iss_ddm_pos_pkt {
  .pkt =
      {
          .pps = 0,
          .x = 0,
          .f = fopen("mtx/ddm-pos/f.vec", "r"),
  },
  .mtx {
    .pps = 0, .dr = fopen("mtx/ddm-pos/dr.csj.mtx", "r"),
    .lr = fopen("mtx/ddm-pos/lr.csj.mtx", "r"),
    .ur = fopen("mtx/ddm-pos/ur.csj.mtx", "r"),
    .ia = fopen("mtx/ddm-pos/ia.csj.mtx", "r"),
    .ja = fopen("mtx/ddm-pos/ja.csj.mtx", "r"),
  }
};

struct iss_csj_pkt Env::iss_ddm_neg_pkt {
  .pkt =
      {
          .pps = 0,
          .x = 0,
          .f = fopen("mtx/ddm-neg/f.vec", "r"),
  },
  .mtx {
    .pps = 0, .dr = fopen("mtx/ddm-neg/dr.csj.mtx", "r"),
    .lr = fopen("mtx/ddm-neg/lr.csj.mtx", "r"),
    .ur = fopen("mtx/ddm-neg/ur.csj.mtx", "r"),
    .ia = fopen("mtx/ddm-neg/ia.csj.mtx", "r"),
    .ja = fopen("mtx/ddm-neg/ja.csj.mtx", "r"),
  }
};

struct iss_csj_pkt Env::iss_big_fst_pkt {
  .pkt =
      {
          .pps = 0,
          .x = 0,
          .f = fopen("mtx/big-fst/f.vec", "r"),
  },
  .mtx {
    .pps = 0, .dr = fopen("mtx/big-fst/dr.csj.mtx", "r"),
    .lr = fopen("mtx/big-fst/lr.csj.mtx", "r"),
    .ur = fopen("mtx/big-fst/ur.csj.mtx", "r"),
    .ia = fopen("mtx/big-fst/ia.csj.mtx", "r"),
    .ja = fopen("mtx/big-fst/ja.csj.mtx", "r"),
  }
};

struct iss_csj_pkt Env::iss_big_snd_pkt {
  .pkt =
      {
          .pps = 0,
          .x = 0,
          .f = fopen("mtx/big-snd/f.vec", "r"),
  },
  .mtx {
    .pps = 0, .dr = fopen("mtx/big-snd/dr.csj.mtx", "r"),
    .lr = fopen("mtx/big-snd/lr.csj.mtx", "r"),
    .ur = fopen("mtx/big-snd/ur.csj.mtx", "r"),
    .ia = fopen("mtx/big-snd/ia.csj.mtx", "r"),
    .ja = fopen("mtx/big-snd/ja.csj.mtx", "r"),
  }
};

class iss_csj_test : public testing::Test {
 public:
  static struct iss_pps pps;

  static double diff_ms(struct timespec* beg, struct timespec* end) {
    double sec = end->tv_sec - beg->tv_sec;
    double nan = abs(end->tv_nsec - beg->tv_nsec);

    return sec * 1000 + nan / 1000000;
  }

  void test_ctx_hlb_dft(FILE* rep, fun_iss_csj_slv slv);
  void test_ctx_hlb_all(FILE* rep, fun_iss_csj_slv slv);
  void test_ctx_ddm_dft(FILE* rep, fun_iss_csj_slv slv);
  void test_ctx_ddm_pos(FILE* rep, fun_iss_csj_slv svl);
  void test_ctx_ddm_neg(FILE* rep, fun_iss_csj_slv slv);
  void test_ctx_big_fst(FILE* rep, fun_iss_csj_slv slv);
  void test_ctx_big_snd(FILE* rep, fun_iss_csj_slv slv);
};

struct iss_pps iss_csj_test::pps = {1e-10, 100000};

void iss_csj_test::test_ctx_hlb_dft(FILE* rep, fun_iss_csj_slv slv) {
  struct timespec beg;
  struct timespec end;

  struct iss_res res = {0, 0};

  struct mtx_csj* mp = mtx_csj_new(Env::hlb_pps);
  struct vec* xp = vec_new(mp->pps.n);
  struct vec* fp = vec_new(mp->pps.n);

  mtx_csj_get(&Env::iss_hlb_pkt.mtx, mp);
  vec_get(Env::iss_hlb_pkt.pkt.f, fp);

  vec_zer(xp);

  clock_gettime(CLOCK_MONOTONIC, &beg);
  slv(mp, xp, fp, &pps, &res, 0);
  clock_gettime(CLOCK_MONOTONIC, &end);

  EXPECT_GT(pps.eps, fabs(res.res));
  EXPECT_GT(pps.mk, res.k);

  fprintf(rep, "Iterations: %d\nResidual: %.7e\nTime: %lf ms\n\n", res.k,
          res.res, diff_ms(&beg, &end));

  vec_put(rep, xp);

  mtx_csj_cls(mp);
  vec_cls(xp);
  vec_cls(fp);
}

void iss_csj_test::test_ctx_hlb_all(FILE* rep, fun_iss_csj_slv slv) {
  struct timespec beg;
  struct timespec end;

  for (int n = 4; n <= 10; ++n) {
    struct mtx_csj_pps mpp = {n, ALL(n)};
    struct iss_res res = {0, 0};

    struct mtx_csj* mp = mtx_csj_new(mpp);
    struct vec* xp = vec_new(mpp.n);
    struct vec* fp = vec_new(mpp.n);

    mtx_csj_all(mp);
    mtx_csj_hlb(mp);

    vec_seq(xp, 1);
    mtx_csj_vmlt(mp, xp, fp);
    vec_zer(xp);

    clock_gettime(CLOCK_MONOTONIC, &beg);
    slv(mp, xp, fp, &pps, &res, 0);
    clock_gettime(CLOCK_MONOTONIC, &end);

    EXPECT_GT(pps.eps, fabs(res.res));
    EXPECT_GT(pps.mk, res.k);

    fprintf(rep, "N: %d\nIterations: %d\nResidual: %.7e\nTime: %lf ms\n\n", n,
            res.k, res.res, diff_ms(&beg, &end));

    mtx_csj_cls(mp);
    vec_cls(xp);
    vec_cls(fp);
  }
}

void iss_csj_test::test_ctx_ddm_dft(FILE* rep, fun_iss_csj_slv slv) {
  struct timespec beg;
  struct timespec end;

  struct iss_res res = {0, 0};

  struct mtx_csj* mp = mtx_csj_new(Env::ddm_dft_pps);
  struct vec* xp = vec_new(mp->pps.n);
  struct vec* fp = vec_new(mp->pps.n);

  mtx_csj_get(&Env::iss_ddm_dft_pkt.mtx, mp);
  vec_get(Env::iss_ddm_dft_pkt.pkt.f, fp);

  vec_zer(xp);

  clock_gettime(CLOCK_MONOTONIC, &beg);
  slv(mp, xp, fp, &pps, &res, 0);
  clock_gettime(CLOCK_MONOTONIC, &end);

  EXPECT_GT(pps.eps, fabs(res.res));
  EXPECT_GT(pps.mk, res.k);

  fprintf(rep, "Iterations: %d\nResidual: %.7e\nTime: %lf ms\n\n", res.k,
          res.res, diff_ms(&beg, &end));

  vec_put(rep, xp);

  mtx_csj_cls(mp);
  vec_cls(xp);
  vec_cls(fp);
}

void iss_csj_test::test_ctx_ddm_pos(FILE* rep, fun_iss_csj_slv slv) {
  struct timespec beg;
  struct timespec end;

  struct iss_res res = {0, 0};

  struct mtx_csj* mp = mtx_csj_new(Env::ddm_pps);
  struct vec* xp = vec_new(mp->pps.n);
  struct vec* fp = vec_new(mp->pps.n);

  mtx_csj_get(&Env::iss_ddm_pos_pkt.mtx, mp);
  vec_get(Env::iss_ddm_pos_pkt.pkt.f, fp);

  vec_zer(xp);

  clock_gettime(CLOCK_MONOTONIC, &beg);
  slv(mp, xp, fp, &pps, &res, 0);
  clock_gettime(CLOCK_MONOTONIC, &end);

  EXPECT_GT(pps.eps, fabs(res.res));
  EXPECT_GT(pps.mk, res.k);

  fprintf(rep, "Iterations: %d\nResidual: %.7e\nTime: %lf ms\n\n", res.k,
          res.res, diff_ms(&beg, &end));

  vec_put(rep, xp);

  mtx_csj_cls(mp);
  vec_cls(xp);
  vec_cls(fp);
}

void iss_csj_test::test_ctx_ddm_neg(FILE* rep, fun_iss_csj_slv slv) {
  struct timespec beg;
  struct timespec end;

  struct iss_res res = {0, 0};

  struct mtx_csj* mp = mtx_csj_new(Env::ddm_pps);
  struct vec* xp = vec_new(mp->pps.n);
  struct vec* fp = vec_new(mp->pps.n);

  mtx_csj_get(&Env::iss_ddm_neg_pkt.mtx, mp);
  vec_get(Env::iss_ddm_neg_pkt.pkt.f, fp);

  vec_zer(xp);

  clock_gettime(CLOCK_MONOTONIC, &beg);
  slv(mp, xp, fp, &pps, &res, 0);
  clock_gettime(CLOCK_MONOTONIC, &end);

  EXPECT_GT(pps.eps, fabs(res.res));
  EXPECT_GT(pps.mk, res.k);

  fprintf(rep, "Iterations: %d\nResidual: %.7e\nTime: %lf ms\n\n", res.k,
          res.res, diff_ms(&beg, &end));

  vec_put(rep, xp);

  mtx_csj_cls(mp);
  vec_cls(xp);
  vec_cls(fp);
}

void iss_csj_test::test_ctx_big_fst(FILE* rep, fun_iss_csj_slv slv) {
  struct timespec beg;
  struct timespec end;

  struct iss_res res = {0, 0};

  struct mtx_csj* mp = mtx_csj_new(Env::big_fst_pps);
  struct vec* xp = vec_new(mp->pps.n);
  struct vec* fp = vec_new(mp->pps.n);

  mtx_csj_get(&Env::iss_big_fst_pkt.mtx, mp);
  vec_get(Env::iss_big_fst_pkt.pkt.f, fp);

  vec_zer(xp);

  clock_gettime(CLOCK_MONOTONIC, &beg);
  slv(mp, xp, fp, &pps, &res, 0);
  clock_gettime(CLOCK_MONOTONIC, &end);

  EXPECT_GT(pps.eps, fabs(res.res));
  EXPECT_GT(pps.mk, res.k);

  fprintf(rep, "Iterations: %d\nResidual: %.7e\nTime: %lf ms\n\n", res.k,
          res.res, diff_ms(&beg, &end));

  vec_put(rep, xp);

  mtx_csj_cls(mp);
  vec_cls(xp);
  vec_cls(fp);
}

void iss_csj_test::test_ctx_big_snd(FILE* rep, fun_iss_csj_slv slv) {
  struct timespec beg;
  struct timespec end;

  struct iss_res res = {0, 0};

  struct mtx_csj* mp = mtx_csj_new(Env::big_snd_pps);
  struct vec* xp = vec_new(mp->pps.n);
  struct vec* fp = vec_new(mp->pps.n);

  mtx_csj_get(&Env::iss_big_snd_pkt.mtx, mp);
  vec_get(Env::iss_big_snd_pkt.pkt.f, fp);

  vec_zer(xp);

  clock_gettime(CLOCK_MONOTONIC, &beg);
  slv(mp, xp, fp, &pps, &res, 0);
  clock_gettime(CLOCK_MONOTONIC, &end);

  EXPECT_GT(pps.eps, fabs(res.res));
  EXPECT_GT(pps.mk, res.k);

  fprintf(rep, "Iterations: %d\nResidual: %.7e\nTime: %lf ms\n\n", res.k,
          res.res, diff_ms(&beg, &end));

  vec_put(rep, xp);

  mtx_csj_cls(mp);
  vec_cls(xp);
  vec_cls(fp);
}

TEST_F(iss_csj_test, los_dft_hlb_dft) {
  FILE* rep = fopen("report/los_dft_hlb_dft.rep", "w+");

  test_ctx_hlb_dft(rep, &iss_csj_los_slv);

  fclose(rep);
}

TEST_F(iss_csj_test, los_dft_hlb_all) {
  FILE* rep = fopen("report/los_dft_hlb_all.rep", "w+");

  test_ctx_hlb_all(rep, &iss_csj_los_slv);

  fclose(rep);
}

TEST_F(iss_csj_test, los_dft_ddm_dft) {
  FILE* rep = fopen("report/los_dft_ddm_dft.rep", "w+");

  test_ctx_ddm_dft(rep, &iss_csj_los_slv);

  fclose(rep);
}

TEST_F(iss_csj_test, los_dft_ddm_pos) {
  FILE* rep = fopen("report/los_dft_ddm_pos.rep", "w+");

  test_ctx_ddm_pos(rep, &iss_csj_los_slv);

  fclose(rep);
}

TEST_F(iss_csj_test, los_dft_ddm_neg) {
  FILE* rep = fopen("report/los_dft_ddm_neg.rep", "w+");

  test_ctx_ddm_neg(rep, &iss_csj_los_slv);

  fclose(rep);
}

TEST_F(iss_csj_test, los_dft_big_fst) {
  FILE* rep = fopen("report/los_dft_big_fst.rep", "w+");

  test_ctx_big_fst(rep, &iss_csj_los_slv);

  fclose(rep);
}

TEST_F(iss_csj_test, los_dft_big_snd) {
  FILE* rep = fopen("report/los_dft_big_snd.rep", "w+");

  test_ctx_big_snd(rep, &iss_csj_los_slv);

  fclose(rep);
}

TEST_F(iss_csj_test, los_ilu_hlb_dft) {
  FILE* rep = fopen("report/los_ilu_hlb_dft.rep", "w+");

  test_ctx_hlb_dft(rep, &iss_csj_ilu_los_slv);

  fclose(rep);
}

TEST_F(iss_csj_test, los_ilu_hlb_all) {
  FILE* rep = fopen("report/los_ilu_hlb_all.rep", "w+");

  test_ctx_hlb_all(rep, &iss_csj_ilu_los_slv);

  fclose(rep);
}

TEST_F(iss_csj_test, los_ilu_ddm_dft) {
  FILE* rep = fopen("report/los_ilu_ddm_dft.rep", "w+");

  test_ctx_ddm_dft(rep, &iss_csj_ilu_los_slv);

  fclose(rep);
}

TEST_F(iss_csj_test, los_ilu_ddm_pos) {
  FILE* rep = fopen("report/los_ilu_ddm_pos.rep", "w+");

  test_ctx_ddm_pos(rep, &iss_csj_ilu_los_slv);

  fclose(rep);
}

TEST_F(iss_csj_test, los_ilu_ddm_neg) {
  FILE* rep = fopen("report/los_ilu_ddm_neg.rep", "w+");

  test_ctx_ddm_neg(rep, &iss_csj_ilu_los_slv);

  fclose(rep);
}

TEST_F(iss_csj_test, los_ilu_big_fst) {
  FILE* rep = fopen("report/los_ilu_big_fst.rep", "w+");

  test_ctx_big_fst(rep, &iss_csj_ilu_los_slv);

  fclose(rep);
}

TEST_F(iss_csj_test, los_ilu_big_snd) {
  FILE* rep = fopen("report/los_ilu_big_snd.rep", "w+");

  test_ctx_big_snd(rep, &iss_csj_ilu_los_slv);

  fclose(rep);
}

TEST_F(iss_csj_test, los_dgl_hlb_dft) {
  FILE* rep = fopen("report/los_dgl_hlb_dft.rep", "w+");

  test_ctx_hlb_dft(rep, &iss_csj_dgl_los_slv);

  fclose(rep);
}

TEST_F(iss_csj_test, los_dgl_hlb_all) {
  FILE* rep = fopen("report/los_dgl_hlb_all.rep", "w+");

  test_ctx_hlb_all(rep, &iss_csj_dgl_los_slv);

  fclose(rep);
}

TEST_F(iss_csj_test, los_dgl_ddm_dft) {
  FILE* rep = fopen("report/los_dgl_ddm_dft.rep", "w+");

  test_ctx_ddm_dft(rep, &iss_csj_dgl_los_slv);

  fclose(rep);
}

TEST_F(iss_csj_test, los_dgl_ddm_pos) {
  FILE* rep = fopen("report/los_dgl_ddm_pos.rep", "w+");

  test_ctx_ddm_pos(rep, &iss_csj_dgl_los_slv);

  fclose(rep);
}

TEST_F(iss_csj_test, los_dgl_ddm_neg) {
  FILE* rep = fopen("report/los_dgl_ddm_neg.rep", "w+");

  test_ctx_ddm_neg(rep, &iss_csj_dgl_los_slv);

  fclose(rep);
}

TEST_F(iss_csj_test, los_dgl_big_fst) {
  FILE* rep = fopen("report/los_dgl_big_fst.rep", "w+");

  test_ctx_big_fst(rep, &iss_csj_dgl_los_slv);

  fclose(rep);
}

TEST_F(iss_csj_test, los_dgl_big_snd) {
  FILE* rep = fopen("report/los_dgl_big_snd.rep", "w+");

  test_ctx_big_snd(rep, &iss_csj_dgl_los_slv);

  fclose(rep);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  testing::AddGlobalTestEnvironment(new Env);

  return RUN_ALL_TESTS();
}
