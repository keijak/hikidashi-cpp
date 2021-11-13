#include <bits/stdc++.h>
using Int = long long;

struct PrimeSieve {
  std::vector<int> spf;  // smallest prime factors table.
  std::vector<int> primes;

  explicit PrimeSieve(int n) : spf(n + 1) {
    // O(n)
    for (int i = 2; i <= n; ++i) {
      if (spf[i] == 0) {
        spf[i] = i;
        primes.push_back(i);
      }
      for (const auto &p : primes) {
        if (i * p > n or p > spf[i]) break;
        spf[i * p] = p;
      }
    }
  }

  inline bool is_prime(int n) const { return spf[n] == n; }

  auto factorize(int n) const {
    assert(0 < n and n < int(spf.size()));
    std::vector<std::pair<Int, int>> res;
    while (n > 1) {
      const int p = spf[n];
      int count = 0;
      do {
        n /= p;
        ++count;
      } while (n % p == 0);
      res.emplace_back(p, count);
    }
    return res;
  }

  Int divisor_count(int n) const {
    assert(0 < n and n < int(spf.size()));
    Int res = 1;
    while (n > 1) {
      const int p = spf[n];
      int count = 0;
      do {
        n /= p;
        ++count;
      } while (n % p == 0);
      res *= (1 + count);
    }
    return res;
  }

  Int divisor_sum(int n) const {
    assert(0 < n and n < int(spf.size()));
    Int res = 1;
    while (n > 1) {
      const int p = spf[n];
      Int power = 1, psum = 1;
      do {
        n /= p;
        power *= p;
        psum += power;
      } while (n % p == 0);
      res *= psum;
    }
    return res;
  }

  // Number of prime factors.
  int prime_factor_count(int n) const {
    assert(0 < n and n < int(spf.size()));
    int res = 0;
    while (n > 1) {
      const int p = spf[n];
      do {
        n /= p;
      } while (n % p == 0);
      ++res;
    }
    return res;
  }

  // Möbius function.
  int moebius(int n) const {
    assert(0 < n and n < int(spf.size()));
    int res = 1;
    while (n > 1) {
      const int p = spf[n];
      n /= p;
      if (n % p == 0) return 0;
      res *= -1;
    }
    return res;
  }

  // Euler's phi function.
  int totient(int n) const {
    assert(0 < n and n < int(spf.size()));
    Int res = n;
    while (n > 1) {
      const int p = spf[n];
      do {
        n /= p;
      } while (n % p == 0);
      res /= p;
      res *= p - 1;
    }
    return res;
  }
};

// Returns all divisors of n. O(sqrt(n)) + sorting.
std::vector<Int> divisors(Int n) {
  std::vector<Int> res;
  for (Int k = 1; k * k <= n; ++k) {
    if (n % k != 0) continue;
    res.push_back(k);
    Int q = n / k;
    if (q != k) res.push_back(q);
  }
  std::sort(res.begin(), res.end());
  return res;
}

// Returns a table of divisor counts of integers <= n.
std::vector<int> divisor_count_table(int n) {
  assert(n >= 1);
  std::vector<int> counts(n + 1, 2);  // 1 and self
  counts[0] = 0;
  counts[1] = 1;
  for (int d = 2; d * 2 <= n; ++d) {
    for (int x = d * 2; x <= n; x += d) {
      ++counts[x];
    }
  }
  return counts;
}

// Factorizes a number into {prime, count} pairs. O(sqrt(n)).
std::vector<std::pair<Int, int>> factorize(Int n) {
  assert(n > 0);
  std::vector<std::pair<Int, int>> res;
  for (Int k = 2; k * k <= n; ++k) {
    if (n % k != 0) continue;
    int count = 0;
    do {
      n /= k;
      ++count;
    } while (n % k == 0);
    res.emplace_back(k, count);
  }
  if (n > 1) {
    res.emplace_back(n, 1);
  }
  return res;
}

// Enumerates divisors from prime factorization.
// O(d(n)) + sorting
std::vector<Int> enumerate_divisors(
    const std::vector<std::pair<Int, int>> &fac) {
  std::vector<Int> res = {1};
  for (auto [p, k] : fac) {
    int sz = res.size();
    for (int i = 0; i < sz; ++i) {
      Int pp = 1;
      for (int j = 0; j < k; ++j) {
        pp *= p;
        res.push_back(res[i] * pp);
      }
    }
  }
  std::sort(res.begin(), res.end());
  return res;
}

// On a large N, often faster than simple `divisors()`.
std::vector<Int> divisors2(Int n) { return enumerate_divisors(factorize(n)); }

// Euler's phi function.
// Number of integers coprime to n (between 1 and n inclusive).
Int totient(Int n) {
  Int res = n;
  for (auto [p, _] : factorize(n)) {
    res /= p;
    res *= p - 1;
  }
  return res;
}

// Returns a bitset to tell if each number is prime. O(n log log n).
std::vector<bool> prime_sieve(int n) {
  std::vector<bool> is_prime(n + 1, true);
  is_prime[0] = is_prime[1] = false;
  for (int i = 4; i <= n; i += 2) is_prime[i] = false;
  for (int i = 3; i * i <= n; i += 2) {
    if (not is_prime[i]) continue;
    for (int j = i * i; j <= n; j += i) is_prime[j] = false;
  }
  return is_prime;
}

// Factorization table for a segment [L, R).
class SegmentSieve {
  Int L, R, M;
  std::vector<Int> spf;  // smallest prime factor for [0, sqrt(R)]
  std::vector<std::vector<Int>> factors;  // small prime factors of (L+i).
  std::vector<Int> factors_prod;  // product of small prime factors of (L+i).

 public:
  SegmentSieve(Int L, Int R) : L(L), R(R), M(1) {
    while (M * M <= R) M <<= 1;  // Ensure M > sqrt(R).
    spf.resize(M);
    std::iota(spf.begin(), spf.end(), 0);
    factors.resize(R - L);
    factors_prod.assign(R - L, 1);

    for (Int i = 2; i * i < R; ++i) {
      if (spf[i] != i) continue;
      for (Int j = i * i; j < M; j += i) {
        if (spf[j] == j) spf[j] = i;
      }
      for (Int j = (L + i - 1) / i * i; j < R; j += i) {
        Int num = j;
        do {
          if (factors_prod[j - L] >= M) break;
          factors[j - L].push_back(i);
          factors_prod[j - L] *= i;
          num /= i;
        } while (num % i == 0);
      }
    }
  }

  inline bool is_prime(int n) const {
    if (n <= 1) return false;
    if (n < M) return spf[n] == n;
    assert(L <= n and n < R);
    auto fp = factors_prod[n - L];
    return (fp == n) or (fp == 1);
  }

  std::vector<Int> factorize(Int n) const {
    if (n <= 1) return {};
    assert(n < M or (L <= n and n < R));
    std::vector<Int> res;
    if (n >= L) {
      res = factors[n - L];
      n /= factors_prod[n - L];
      if (n >= M) {
        res.push_back(n);  // n must be a large prime.
        n = 1;
      }
    }
    while (n > 1) {
      res.push_back(spf[n]);
      n /= spf[n];
    }
    std::sort(res.begin(), res.end());
    return res;
  }
};
