#include <bits/stdc++.h>
#include <tuple>

#include "../src/geometry_int.hpp"
#include "gtest/gtest.h"

using namespace std;

TEST(Point2dTest, Basic) {
  Point2d<Int> p0;
  EXPECT_EQ(p0.x, 0);
  EXPECT_EQ(p0.y, 0);

  Point2d<Int> p1(1, 2), p2(-2, 5);
  EXPECT_EQ(p1 + p0, p1);
  EXPECT_EQ(p1 + p2, Point2d<Int>(-1, 7));
  EXPECT_EQ(dot(p1, p2), 8);
  EXPECT_EQ(cross(p1, p2), 9);
}

TEST(Point2dTest, ArgLess) {
  vector<P> pts = {
      P(1, 0), P(1, 1), P(0, 1), P(-1, 1), P(-1, 0), P(-1, -1), P(0, -1), P(1, -1)
  };
  for (int i = 0; i < 9; ++i) {
    for (int j = 0; j < 9; ++j) {
      EXPECT_EQ(ArgLess{}(pts[i], pts[j]), i < j);
    }
  }
}
