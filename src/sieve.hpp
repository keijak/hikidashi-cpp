// Returns a bitset to tell if a number less than or equal to n is prime.
vector<bool> sieve_simple(int n) {
  vector<bool> is_prime(n + 1, true);
  is_prime[0] = is_prime[1] = false;
  for (int i = 4; i <= n; i += 2) is_prime[i] = false;
  for (int i = 3; i * i <= n; i += 2) {
    if (!is_prime[i]) continue;
    for (int j = 2 * i; j <= n; j += i) is_prime[j] = false;
  }
  return is_prime;
}

// osa_k's algorithm.
vector<int> sieve(int n) {
  vector<int> min_factor(n + 1);
  REP(i, n + 1) { min_factor[i] = i; }
  for (int i = 4; i <= n; i += 2) min_factor[i] = 2;
  for (int i = 3; i * i <= n; i += 2) {
    if (min_factor[i] != i) continue;
    for (int j = 2 * i; j <= n; j += i) min_factor[j] = i;
  }
  return min_factor;
}

// Quick factorization.
vector<pair<int, int>> factorize(int n, const vector<int>& min_factor) {
  assert(0 < n and n < int(min_factor.size()));
  vector<pair<int, int>> res;
  for (;;) {
    int p = min_factor[n];
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

// Returns the prime numbers less than or equal to n.
vector<int> primes(int n) {
  vector<int> res;
  auto min_factor = sieve(n);
  for (int i = 2; i <= n; ++i) {
    if (min_factor[i] == i) res.push_back(i);
  }
  return res;
}
