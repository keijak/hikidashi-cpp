#include <bits/stdc++.h>
#include "../src/wavelet_matrix.hpp"
#include "gtest/gtest.h"

using namespace std;

struct WaveletMatrixTest : public ::testing::Test {
  vector<unsigned> vec;
  WaveletMatrix<unsigned> wm;

  WaveletMatrixTest() {
    vec = {1, 3, 5, 7, 3, 2, 4, 1, 2, 3, 4};
    wm = WaveletMatrix<unsigned>(vec);
  }
};

TEST_F(WaveletMatrixTest, Basic) {
  EXPECT_EQ(wm.size(), vec.size());
  for (int i = 0; i < int(vec.size()); ++i) {
    EXPECT_EQ(wm[i], vec[i]);
  }
}

TEST_F(WaveletMatrixTest, Rank) {
  EXPECT_EQ(wm.rank(3, 1), 0);
  EXPECT_EQ(wm.rank(3, 2), 1);
  EXPECT_EQ(wm.rank(3, 3), 1);
  EXPECT_EQ(wm.rank(3, 5), 2);
  EXPECT_EQ(wm.rank(3, 10), 3);
  EXPECT_EQ(wm.rank(3, wm.size()), 3);
}

TEST_F(WaveletMatrixTest, KthSmallest) {
  EXPECT_EQ(wm.kth_smallest(0, wm.size(), 0), 1);
  EXPECT_EQ(wm.kth_smallest(1, 7, 0), 2);
  EXPECT_EQ(wm.kth_smallest(1, 7, 1), 3);
  EXPECT_EQ(wm.kth_smallest(1, 7, 2), 3);
  EXPECT_EQ(wm.kth_smallest(1, 7, 3), 4);
  EXPECT_EQ(wm.kth_smallest(1, 7, 4), 5);
  EXPECT_EQ(wm.kth_smallest(10, 11, 0), 4);
}

TEST_F(WaveletMatrixTest, KthLargest) {
  EXPECT_EQ(wm.kth_largest(0, wm.size(), 0), 7);
}
