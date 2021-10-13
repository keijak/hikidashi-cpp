// Integer math operations that take negative numbers into account.

#include <bits/stdc++.h>

#include <atcoder/math>

using i64 = long long;

template <class T>
T saturating_mul(T x, T y) {
  static_assert(std::is_integral_v<T>);
  T res;
  if (__builtin_mul_overflow(x, y, &res)) {
    return res;
  } else if ((x ^ y) < 0) {
    return std::numeric_limits<T>::lowest();
  } else {
    return std::numeric_limits<T>::max();
  }
}

// Returns floor(sqrt(x)).
i64 floor_sqrt(i64 x) {
  assert(x >= 0);
  if (x <= 1) return x;
  i64 r = std::floor(sqrtl((long double)x));
  while (r * r < x) ++r;
  while (r * r > x) --r;
  return r;
}

// Returns ceil(sqrt(x)).
i64 ceil_sqrt(i64 x) {
  assert(x >= 0);
  if (x <= 1) return x;
  i64 r = std::ceil(sqrtl((long double)x));
  while (r * r > x) --r;
  while (r * r < x) ++r;
  return r;
}

// Returns x mod y.
// Guarantees 0 <= r < y (even when x is negative).
template <class T>
T floor_mod(T x, T y) {
  assert(y > 0);
  auto r = x % y;
  if (r < 0) r += y;
  return r;
}

// Returns ceil(x / y).
template <class T>
T ceil_div(T x, T y) {
  assert(y != 0);
  return x / y + (((x ^ y) >= 0) and (x % y));
}

// Returns floor(x / y).
template <class T>
T floor_div(T x, T y) {
  assert(y != 0);
  return x / y - (((x ^ y) < 0) and (x % y));
}

template <typename T>
T power(T b, i64 e) {
  assert(e >= 0);
  T x = 1;
  while (e > 0) {
    if (e & 1) x *= b;
    b *= b;
    e >>= 1;
  }
  return x;
}

// [1, x, x^2, x^3, ..., x^n]
template <typename T>
std::vector<T> pow_seq(int n, int base = 2) {
  assert(n >= 0 and base > 0);
  std::vector<T> p(n + 1);
  p[0] = 1;
  for (int i = 0; i < n; ++i) {
    p[i + 1] = p[i] * base;
  }
  return p;
}

template <typename T>
T arithmetic_progression_sum(T a1, T d, T n) {
  T an = a1 + d * (n - 1);
  return n * (a1 + an) / 2;
}

template <typename T>
T geometric_progression_sum(T a1, T r, T n) {
  return a1 * (power(r, n) - 1) / (r - 1);
}

// [floor(n/1),  floor(n/2), ..., floor(n/n)]
// Contains O(sqrt(n)) distinct values.
//
// Returns (value, l, r) for each distinct value, such that
// value = floor(n/x) for l <= x < r
auto floor_harmonic_sequence(i64 n) {
  std::vector<std::tuple<i64, i64, i64>> ret;
  i64 l = 1;
  while (l <= n) {
    i64 r = n / (n / l) + 1;
    // floor(n/x) yields the same value for l <= x < r.
    ret.emplace_back(n / l, l, r);
    l = r;
  }
  return ret;
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
  auto m = floor_div(x, b);
  x -= m * b;
  y += m * a;
  assert(0 <= x and x < b);
  return std::array{x, y};
}

// Discrete logarithm (Baby-step Giant-step algorithm).
// Returns the smallest x that satisfies a^x ≡ b (mod m).
// m is assumed to be a prime number.
std::optional<int> log_mod(i64 a, i64 b, const int m) {
  const int L = ceil_sqrt(m);  // block size
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
