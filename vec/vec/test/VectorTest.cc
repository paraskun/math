#include <gtest/gtest.h>

extern "C" {
#include <math.h>
#include <vec/vec.h>
}

TEST(Vector, GenerateSequence) {
  struct vec* vp = vec_new(10000);

  vec_seq(vp, 1);

  for (int i = 0; i < vp->n; ++i)
    EXPECT_DOUBLE_EQ(i + 1, vp->vp[i]);

  vec_cls(vp);
}

TEST(Vector, CombineWithPositiveCoefficient) {
  struct vec* ap = vec_new(3);
  struct vec* bp = vec_new(3);

  double k = 0.1;

  ap->vp[0] = 1;
  ap->vp[1] = 2;
  ap->vp[2] = -3;

  bp->vp[0] = 4;
  bp->vp[1] = -5;
  bp->vp[2] = 6;

  vec_cmb(ap, bp, ap, k);

  EXPECT_DOUBLE_EQ(1.4, ap->vp[0]);
  EXPECT_DOUBLE_EQ(1.5, ap->vp[1]);
  EXPECT_DOUBLE_EQ(-2.4, ap->vp[2]);

  vec_cls(ap);
  vec_cls(bp);
}

TEST(Vector, CombineWithNegativeCoefficient) {
  struct vec* ap = vec_new(3);
  struct vec* bp = vec_new(3);

  double k = -0.1;

  ap->vp[0] = 1;
  ap->vp[1] = 2;
  ap->vp[2] = -3;

  bp->vp[0] = 4;
  bp->vp[1] = -5;
  bp->vp[2] = 6;

  vec_cmb(ap, bp, ap, k);

  EXPECT_DOUBLE_EQ(0.6, ap->vp[0]);
  EXPECT_DOUBLE_EQ(2.5, ap->vp[1]);
  EXPECT_DOUBLE_EQ(-3.6, ap->vp[2]);

  vec_cls(ap);
  vec_cls(bp);
}

TEST(Vector, Multiply) {
  struct vec* ap = vec_new(3);
  struct vec* bp = vec_new(3);

  double r;

  ap->vp[0] = 1;
  ap->vp[1] = 2;
  ap->vp[2] = -3;

  bp->vp[0] = 4;
  bp->vp[1] = -5;
  bp->vp[2] = 6;

  vec_mlt(ap, bp, &r);

  EXPECT_DOUBLE_EQ(-24.0, r);

  vec_cls(ap);
  vec_cls(bp);
}

TEST(Vector, Norm) {
  struct vec* ap = vec_new(3);

  double r;

  ap->vp[0] = 1;
  ap->vp[1] = 2;
  ap->vp[2] = -3;

  vec_nrm(ap, &r);

  EXPECT_DOUBLE_EQ(sqrt(14.0), r);

  vec_cls(ap);
}

TEST(Vector, Copy) {
  struct vec* ap = vec_new(3);
  struct vec* bp = vec_new(3);

  ap->vp[0] = 1;
  ap->vp[1] = 2;
  ap->vp[2] = -3;

  vec_cpy(ap, bp);

  EXPECT_DOUBLE_EQ(1.0, bp->vp[0]);
  EXPECT_DOUBLE_EQ(2.0, bp->vp[1]);
  EXPECT_DOUBLE_EQ(-3.0, bp->vp[2]);

  vec_cls(ap);
  vec_cls(bp);
}

TEST(Vector, Zero) {
  struct vec* ap = vec_new(3);

  ap->vp[0] = 1;
  ap->vp[1] = 2;
  ap->vp[2] = -3;

  vec_zer(ap);

  EXPECT_DOUBLE_EQ(0.0, ap->vp[0]);
  EXPECT_DOUBLE_EQ(0.0, ap->vp[1]);
  EXPECT_DOUBLE_EQ(0.0, ap->vp[2]);

  vec_cls(ap);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
