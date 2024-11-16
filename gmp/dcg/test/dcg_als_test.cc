#include <gtest/gtest.h>

extern "C" {
#include <dcg_als.h>
}

TEST(dcg_sll, ins_end_exp) {
  struct dcg_sll* l = dcg_sll_new();

  dcg_sll_ins(l, &l->end, 1, 1);
  dcg_sll_ins(l, &l->end, 2, 2);
  dcg_sll_ins(l, &l->end, 3, 3);

  EXPECT_EQ(3, l->s);
  EXPECT_EQ(3, l->end->vtx);

  struct dcg_edg* e = l->beg;

  for (int i = 0; i < 3; ++i) {
    EXPECT_EQ(i + 1, e->vtx);
    EXPECT_EQ(i + 1, e->wgt);

    e = e->next;
  }

  dcg_sll_cls(l);
}

TEST(dcg_sll, ins_end_imp) {
  struct dcg_sll* l = dcg_sll_new();
  struct dcg_edg* e = NULL;

  dcg_sll_ins(l, &e, 1, 1);
  dcg_sll_ins(l, &e, 2, 2);
  dcg_sll_ins(l, &e, 3, 3);

  EXPECT_EQ(3, l->s);
  EXPECT_EQ(3, l->end->vtx);

  e = l->beg;

  for (int i = 0; i < 3; ++i) {
    EXPECT_EQ(i + 1, e->vtx);
    EXPECT_EQ(i + 1, e->wgt);

    e = e->next;
  }

  dcg_sll_cls(l);
}

TEST(dcg_sll, ins_btw) {
  struct dcg_sll* l = dcg_sll_new();
  struct dcg_edg* e = NULL;

  dcg_sll_ins(l, &e, 1, 1);
  dcg_sll_ins(l, &l->end, 3, 3);
  dcg_sll_ins(l, &e, 2, 2);

  EXPECT_EQ(3, l->s);
  EXPECT_EQ(3, l->end->vtx);

  e = l->beg;

  for (int i = 0; i < 3; ++i) {
    EXPECT_EQ(i + 1, e->vtx);
    EXPECT_EQ(i + 1, e->wgt);

    e = e->next;
  }

  dcg_sll_cls(l);
}

TEST(dcg_sll, ins_beg) {
  struct dcg_sll* l = dcg_sll_new();
  struct dcg_edg* e = NULL;

  dcg_sll_ins(l, &e, 3, 3);
  e = NULL;
  dcg_sll_ins(l, &e, 2, 2);
  e = NULL;
  dcg_sll_ins(l, &e, 1, 1);

  EXPECT_EQ(3, l->s);
  EXPECT_EQ(3, l->end->vtx);

  e = l->beg;

  for (int i = 0; i < 3; ++i) {
    EXPECT_EQ(i + 1, e->vtx);
    EXPECT_EQ(i + 1, e->wgt);

    e = e->next;
  }

  dcg_sll_cls(l);
}

TEST(dcg_sll, add) {
  struct dcg_sll* l = dcg_sll_new();

  dcg_sll_add(l, 3, 3);
  dcg_sll_add(l, 1, 1);
  dcg_sll_add(l, 2, 2);

  EXPECT_EQ(3, l->s);
  EXPECT_EQ(3, l->end->vtx);

  struct dcg_edg* e = l->beg;

  for (int i = 0; i < 3; ++i) {
    EXPECT_EQ(i + 1, e->vtx);
    EXPECT_EQ(i + 1, e->wgt);

    e = e->next;
  }

  dcg_sll_cls(l);
}

TEST(dcg_als, fwp_cless) {
  FILE* rep = fopen("report/fwp_smpl.rep", "w+");

  struct dcg_als* g = dcg_als_new(4);

  dcg_als_add(g, 0, 1, 1);
  dcg_als_add(g, 0, 2, 6);
  dcg_als_add(g, 1, 2, 4);
  dcg_als_add(g, 1, 3, 1);
  dcg_als_add(g, 3, 2, 1);

  dcg_als_fwp(g, 1);
  dcg_als_map(rep, g);

  dcg_als_cls(g);

  fclose(rep);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
