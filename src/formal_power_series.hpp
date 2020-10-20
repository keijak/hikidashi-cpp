#include <bits/stdc++.h>
using i64 = long long;

template <typename T, int MAX_DEGREE>
struct DenseFPS {
  std::vector<T> coeff;  // coefficient

  DenseFPS() = default;
  explicit DenseFPS(std::vector<T> coeff) : coeff(std::move(coeff)) {}
  explicit DenseFPS(T scalar) : DenseFPS(std::vector<T>(1, scalar)) {}
  DenseFPS(const DenseFPS& other) : coeff(other.coeff) {}
  DenseFPS(DenseFPS&& other) : coeff(std::move(other.coeff)) {}
  DenseFPS& operator=(const DenseFPS& other) { coeff = other.coeff; }
  DenseFPS& operator=(DenseFPS&& other) { coeff = std::move(other.coeff); }

  int size() const { return (int)coeff.size(); }

  const T& operator[](int i) const { return coeff[i]; }

  DenseFPS& operator+=(const DenseFPS& other) {
    int n = max(size(), other.size());
    if (n > size()) {
      coeff.resize(n);
    }
    int m = std::min(size(), other.size());
    for (int i = 0; i < m; ++i) {
      coeff[i] += other[i];
    }
    return *this;
  }

  DenseFPS& operator-=(const DenseFPS& other) {
    int n = max(size(), other.size());
    if (n > size()) {
      coeff.resize(n);
    }
    int m = std::min(size(), other.size());
    for (int i = 0; i < m; ++i) {
      coeff[i] -= other[i];
    }
    return *this;
  }

  DenseFPS& operator*=(const DenseFPS& other) {
    int n = std::min(int(size() + other.size() - 1), MAX_DEGREE + 1);
    auto res = atcoder::convolution(coeff, other.coeff);
    res.resize(n);
    this->coeff = std::move(res);
    return *this;
  }

  DenseFPS& mul(const DenseFPS& other) {
    int n = std::min(int(size() + other.size() - 1), MAX_DEGREE + 1);
    if (n > size()) {
      terms.resize(n);
    }
    std::vector<T> res(n);
    for (int i = 0; i < size(); ++i) {
      for (int j = 0; j < other.size(); ++j) {
        if (i + j >= n) break;
        res[i + j] += (*this)[i] * other[j];
      }
    }
    terms = std::move(res);
    return *this;
  }

  friend DenseFPS operator+(const DenseFPS& x, const DenseFPS& y) {
    DenseFPS res(x);
    res += y;
    return res;
  }

  friend DenseFPS operator-(const DenseFPS& x, const DenseFPS& y) {
    DenseFPS res(x);
    res -= y;
    return res;
  }

  friend DenseFPS operator*(const DenseFPS& x, const DenseFPS& y) {
    DenseFPS res(x);
    res *= y;
    return res;
  }

  DenseFPS pow(u64 t) const {
    DenseFPS base = *this;
    DenseFPS res(T(1));
    while (t) {
      if (t & 1) res *= base;
      base *= base;
      t >>= 1;
    }
    return res;
  }
};

template <typename T>
struct DenseFPS {
  std::vector<T> terms;  // coefficient

  explicit DenseFPS(std::vector<T> terms) : terms(std::move(terms)) {}
  explicit DenseFPS(T scalar) : DenseFPS(std::vector<T>(1, scalar)) {}
  DenseFPS(const DenseFPS& other) : terms(other.terms) {}
  DenseFPS(DenseFPS&& other) : terms(std::move(other.terms)) {}
  DenseFPS& operator=(const DenseFPS& other) { terms = other.terms; }
  DenseFPS& operator=(DenseFPS&& other) { terms = std::move(other.terms); }

  int size() const { return (int)terms.size(); }

  const T& operator[](int i) const { return terms[i]; }

  DenseFPS& operator+=(const DenseFPS& other) {
    int n = max(size(), other.size());
    if (n > size()) {
      terms.resize(n);
    }
    int m = std::min(size(), other.size());
    for (int i = 0; i < m; ++i) {
      terms[i] += other[i];
    }
    return *this;
  }

  DenseFPS& operator-=(const DenseFPS& other) {
    int n = max(size(), other.size());
    if (n > size()) {
      terms.resize(n);
    }
    int m = std::min(size(), other.size());
    for (int i = 0; i < m; ++i) {
      terms[i] -= other[i];
    }
    return *this;
  }

  // TODO: Use FFT.

  friend DenseFPS operator+(const DenseFPS& x, const DenseFPS& y) {
    DenseFPS res(x);
    res += y;
    return res;
  }

  friend DenseFPS operator-(const DenseFPS& x, const DenseFPS& y) {
    DenseFPS res(x);
    res -= y;
    return res;
  }

  friend DenseFPS operator*(const DenseFPS& x, const DenseFPS& y) {
    DenseFPS res(x);
    res *= y;
    return res;
  }

  DenseFPS pow(unsigned long long t) const {
    DenseFPS base = *this;
    DenseFPS res = T(1);
    while (t) {
      if (t & 1) res *= base;
      base *= base;
      t >>= 1;
    }
    return res;
  }
};

template <typename T>
class SparseFPS {
  int size_;
  std::vector<i64> degrees;
  std::vector<T> coeffs;

  explicit SparseFPS(std::vector<std::pair<i64, T>> terms)
      : size_(terms.size()), degrees(size_), coeffs(size_) {
    // Sort by degree in ascending order.
    sort(terms.begin(), terms.end());
    for (int i = 0; i < size_; ++i) {
      degrees[i] = terms[i].first;
      coeffs[i] = terms[i].second;
    }
  }

  int size() const { return size_; }

  void add(i64 degree, T coeff) {
    if (not degrees.empty()) {
      assert(degree > degrees.back());
    }
    degrees.push_back(std::move(degree));
    coeffs.push_back(std::move(coeff));
  }

  const T& operator[](int i) const {
    auto it = std::lower_bound(degrees.begin(), degrees.end(), i);
    if (it == degrees.end()) return (T)(0);
    int j = std::distance(degrees.begin(), it);
    return coeffs[j];
  }
};
