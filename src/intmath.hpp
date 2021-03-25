// Integer math operations that take negative numbers into account.

#include <bits/stdc++.h>
using i64 = long long;

#include <atcoder/math>

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

// Returns floor(sqrt(x)).
i64 floorsqrt(i64 x) {
  assert(x >= 0);
  if (x <= 1) return x;
  i64 r = std::floor(sqrtl((long double)x));
  while (r * r < x) ++r;
  while (r * r > x) --r;
  return r;
}

// Returns ceil(sqrt(x)).
i64 ceilsqrt(i64 x) {
  assert(x >= 0);
  if (x <= 1) return x;
  i64 r = std::ceil(sqrtl((long double)x));
  while (r * r > x) --r;
  while (r * r < x) ++r;
  return r;
}

// Etended Euclidean algorithm.
// Returns [g, x, y] where g = a*x + b*y = GCD(a, b).
// Note that g, x, y can be negative.
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
  if (g < 0) {
    g *= -1, x *= -1, y *= -1;
  }
  if (c % g != 0) return std::nullopt;
  a /= g, b /= g, c /= g;
  x *= c, y *= c;
  auto m = floordiv(x, b);
  x -= m * b;
  y += m * a;
  assert(0 <= x and x < b);
  return std::array{x, y};
}

// Discrete logarithm.
// Returns the smallest x that satisfies a^x ≡ b (mod m).
// m is assumed to be a prime number.
std::optional<int> log_mod(i64 a, i64 b, const int m) {
  const int L = ceilsqrt(m);  // block size
  a %= m, b %= m;

  // {a^0 => 0, a^1 => 1, ...}
  std::map<int, int> baby;
  {
    int rem = 1;
    for (long long r = 0; r < L; ++r) {
      if (not baby.count(rem)) baby[rem] = r;
      (rem *= a) %= m;
    }
  }

  // Giant step: a^{-L}
  int step = atcoder::pow_mod(atcoder::inv_mod(a, m), L, m);
  int giant = b;
  for (int q = 0; q < L; ++q) {
    if (auto it = baby.find(giant); it != baby.end()) {
      return q * L + it->second;
    }
    (giant *= step) %= m;
  }
  return std::nullopt;
}
