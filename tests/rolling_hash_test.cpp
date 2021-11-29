#include <bits/stdc++.h>
#include "../src/rolling_hash.hpp"
#include "gtest/gtest.h"

using namespace std;

TEST(RollingHashTest, Basic) {
  SpanHash h(string("foobarfoo"));
  EXPECT_EQ(h.get(0, 3), h.get(6, 9));  // "foo" == "foo"
  EXPECT_NE(h.get(0, 3), h.get(3, 6));  // "foo" != "bar"
  EXPECT_NE(h.get(0, 3), h.get(0, 4));  // "foo" != "foob"
}

TEST(RollingHashTest, Concat) {
  SpanHash h(string("abcabcabc"));
  EXPECT_EQ(
      RollingHash::concat(h.get(0, 3), h.get(6, 8), 2),
      h.get(0, 5));
}
