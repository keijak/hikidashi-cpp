// Integer math operations that take negative numbers into account.

#include <bits/stdc++.h>
using i64 = long long;

// Returns ceil(x / y).
i64 ceildiv(i64 x, i64 y) {
  int sign = (x < 0 ? -1 : 1) * (y < 0 ? -1 : 1);
  if (sign == 1) {
    auto d = std::abs(y);
    return (std::abs(x) + d - 1) / d;
  } else {
    return -(std::abs(x) / std::abs(y));
  }
}

// Returns floor(x / y).
i64 floordiv(i64 x, i64 y) {
  int sign = (x < 0 ? -1 : 1) * (y < 0 ? -1 : 1);
  if (sign == 1) {
    return std::abs(x) / std::abs(y);
  } else {
    auto d = std::abs(y);
    return -((std::abs(x) + d - 1) / d);
  }
}

// Returns x mod y.
i64 floormod(i64 x, i64 y) {
  auto q = floordiv(x, y);
  auto r = x - q * y;
  assert(r >= 0);
  return r;
}

// Etended Euclidean algorithm.
// Returns [g, x, y] where g = a*x + b*y = GCD(a, b).
// Either x or y is negative.
std::array<i64, 3> ext_gcd(i64 a, i64 b) {
  i64 ax = 1, ay = 0, bx = 0, by = 1;
  while (b != 0) {
    auto r = a % b;
    auto q = a / b;
    a = b;
    b = r;
    ax -= bx * q;
    std::swap(ax, bx);
    ay -= by * q;
    std::swap(ay, by);
  }
  return {a, ax, ay};
}

// Linear Diophantine equation (Bezout equation).
// Solves a*x + b*y = c (a,b,c: positive integers).
// Returns [x, y] where 0 <= x < b/gcd(a,b).
// If you want y which satisfies "0 <= y < a/gcd(a,b)", swap arguments:
// i.e. invoke `linear_diophantine(b, a, c)`.
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
