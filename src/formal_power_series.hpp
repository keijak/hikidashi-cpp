#include <bits/stdc++.h>
using i64 = long long;
using u64 = unsigned long long;

// Formal Power Series (dense format).
template <typename T, int MAX_DEGREE>
struct DenseFPS {
  // Coefficients of terms from x^0 to x^MAX_DEGREE.
  std::vector<T> coeff;

  DenseFPS() : coeff(MAX_DEGREE + 1) {}  // zero-initialized
  explicit DenseFPS(std::vector<T> c) : coeff(std::move(c)) {}

  DenseFPS(const DenseFPS &other) : coeff(other.coeff) {}
  DenseFPS(DenseFPS &&other) : coeff(std::move(other.coeff)) {}
  DenseFPS &operator=(const DenseFPS &other) {
    coeff = other.coeff;
    return *this;
  }
  DenseFPS &operator=(DenseFPS &&other) {
    coeff = std::move(other.coeff);
    return *this;
  }

  static constexpr int size() { return MAX_DEGREE + 1; }

  const T &operator[](int i) const { return coeff[i]; }

  DenseFPS &operator+=(const DenseFPS &other) {
    for (int i = 0; i < size(); ++i) {
      coeff[i] += other[i];
    }
    return *this;
  }

  DenseFPS &operator-=(const DenseFPS &other) {
    for (int i = 0; i < size(); ++i) {
      coeff[i] -= other[i];
    }
    return *this;
  }

  DenseFPS &operator*=(const DenseFPS &other) {
    std::vector<T> res(size());
    for (int i = 0; i < size(); ++i) {
      for (int j = 0; i + j < size(); ++j) {
        res[i + j] += (*this)[i] * other[j];
      }
    }
    coeff = std::move(res);
    return *this;
  }

  friend DenseFPS operator+(const DenseFPS &x, const DenseFPS &y) {
    DenseFPS res(x);
    res += y;
    return res;
  }

  friend DenseFPS operator-(const DenseFPS &x, const DenseFPS &y) {
    DenseFPS res(x);
    res -= y;
    return res;
  }

  friend DenseFPS operator*(const DenseFPS &x, const DenseFPS &y) {
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

// Fast polynomial multiplication by single NTT.
template <typename ModInt, int MAX_DEGREE>
DenseFPS<ModInt, MAX_DEGREE> mul_ntt(const DenseFPS<ModInt, MAX_DEGREE> &x,
                                     const DenseFPS<ModInt, MAX_DEGREE> &y) {
  static_assert(ModInt::mod() != 1'000'000'007);  // Must be NTT-friendly MOD!
  auto z = atcoder::convolution(x.coeff, y.coeff);
  z.resize(MAX_DEGREE + 1);  // Maybe shrink.
  return {std::move(z)};
}

// Polynomial multiplication by NTT + Garner (arbitrary mod).
template <typename ModInt, int MAX_DEGREE>
DenseFPS<ModInt, MAX_DEGREE> mul_mod(const DenseFPS<ModInt, MAX_DEGREE> &x,
                                     const DenseFPS<ModInt, MAX_DEGREE> &y) {
  std::vector<i64> xll(x.size()), yll(y.size());
  for (int i = 0; i < x.size(); ++i) {
    xll[i] = x[i].val();
  }
  for (int i = 0; i < y.size(); ++i) {
    yll[i] = y[i].val();
  }
  auto zll = atcoder::convolution_ll(xll, yll);
  DenseFPS<ModInt, MAX_DEGREE> res;
  for (int i = 0; i <= MAX_DEGREE; ++i) {
    res.coeff[i] = zll[i];
  }
  return res;
}

// Polynomial multiplication by NTT + Garner (long long).
template <int MAX_DEGREE>
DenseFPS<i64, MAX_DEGREE> mul_ll(const DenseFPS<i64, MAX_DEGREE> &x,
                                 const DenseFPS<i64, MAX_DEGREE> &y) {
  auto z = atcoder::convolution_ll(x.coeff, y.coeff);
  z.resize(MAX_DEGREE + 1);  // Maybe shrink.
  return DenseFPS<i64, MAX_DEGREE>(std::move(z));
}

// Formal Power Series (sparse format).
template <typename T>
class SparseFPS {
  int size_;
  std::vector<i64> degree;
  std::vector<T> coeff;

  explicit SparseFPS(std::vector<std::pair<i64, T>> terms)
      : size_(terms.size()), degree(size_), coeff(size_) {
    // Sort by degree in ascending order.
    sort(terms.begin(), terms.end());
    for (int i = 0; i < size_; ++i) {
      degree[i] = terms[i].first;
      coeff[i] = terms[i].second;
    }
  }

  int size() const { return size_; }

  void add(i64 degree, T coeff) {
    if (not degree.empty()) {
      assert(degree > degree.back());
    }
    degree.push_back(std::move(degree));
    coeff.push_back(std::move(coeff));
  }

  const T &operator[](int i) const {
    auto it = std::lower_bound(degree.begin(), degree.end(), i);
    if (it == degree.end()) return (T)(0);
    int j = std::distance(degree.begin(), it);
    return coeff[j];
  }
};
