#include <bits/stdc++.h>

// Computes the n-th term of a linear recurrence sequence.
template <typename T>
struct Kitamasa {
  const int k;

  // Initial k elements (a[0] .. a[k-1]).
  const std::vector<T> a_;

  // Coefficients of the recurrence.
  //   a[n] = coeff[0]*a[n-1] + coeff[1]*a[n-2] + ... + coeff[k-1]*a[n-k].
  const std::vector<T> coeff_;

  explicit Kitamasa(std::vector<T> a, std::vector<T> c)
      : k(a.size()), a_(a), coeff_(std::move(c)) {}

  // n: 0-indexed.
  T nth(long long n) const {
    assert(n >= 0);
    auto x = nth_coeff(n + 1);
    T res = 0;
    for (int i = 0; i < k; ++i) {
      res += x[i] * a_[i];
    }
    return res;
  }

 private:
  // n: 1-indexed.
  std::vector<T> nth_coeff(long long n) const {
    if (n <= k) {
      std::vector<T> res(k, 0);
      res[n - 1] = 1;
      return res;
    }
    // Doubling.
    if (n & 1) {
      // f(n-1) => f(n)
      return next(nth_coeff(n - 1));
    } else {
      // f(n/2) => f(n)
      std::vector<T> s = nth_coeff(n >> 1);
      std::vector<T> t = s;
      std::vector<T> res(k, 0);
      for (int i = 0; i < k; ++i) {
        t = next(std::move(t));
        for (int j = 0; j < k; ++j) {
          res[j] += t[j] * s[i];
        }
      }
      return res;
    }
  }

  std::vector<T> next(std::vector<T> x) const {
    std::vector<T> res(k, 0);
    for (int i = 1; i < k; ++i) {
      res[i] = x[i - 1];
    }
    for (int i = 0; i < k; ++i) {
      res[i] += x[k - 1] * coeff_[k - 1 - i];
    }
    return res;
  }
};
