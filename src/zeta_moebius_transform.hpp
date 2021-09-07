// Zeta Transform and Moebius Transform.
#include <cassert>
#include <vector>

template <typename T>
void superset_zeta_transform(std::vector<T> &f) {
  int n = f.size();
  assert((n & (n - 1)) == 0);
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
  int n = f.size();
  assert((n & (n - 1)) == 0);
  for (int i = 1; i < n; i <<= 1) {
    for (int j = 0; j < n; j++) {
      if ((j & i) == 0) {
        f[j] -= f[j | i];
      }
    }
  }
}

template <typename T>
void subset_zeta_transform(std::vector<T> &f) {
  int n = f.size();
  assert((n & (n - 1)) == 0);
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
  int n = f.size();
  assert((n & (n - 1)) == 0);
  for (int i = 1; i < n; i <<= 1) {
    for (int j = 0; j < n; j++) {
      if ((j & i) == 0) {
        f[j | i] -= f[j];
      }
    }
  }
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
