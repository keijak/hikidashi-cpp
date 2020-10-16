#include <bits/stdc++.h>
using i64 = long long;

template <typename T>
struct DenseFPS {
  std::vector<T> terms;  // coefficient

  explicit DenseFPS(std::vector<T> terms) : terms(std::move(terms)) {}

  int size() const { return (int)terms.size(); }

  const T& operator[](int i) const { return terms[i]; }
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
