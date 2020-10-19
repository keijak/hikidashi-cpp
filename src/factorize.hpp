#include <bits/stdc++.h>
using i64 = long long;

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

// Factorizes a number into {prime, count} pairs. O(sqrt(n)).
std::vector<std::pair<i64, int>> factorize(i64 n) {
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

// Returns a bitset to tell if each number is prime. O(n log log n).
std::vector<bool> prime_sieve(int n) {
  std::vector<bool> is_prime(n + 1, true);
  is_prime[0] = is_prime[1] = false;
  for (int i = 4; i <= n; i += 2) is_prime[i] = false;
  for (int i = 3; i * i <= n; i += 2) {
    if (!is_prime[i]) continue;
    for (int j = 2 * i; j <= n; j += i) is_prime[j] = false;
  }
  return is_prime;
}

// osa_k's algorithm.
// Returns a vector that maps x to x's smallest divisor. O(n log log n).
std::vector<int> min_divisor_sieve(int n) {
  std::vector<int> res(n + 1);
  for (int i = 1; i <= n; ++i) {
    res[i] = i;
  }
  for (int i = 4; i <= n; i += 2) res[i] = 2;
  for (int i = 3; i * i <= n; i += 2) {
    if (res[i] != i) continue;
    for (int j = 2 * i; j <= n; j += i) res[j] = i;
  }
  return res;
}

// Quick factorization.
std::vector<std::pair<int, int>> quick_factorize(
    int n, const std::vector<int>& min_divisor) {
  assert(0 < n and n < int(min_divisor.size()));
  std::vector<std::pair<int, int>> res;
  for (;;) {
    int p = min_divisor[n];
    if (p == 1) break;
    int count = 0;
    do {
      n /= p;
      ++count;
    } while (n % p == 0);
    res.emplace_back(p, count);
  }
  return res;
}

// Returns all prime numbers smaller than or equal to n.
std::vector<int> primes_upto(int n) {
  std::vector<int> res;
  const auto& res = min_divisor_sieve(n);
  for (int i = 2; i <= n; ++i) {
    if (res[i] == i) res.push_back(i);
  }
  return res;
}
