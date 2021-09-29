#include <bits/stdc++.h>
using i64 = long long;

int to_decimal_char(char ch) {
  if ('0' <= ch and ch <= '9') {
    return ch - '0';
  }
  if ('A' <= ch and ch <= 'Z') {
    return ch - 'A' + 10;
  }
  if ('a' <= ch and ch <= 'z') {
    return ch - 'a' + 10;
  }
  assert(false);
}

// Parses a k-ary integer into a decimal integer.
// Note: `std::stoll(x, nullptr, k)` might be good enough.
i64 to_decimal(const std::string& x_base_k, int k) {
  if (x_base_k == "0") {
    return 0;
  }
  i64 x_base_10 = 0;
  for (char dchar : x_base_k) {
    int d = to_decimal_char(dchar);
    x_base_10 = x_base_10 * k + d;
  }
  return x_base_10;
}

// Converts a decimal integer to a k-ary integer string.
std::string from_decimal(i64 x_base_10, int k) {
  if (x_base_10 == 0) {
    return "0";
  }
  std::string x_base_k;
  while (x_base_10) {
    char d = (x_base_10 % k) + '0';
    x_base_10 /= k;
    x_base_k.push_back(d);
  }
  std::reverse(x_base_k.begin(), x_base_k.end());
  return x_base_k;
}
