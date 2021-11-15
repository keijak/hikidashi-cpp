// In-place Zeta Transform and Moebius Transform.
#include <cassert>
#include <vector>

// F(T) = sum_{S ⊂ T} f(S)
template <typename T>
void subset_zeta_transform(std::vector<T> &f) {
  const int n = f.size();
  assert((n & (n - 1)) == 0);  // n: power of 2.
  for (int i = 1; i < n; i <<= 1) {
    for (int j = 0; j < n; j++) {
      if ((j & i) == 0) {
        f[j | i] += f[j];
      }
    }
  }
}

template <typename T>
void subset_mobius_transform(std::vector<T> &f) {
  const int n = f.size();
  assert((n & (n - 1)) == 0);  // n: power of 2.
  for (int i = 1; i < n; i <<= 1) {
    for (int j = 0; j < n; j++) {
      if ((j & i) == 0) {
        f[j | i] -= f[j];
      }
    }
  }
}

template <typename T>
std::vector<T> bitwise_or_convolution(std::vector<T> f, std::vector<T> g) {
  assert(f.size() == g.size());
  subset_zeta_transform(f);
  subset_zeta_transform(g);
  for (int i = 0; i < int(f.size()); ++i) {
    f[i] *= g[i];
  }
  subset_mobius_transform(f);
  return f;
}

// F(T) = sum_{S ⊃ T} f(S)
template <typename T>
void superset_zeta_transform(std::vector<T> &f) {
  const int n = f.size();
  assert((n & (n - 1)) == 0);  // n: power of 2.
  for (int i = 1; i < n; i <<= 1) {
    for (int j = 0; j < n; j++) {
      if ((j & i) == 0) {
        f[j] += f[j | i];
      }
    }
  }
}

template <typename T>
void superset_mobius_transform(std::vector<T> &f) {
  const int n = f.size();
  assert((n & (n - 1)) == 0);  // n: power of 2.
  for (int i = 1; i < n; i <<= 1) {
    for (int j = 0; j < n; j++) {
      if ((j & i) == 0) {
        f[j] -= f[j | i];
      }
    }
  }
}

template <typename T>
std::vector<T> bitwise_and_convolution(std::vector<T> f, std::vector<T> g) {
  assert(f.size() == g.size());
  superset_zeta_transform(f);
  superset_zeta_transform(g);
  for (int i = 0; i < int(f.size()); ++i) {
    f[i] *= g[i];
  }
  superset_mobius_transform(f);
  return f;
}

// F(n) = sum_{i|n} f(i)
// O(N log log N)
template <class T>
void divisor_zeta_transform(std::vector<T> &f, const std::vector<int> &primes) {
  const int n = f.size();
  for (auto p : primes) {
    if (p >= n) break;
    for (int k = 1; k * p < n; ++k) {
      f[k * p] += f[k];
    }
  }
}

// f(n) = sum_{i|n} mu(n/i)*F(i)
// O(N log log N)
template <class T>
void divisor_moebius_transform(std::vector<T> &f,
                               const std::vector<int> &primes) {
  const int n = f.size();
  for (auto p : primes) {
    if (p >= n) break;
    for (int k = (n - 1) / p; k > 0; --k) {
      f[k * p] -= f[k];
    }
  }
}

template <typename T>
std::vector<T> lcm_convolution(std::vector<T> f, std::vector<T> g,
                               const std::vector<int> &primes) {
  assert(f.size() == g.size());
  divisor_zeta_transform(f, primes);
  divisor_zeta_transform(g, primes);
  for (int i = 0; i < int(f.size()); ++i) {
    f[i] *= g[i];
  }
  divisor_moebius_transform(f, primes);
  return f;
}

// F(n) = sum_{n|i} f(i)
// O(N log log N)
template <class T>
void multiple_zeta_transform(std::vector<T> &f,
                             const std::vector<int> &primes) {
  const int n = f.size();
  for (auto p : primes) {
    if (p >= n) break;
    for (int k = (n - 1) / p; k > 0; --k) {
      f[k] += f[k * p];
    }
  }
}

// f(n) = sum_{n|i} mu(i/n)*F(i)
// O(N log log N)
template <class T>
void multiple_moebius_transform(std::vector<T> &f,
                                const std::vector<int> &primes) {
  const int n = f.size();
  for (auto p : primes) {
    if (p >= n) break;
    for (int k = 1; k * p < n; ++k) {
      f[k] -= f[k * p];
    }
  }
}

template <typename T>
std::vector<T> gcd_convolution(std::vector<T> f, std::vector<T> g,
                               const std::vector<int> &primes) {
  assert(f.size() == g.size());
  multiple_zeta_transform(f, primes);
  multiple_zeta_transform(g, primes);
  for (int i = 0; i < int(f.size()); ++i) {
    f[i] *= g[i];
  }
  multiple_moebius_transform(f, primes);
  return f;
}
