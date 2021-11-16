#include <bits/stdc++.h>
#include "compress.hpp"
#include "gtest/gtest.h"

using namespace std;

TEST(CompressedTest, Test) {
  vector<int> v = {3, 1, 5, 1, 3, 1, 1, 5, 5, 2};
  Compressed<int> cv(v);
  EXPECT_EQ(cv.size(), 4);
  EXPECT_EQ(cv.values, (vector{1, 2, 3, 5}));

  EXPECT_EQ(cv.value(2), 3);
  EXPECT_EQ(cv.index(3), 2);
  EXPECT_EQ(cv.lower_index(3), 2);

  EXPECT_EQ(cv.index(4), 3);
  EXPECT_EQ(cv.lower_index(4), 2);

  EXPECT_TRUE(cv.contains(2));
  EXPECT_FALSE(cv.contains(4));
}
