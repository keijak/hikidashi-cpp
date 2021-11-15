#include <bits/stdc++.h>
using Int = long long;

int to_decimal_digit(char ch) {
  if ('0' <= ch and ch <= '9') {
    return ch - '0';
  }
  if ('A' <= ch and ch <= 'Z') {
    return ch - 'A' + 10;
  }
  if ('a' <= ch and ch <= 'z') {
    return ch - 'a' + 36;
  }
  assert(false);
}

// Parses a k-ary integer into a decimal integer.
// Note: `std::stoll(x, nullptr, k)` might be good enough.
Int to_decimal(const std::string& x_base_k, int k) {
  if (x_base_k == "0") {
    return 0;
  }
  Int x_base_10 = 0;
  for (char dchar : x_base_k) {
    int d = to_decimal_digit(dchar);
    x_base_10 = x_base_10 * k + d;
  }
  return x_base_10;
}

char from_decimal_digit(int di) {
  if (di < 10) return di + '0';       // '0' - '9'
  if (di < 36) return di - 10 + 'A';  // 'A' - 'Z'
  return di - 36 + 'a';               // 'a' - 'z'
}

// Converts a decimal integer to a k-ary integer string.
std::string from_decimal(Int x_base_10, int k) {
  if (x_base_10 == 0) {
    return "0";
  }
  std::string x_base_k;
  while (x_base_10) {
    int di = x_base_10 % k;
    char dc = from_decimal_digit(di);
    x_base_k.push_back(dc);
    x_base_10 /= k;
  }
  std::reverse(x_base_k.begin(), x_base_k.end());
  return x_base_k;
}
