#include <bits/stdc++.h>

#include "../src/rational.hpp"
#include "gtest/gtest.h"

using namespace std;

TEST(RationalTest, Zero) {
  Rat r;
  EXPECT_EQ(r.numerator(), 0);
  EXPECT_EQ((r * 100).numerator(), 0);
  EXPECT_EQ(r, Rat(0));
}

TEST(RationalTest, Construct) {
  Rat r(3);
  EXPECT_EQ(r.numerator(), 3);
  EXPECT_EQ(r.denominator(), 1);
}

TEST(RationalTest, LazyReduction) {
  Rat r(60, 70);
  EXPECT_EQ(r.nume_, 60);
  EXPECT_EQ(r.deno_, 70);
  EXPECT_EQ(r.numerator(), 6);
  EXPECT_EQ(r.denominator(), 7);
}

TEST(RationalTest, Arithmetic) {
  EXPECT_EQ(Rat(6, 10) + Rat(3, 10), Rat(9, 10));
  EXPECT_EQ(Rat(6, 10) - Rat(2, 10), Rat(2, 5));
  EXPECT_EQ(Rat(29, 13) * Rat(26, 29), Rat(2, 1));
  EXPECT_EQ(Rat(29, 13) / Rat(29, 26), Rat(2, 1));
}
