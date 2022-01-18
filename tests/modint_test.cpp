#include <bits/stdc++.h>

#include "../src/modint.hpp"
#include "gtest/gtest.h"

using namespace std;

TEST(ModIntTest, Mod7) {
  ModInt<7> a1;
  EXPECT_EQ(a1.val(), 0);
  ModInt<7> a2(3);
  EXPECT_EQ(a2.val(), 3);
  EXPECT_EQ((a1 + a2).val(), 3);
  EXPECT_EQ((a1 * a2).val(), 0);
  ModInt<7> a3 = 10;
  EXPECT_EQ(a3.val(), 3);
  EXPECT_EQ((a3 + a2).val(), 6);
  EXPECT_EQ((a3 * a2).val(), 2);
  ModInt<7> a4 = 50;
  EXPECT_EQ(a4.val(), 1);
  ModInt<7> a5 = -50;
  EXPECT_EQ(a5.val(), 6);
}

TEST(ModIntTest, InvModPrime) {
  ModInt<7> a = 4;
  auto b = a.inv();
  EXPECT_EQ((a * b).val(), 1);
  EXPECT_EQ(b.val(), 2);
}

TEST(ModIntTest, InvModNonPrime) {
  ModInt<6> a = 5;
  auto b = a.inv();
  EXPECT_EQ((a * b).val(), 1);
  EXPECT_EQ(b.val(), 5);
}
