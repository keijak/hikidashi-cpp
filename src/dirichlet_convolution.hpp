#include <bits/stdc++.h>

// Dirichlet Convolution.
// Updates `f` in place:
//   f = f * g
//     = \sum_{d|n} f(d)g(n/d)
// Time complexity: O(n log log n)
template <typename T>
void dirichlet_convolution_inplace(std::vector<T>& f, const std::vector<T>& g,
                                   const int n,
                                   const std::vector<int>& primes) {
  assert(n < (int)f.size() and n < (int)g.size());
  for (int p : primes) {
    for (int i = n / p; i > 0; --i) {
      const int to = i * p;
      for (int x = i, y = p;; x /= p, y *= p) {
        f[to] += f[x] * g[y];
        if (x % p != 0) break;
      }
    }
  }
}

template <typename T>
std::vector<T> dirichlet_convolution(const std::vector<T>& f,
                                     const std::vector<T>& g, const int n,
                                     const std::vector<int>& primes) {
  std::vector<T> ret = f;
  ret.resize(n + 1);
  dirichlet_convolution_inplace(ret, g, n, primes);
  return ret;
}
