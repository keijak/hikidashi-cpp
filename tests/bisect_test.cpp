#include <bits/stdc++.h>
#include "../src/bisect.hpp"
#include "gtest/gtest.h"

using namespace std;

TEST(MidpointTest, Basic) {
  EXPECT_EQ(midpoint(5, 9), 7);
  EXPECT_EQ(midpoint(-5, -9), -7);
  EXPECT_EQ(midpoint(5, 8), 6);
  EXPECT_EQ(midpoint(-5, -8), -6);
  EXPECT_EQ(midpoint(-5, 7), 1);
  EXPECT_EQ(midpoint(-5, -6), -5);
  EXPECT_EQ(midpoint(-3, -4), -3);
}

TEST(MidpointTest, NoOverflow) {
  constexpr int kMax = std::numeric_limits<int>::max();
  constexpr int kMin = std::numeric_limits<int>::lowest();
  EXPECT_EQ(midpoint<int>(kMax, kMax), kMax);
  EXPECT_EQ(midpoint<int>(kMax, kMax - 2), kMax - 1);
  EXPECT_EQ(midpoint<int>(kMin, kMin), kMin);
  EXPECT_EQ(midpoint<int>(kMin, kMin + 2), kMin + 1);
}

TEST(MidpointTest, Short) {
  EXPECT_EQ(midpoint((short) -3, (short) -4), -3);
  EXPECT_EQ(midpoint((short) -4, (short) -3), -4);
  EXPECT_EQ(midpoint((short) -3, (short) 4), 0);
  EXPECT_EQ(midpoint((short) 4, (short) -3), 1);
}
