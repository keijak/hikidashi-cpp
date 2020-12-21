// Integer math operations that take negative numbers into account.

#include <bits/stdc++.h>
using i64 = long long;

// Returns ceil(x / y).
i64 ceildiv(i64 x, i64 y) {
  if (y <= 0) {
    assert(y != 0);
    y *= -1;
    x *= -1;
  }
  if (x >= 0) {
    return (x + y - 1) / y;
  } else {
    return -((-x) / y);
  }
}

// Returns floor(x / y).
i64 floordiv(i64 x, i64 y) {
  if (y <= 0) {
    assert(y != 0);
    y *= -1;
    x *= -1;
  }
  if (x >= 0) {
    return x / y;
  } else {
    return -((-x + y - 1) / y);
  }
}

// Returns x mod y.
// Guarantees 0 <= r < y (even when x is negative).
i64 floormod(i64 x, i64 y) {
  assert(y > 0);
  auto r = x % y;
  return (r < 0) ? (r + y) : r;
}

// Etended Euclidean algorithm.
// Returns [g, x, y] where g = a*x + b*y = GCD(a, b).
std::array<i64, 3> ext_gcd(i64 a, i64 b) {
  if (b == 0) return {a, 1LL, 0LL};
  auto res = ext_gcd(b, a % b);  // = (g, x, y)
  res[1] -= (a / b) * res[2];
  std::swap(res[1], res[2]);
  return res;  // = (g, y, x - (a/b)*y)
}

// Linear Diophantine equation (Bezout equation).
// Solves a*x + b*y = c (a,b,c: positive integers).
// Returns [x, y] where 0 <= x < b/gcd(a,b).
std::optional<std::array<i64, 2>> linear_diophantine(i64 a, i64 b, i64 c) {
  auto [g, x, y] = ext_gcd(a, b);
  if (c % g != 0) return std::nullopt;
  a /= g, b /= g, c /= g;
  x *= c, y *= c;
  auto m = floordiv(x, b);
  x -= m * b;
  y += m * a;
  assert(0 <= x and x < b);
  return std::array{x, y};
}
