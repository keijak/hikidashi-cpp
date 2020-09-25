// Factorizes a number into {prime, count} pairs.
//
// Maybe precalculate primes for a faster main loop.
//   for (auto k : primes) { ... }
vector<pair<i64, int>> factorize(i64 n) {
  vector<pair<i64, int>> res;
  for (i64 k = 2; k * k <= n; ++k) {
    int cnt = 0;
    while (n % k == 0) {
      n /= k;
      ++cnt;
    }
    if (cnt > 0) {
      res.emplace_back(k, cnt);
    }
  }
  if (n > 1) {
    res.emplace_back(n, 1);
  }
  return res;
}
