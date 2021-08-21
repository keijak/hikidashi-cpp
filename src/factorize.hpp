#include <bits/stdc++.h>
using i64 = long long;

struct PrimeSieve {
  std::vector<int> spf;  // smallest prime factors table.
  std::vector<int> primes;

  explicit PrimeSieve(int n) : spf(n + 1), primes(1, 2) {
    // Linear Sieve
    for (int i = 2; i <= n; ++i) {
      if (spf[i] == 0) {
        spf[i] = i;
        primes.push_back(i);
      }
      for (const auto& p : primes) {
        if (i * p > n or p > spf[i]) break;
        spf[i * p] = p;
      }
    }
  }

  inline bool is_prime(int n) { return spf[n] == n; }

  std::vector<std::pair<int, int>> factorize(int n) {
    assert(0 < n and n < int(spf.size()));
    std::vector<std::pair<int, int>> res;
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

  i64 divisor_count(int n) {
    assert(0 < n and n < int(spf.size()));
    i64 res = 1;
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

  i64 divisor_sum(int n) {
    assert(0 < n and n < int(spf.size()));
    i64 res = 1;
    while (n > 1) {
      const int p = spf[n];
      i64 power = 1, psum = 1;
      do {
        n /= p;
        power *= p;
        psum += power;
      } while (n % p == 0);
      res *= psum;
    }
    return res;
  }

  // Möbius function.
  int moebius(int n) {
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

  // Euler's totient function.
  int phi(int n) {
    assert(0 < n and n < int(spf.size()));
    i64 res = n;
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
std::vector<i64> divisors(i64 n) {
  std::vector<i64> res;
  for (i64 k = 1; k * k <= n; ++k) {
    if (n % k != 0) continue;
    res.push_back(k);
    i64 q = n / k;
    if (q != k) res.push_back(q);
  }
  std::sort(res.begin(), res.end());
  return res;
}

// Returns a table of divisor counts of integers <= n.
std::vector<int> divisor_count_table(int n) {
  std::vector<int> counts(n + 1, 1);
  counts[0] = 0;
  for (int i = 2; i * i <= n; ++i) {
    for (int j = i * i; j <= n; j += i) {
      ++counts[j];
    }
  }
  return counts;
}

// Factorizes a number into {prime, count} pairs. O(sqrt(n)).
std::vector<std::pair<i64, int>> factorize(i64 n) {
  assert(n > 0);
  std::vector<std::pair<i64, int>> res;
  for (i64 k = 2; k * k <= n; ++k) {
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
std::vector<i64> enumerate_divisors(
    const std::vector<std::pair<i64, int>>& fac) {
  std::vector<i64> res = {1};
  for (auto [p, k] : fac) {
    int sz = res.size();
    for (int i = 0; i < sz; ++i) {
      i64 pp = 1;
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
std::vector<i64> divisors2(i64 n) { return enumerate_divisors(factorize(n)); }

// Euler's totient function.
// Number of integers coprime to n (between 1 and n inclusive).
i64 phi(i64 n) {
  i64 res = n;
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

// Returns a prime table of the segment [L, R).
// (table[x-L] != 0) ⇔ x is a prime number.
std::vector<i64> segment_sieve(i64 L, i64 R) {
  static const i64 SQRTN = 1 << 16;  // upper bound of sqrt(R)
  static int p[SQRTN], lookup = 0;
  assert(R <= SQRTN * SQRTN);
  if (!lookup) {
    for (int i = 2; i < SQRTN; ++i) p[i] = i;
    for (int i = 2; i * i < SQRTN; ++i)
      if (p[i])
        for (int j = i * i; j < SQRTN; j += i) p[j] = 0;
    std::remove(p, p + SQRTN, 0);
    lookup = 1;
  }
  std::vector<i64> table(R - L);
  for (i64 i = L; i < R; ++i) {
    table[i - L] = i;
  }
  for (int i = 0; i64(p[i]) * p[i] < R; ++i) {  // O( \sqrt(R) )
    i64 j;
    if (p[i] >= L) {
      j = i64(p[i]) * p[i];
    } else if (L % p[i] == 0) {
      j = L;
    } else {
      j = L - (L % p[i]) + p[i];
    }
    for (; j < R; j += p[i]) {
      table[j - L] = 0;
    }
  }
  return table;
}
