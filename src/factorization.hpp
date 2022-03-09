#include <bits/stdc++.h>
using Int = long long;

// Precalculating multiplicative functions from 1 to N in O(N).
// https://codeforces.com/blog/entry/54090

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
        if (i * p > n) break;
        spf[i * p] = p;
        if (i % p == 0) break;
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

// Linear Sieve for precalculating multiplicative function tables.
// https://codeforces.com/blog/entry/54090
struct LinearSieve {
  std::vector<int> spf;  // smallest prime factors.
  std::vector<int> mu;   // Moebius function
  std::vector<int> phi;  // Euler's totient function
  std::vector<int> primes;

  explicit LinearSieve(int n) : spf(n + 1), mu(n + 1), phi(n + 1) {
    mu[1] = 1;
    phi[1] = 1;
    for (int i = 2; i <= n; ++i) {
      if (spf[i] == 0) {
        spf[i] = i;
        mu[i] = -1;
        phi[i] = i - 1;
        primes.push_back(i);
      }
      for (const auto &p : primes) {
        if (i * p > n) break;
        spf[i * p] = p;
        if (i % p == 0) {
          mu[i * p] = 0;
          phi[i * p] = phi[i] * p;
          break;
        } else {
          mu[i * p] = -mu[i];
          phi[i * p] = phi[i] * phi[p];
        }
      }
    }
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

// Moebius function in O(sqrt(n)).
int moebius(Int n) {
  int res = 1;
  for (auto [p, k] : factorize(n)) {
    if (k >= 2) return 0;
    res *= -1;
  }
  return res;
}

// Euler's phi function in O(sqrt(n)).
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
  Int L, R, max_spf_;
  std::vector<Int> spf_;  // smallest prime factor for [0, max_spf_]
  std::vector<std::vector<Int>> factors_;  // small prime factors of (L+i).
  std::vector<Int> factors_prod_;  // product of small prime factors of (L+i).

 public:
  SegmentSieve(Int l, Int r, Int m = 1) : L(l), R(r), max_spf_(m) {
    while (__int128_t(max_spf_) * max_spf_ <= R)
      max_spf_ *= 2;  // Ensure max_spf > sqrt(R).
    spf_.resize(max_spf_ + 1);
    std::iota(spf_.begin(), spf_.end(), 0);
    factors_.resize(R - L);
    factors_prod_.assign(R - L, 1);

    for (Int i = 2; i * i < R; ++i) {
      if (spf_[i] != i) continue;
      for (Int j = i * i; j <= max_spf_; j += i) {
        if (spf_[j] == j) spf_[j] = i;
      }
      for (Int j = (L + i - 1) / i * i; j < R; j += i) {
        Int num = j;
        do {
          if (factors_prod_[j - L] > max_spf_) break;
          factors_[j - L].push_back(i);
          factors_prod_[j - L] *= i;
          num /= i;
        } while (num % i == 0);
      }
    }
  }

  inline bool is_prime(int n) const {
    if (n <= 1) return false;
    if (n <= max_spf_) return spf_[n] == n;
    assert(L <= n and n < R);
    auto p = factors_prod_[n - L];
    return p == 1 or p == n;
  }

  std::vector<Int> factorize(Int n) const {
    if (n <= 1) return {};
    assert(n <= max_spf_ or (L <= n and n < R));
    std::vector<Int> res;
    if (n >= L) {
      res = factors_[n - L];
      n /= factors_prod_[n - L];
      if (n > max_spf_) {
        res.push_back(n);  // n must be a large prime.
        n = 1;
      }
    }
    while (n > 1) {
      res.push_back(spf_[n]);
      n /= spf_[n];
    }
    std::sort(res.begin(), res.end());
    return res;
  }
};
