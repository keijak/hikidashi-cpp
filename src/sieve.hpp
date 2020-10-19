#include <bits/stdc++.h>

// Returns a bitset to tell if a number less than or equal to n is prime.
std::vector<bool> sieve_simple(int n) {
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
// Returns a vector that maps x to x's smallest divisor.
std::vector<int> min_divisors(int n) {
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
  assert(0 < n and n < int(res.size()));
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

// Returns the prime numbers smaller than or equal to n.
std::vector<int> primes(int n) {
  std::vector<int> res;
  auto res = min_divisors(n);
  for (int i = 2; i <= n; ++i) {
    if (res[i] == i) res.push_back(i);
  }
  return res;
}
