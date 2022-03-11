// Integer math operations that take negative numbers into account.

#include <bits/stdc++.h>

#include <atcoder/math>

using Int = long long;

// Saturating addition
template <class T>
T sat_add(T x, T y) {
  static_assert(std::is_integral<T>::value);
  static constexpr T kMin = std::numeric_limits<T>::lowest();
  static constexpr T kMax = std::numeric_limits<T>::max();
  static_assert(kMax != 0);
  if (T res; not __builtin_add_overflow(x, y, &res)) {
    return res;
  } else if constexpr (not std::is_signed<T>::value) {
    return kMax;
  } else {
    return (x < 0) ? kMin : kMax;
  }
}

// Saturating multiplication
template <class T>
T sat_mul(T x, T y) {
  static_assert(std::is_integral<T>::value);
  static constexpr T kMin = std::numeric_limits<T>::lowest();
  static constexpr T kMax = std::numeric_limits<T>::max();
  static_assert(kMax != 0);
  if (T res; not __builtin_mul_overflow(x, y, &res)) {
    return res;
  } else if constexpr (not std::is_signed<T>::value) {
    return kMax;
  } else {
    return ((x ^ y) < 0) ? kMin : kMax;
  }
}

// Returns floor(sqrt(x)).
Int floor_sqrt(Int x) {
  assert(x >= 0);
  if (x <= 1) return x;
  Int r = std::floor(sqrtl((long double)x));
  while (r * r < x) ++r;
  while (r * r > x) --r;
  return r;
}

// Returns ceil(sqrt(x)).
Int ceil_sqrt(Int x) {
  assert(x >= 0);
  if (x <= 1) return x;
  Int r = std::ceil(sqrtl((long double)x));
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
T power(T b, Int e) {
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
auto floor_harmonic_sequence(Int n) {
  std::vector<std::tuple<Int, Int, Int>> ret;
  Int l = 1;
  while (l <= n) {
    Int r = n / (n / l) + 1;
    // floor(n/x) yields the same value for l <= x < r.
    ret.emplace_back(n / l, l, r);
    l = r;
  }
  return ret;
}

// Etended Euclidean algorithm.
// Returns [g, x, y] where g = a*x + b*y = GCD(a, b).
// Note that g, x, y can be negative.
std::array<Int, 3> ext_gcd(Int a, Int b) {
  if (b == 0) return {a, 1LL, 0LL};
  auto res = ext_gcd(b, a % b);  // = (g, x, y)
  res[1] -= (a / b) * res[2];
  std::swap(res[1], res[2]);
  return res;  // = (g, y, x - (a/b)*y)
}

// Linear Diophantine equation (Bezout equation).
// Solves a*x + b*y = c (a,b,c: positive integers).
// Returns [x, y] where 0 <= x < b/gcd(a,b).
std::optional<std::array<Int, 2>> linear_diophantine(Int a, Int b, Int c) {
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
std::optional<int> log_mod(Int a, Int b, const int m) {
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

// verified: https://judge.yosupo.jp/problem/sqrt_mod
std::optional<Int> sqrt_mod(Int a, int p) {
  if (a == 0) return 0;
  if (p == 2) return a;
  if (atcoder::pow_mod(a, (p - 1) >> 1, p) != 1) return std::nullopt;
  Int b = 1;
  while (atcoder::pow_mod(b, (p - 1) >> 1, p) == 1) ++b;
  Int e = 0, m = p - 1;
  while (m % 2 == 0) m >>= 1, ++e;
  auto x = atcoder::pow_mod(a, (m - 1) >> 1, p);
  auto y = a * (x * x % p) % p;
  (x *= a) %= p;
  auto z = atcoder::pow_mod(b, m, p);
  while (y != 1) {
    Int j = 0, t = y;
    while (t != 1) {
      j += 1;
      (t *= t) %= p;
    }
    z = atcoder::pow_mod(z, 1LL << (e - j - 1), p);
    (x *= z) %= p;
    (z *= z) %= p;
    (y *= z) %= p;
    e = j;
  }
  return x;
}
