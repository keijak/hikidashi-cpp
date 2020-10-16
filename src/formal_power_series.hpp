#include <bits/stdc++.h>
using i64 = long long;

template <typename T>
struct DenseFPS {
  std::vector<T> terms;  // coefficient

  explicit DenseFPS(std::vector<T> terms) : terms(std::move(terms)) {}
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
  DenseFPS& operator*=(const DenseFPS& other) {
    int n = size() + other.size() - 1;
    if (n > size()) {
      terms.resize(n);
    }
    std::vector<T> res(n);
    for (int i = 0; i < size(); ++i) {
      for (int j = 0; j < other.size(); ++j) {
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
