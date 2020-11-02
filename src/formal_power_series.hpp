#include <bits/stdc++.h>

#include <atcoder/convolution>
#include <atcoder/modint>

using i64 = long long;
using u64 = unsigned long long;

// Formal Power Series (dense format).
template <typename T, int DMAX>
struct DenseFPS {
  // Coefficients of terms from x^0 to x^DMAX.
  std::vector<T> coeff_;

  DenseFPS() : coeff_(1) {}  // zero-initialized
  explicit DenseFPS(std::vector<T> c) : coeff_(std::move(c)) {
    assert((int)c.size() <= DMAX + 1);
  }

  DenseFPS(const DenseFPS &other) : coeff_(other.coeff_) {}
  DenseFPS(DenseFPS &&other) : coeff_(std::move(other.coeff_)) {}
  DenseFPS &operator=(const DenseFPS &other) {
    coeff_ = other.coeff_;
    return *this;
  }
  DenseFPS &operator=(DenseFPS &&other) {
    coeff_ = std::move(other.coeff_);
    return *this;
  }

  int size() const { return (int)coeff_.size(); }

  // Returns the coefficient of x^dy.
  T operator[](int dy) const {
    if (dy >= size()) return 0;
    return coeff_[dy];
  }

  DenseFPS &operator+=(const T &scalar) {
    coeff_[0] += scalar;
    return *this;
  }
  friend DenseFPS operator+(const DenseFPS &x, const T &scalar) {
    DenseFPS res = x;
    res += scalar;
    return res;
  }
  DenseFPS &operator+=(const DenseFPS &other) {
    if (size() < other.size()) {
      coeff_.resize(other.size());
    }
    for (int dx = 0; dx < other.size(); ++dx) coeff_[dx] += other[dx];
    return *this;
  }
  friend DenseFPS operator+(const DenseFPS &x, const DenseFPS &y) {
    DenseFPS res = x;
    res += y;
    return res;
  }

  DenseFPS &operator-=(const DenseFPS &other) {
    if (size() < other.size()) {
      coeff_.resize(other.size());
    }
    for (int dx = 0; dx < other.size(); ++dx) coeff_[dx] -= other[dx];
    return *this;
  }
  friend DenseFPS operator-(const DenseFPS &x, const DenseFPS &y) {
    DenseFPS res = x;
    res -= y;
    return res;
  }

  DenseFPS &operator*=(const T &scalar) {
    for (auto &x : coeff_) x *= scalar;
    return *this;
  }
  friend DenseFPS operator*(const DenseFPS &x, const T &scalar) {
    DenseFPS res = x;
    res *= scalar;
    return res;
  }
  DenseFPS &operator*=(const DenseFPS &other) {
    *this = this->mul_naive(other);
    return *this;
  }
  friend DenseFPS operator*(const DenseFPS &x, const DenseFPS &y) {
    return x.mul_naive(y);
  }

  DenseFPS &operator/=(const T &scalar) {
    for (auto &x : coeff_) x /= scalar;
    return *this;
  }
  friend DenseFPS operator/(const DenseFPS &x, const T &scalar) {
    DenseFPS res = x;
    res /= scalar;
    return res;
  }
  DenseFPS &operator/=(const DenseFPS &other) {
    return *this *= other.inv_naive();
  }
  friend DenseFPS operator/(const DenseFPS &x, const DenseFPS &y) {
    return x * y.inv_naive();
  }

  DenseFPS derivative() const {
    std::vector<T> res(size());
    for (int i = 1; i < size(); ++i) {
      res[i - 1] = coeff_[i] * i;
    }
    return DenseFPS(std::move(res));
  }

  DenseFPS integral(T c) const {
    std::vector<T> res(size());
    res[0] = std::move(c);
    for (int i = 1; i < size(); ++i) {
      res[i] = coeff_[i - 1] / i;
    }
    return DenseFPS(std::move(res));
  }

 private:
  // Naive multiplication. O(N^2)
  DenseFPS mul_naive(const DenseFPS &other) const {
    const int n = std::min(size() + other.size() - 1, DMAX + 1);
    std::vector<T> res(n);
    for (int dx = 0; dx < size(); ++dx) {
      for (int j = 0; j < other.size(); ++j) {
        if (dx + j >= n) break;
        res[dx + j] += coeff_[dx] * other.coeff_[j];
      }
    }
    return DenseFPS(std::move(res));
  }

  // Naive inverse. O(N^2)
  DenseFPS inv_naive() const {
    std::vector<T> res(size());
    res[0] = coeff_[0].inv();
    for (int i = 1; i < size(); ++i) {
      mint s = 0;
      for (int j = 1; j <= i; ++j) {
        s += coeff_[j] * res[i - j];
      }
      res[i] = -res[0] * s;
    }
    return DenseFPS(std::move(res));
  }
};

namespace fps {

// Fast polynomial multiplication by single NTT.
template <typename ModInt, int DMAX>
DenseFPS<ModInt, DMAX> mul_ntt(const DenseFPS<ModInt, DMAX> &x,
                               const DenseFPS<ModInt, DMAX> &y) {
  static_assert(ModInt::mod() != 1'000'000'007);  // Must be a NTT-friendly MOD!
  auto res = atcoder::convolution(x.coeff_, y.coeff_);
  if (res.size() > DMAX + 1) res.resize(DMAX + 1);  // shrink
  return DenseFPS<ModInt, DMAX>(std::move(res));
}

// Polynomial multiplication by NTT + Garner (arbitrary mod).
template <typename ModInt, int DMAX>
DenseFPS<ModInt, DMAX> mul_mod(const DenseFPS<ModInt, DMAX> &x,
                               const DenseFPS<ModInt, DMAX> &y) {
  std::vector<i64> xll(x.size()), yll(y.size());
  for (int dx = 0; dx < x.size(); ++dx) xll[dx] = x[dx].val();
  for (int dx = 0; dx < y.size(); ++dx) yll[dx] = y[dx].val();
  auto zll = atcoder::convolution_ll(xll, yll);
  int n = std::min<int>(zll.size(), DMAX + 1);
  std::vector<T> res(n);
  for (int dx = 0; dx < n; ++dx) res[dx] = zll[dx];
  return DenseFPS<ModInt, DMAX>(std::move(res));
}

// Polynomial multiplication by NTT + Garner (long long).
template <int DMAX>
DenseFPS<i64, DMAX> mul_ll(const DenseFPS<i64, DMAX> &x,
                           const DenseFPS<i64, DMAX> &y) {
  auto res = atcoder::convolution_ll(x.coeff_, y.coeff_);
  if (res.size() > DMAX + 1) res.resize(DMAX + 1);  // shrink
  return DenseFPS<i64, DMAX>(std::move(res));
}

// Fast polynomial inverse with NTT.
template <typename ModInt, int DMAX>
DenseFPS<ModInt, DMAX> inv_ntt(const DenseFPS<ModInt, DMAX> &x) {
  assert(x[0].val() != 0);  // must be invertible
  const int n = x.size();
  std::vector<ModInt> res(n);
  res[0] = x[0].inv();
  for (int d = 1; d < n; d <<= 1) {
    vector<mint> f(2 * d), g(2 * d);
    for (int j = 0, m = min(n, 2 * d); j < m; ++j) f[j] = x[j];
    for (int j = 0; j < d; ++j) g[j] = res[j];
    f = atcoder::convolution(f, g);
    f.resize(2 * d);
    for (int j = 0; j < d; ++j) f[j] = 0;
    f = atcoder::convolution(f, g);
    for (int j = d, m = min(2 * d, n); j < m; ++j) res[j] = -f[j];
  }
  return DenseFPS<ModInt, DMAX>(std::move(res));
}

template <typename T, int DMAX>
DenseFPS<T, DMAX> pow_generic(
    const DenseFPS<T, DMAX> &x, u64 t,
    DenseFPS<T, DMAX> (*mul)(const DenseFPS<T, DMAX> &,
                             const DenseFPS<T, DMAX> &)) {
  DenseFPS<T, DMAX> base = x, res;
  res.coeff_[0] = 1;
  while (t) {
    if (t & 1) res = mul(res, base);
    base = mul(base, base);
    t >>= 1;
  }
  return res;
}

template <typename ModInt, int DMAX>
DenseFPS<ModInt, DMAX> pow_ntt(const DenseFPS<ModInt, DMAX> &x, u64 t) {
  return pow_generic(x, t, mul_ntt);
}

template <typename ModInt, int DMAX>
DenseFPS<ModInt, DMAX> pow_mod(const DenseFPS<ModInt, DMAX> &x, u64 t) {
  return pow_generic(x, t, mul_mod);
}

template <int DMAX>
DenseFPS<i64, DMAX> pow_ll(const DenseFPS<i64, DMAX> &x, u64 t) {
  return pow_generic(x, t, mul_ll);
}

}  // namespace fps

// Formal Power Series (sparse format).
template <typename T>
struct SparseFPS {
  int size_;
  std::vector<int> degree_;
  std::vector<T> coeff_;

  SparseFPS() : size_(0) {}

  explicit SparseFPS(std::vector<std::pair<int, T>> terms)
      : size_(terms.size()), degree_(size_), coeff_(size_) {
    // Sort by degree_ in ascending order.
    sort(terms.begin(), terms.end());
    for (int dx = 0; dx < size_; ++dx) {
      degree_[dx] = terms[dx].first;
      coeff_[dx] = terms[dx].second;
    }
  }

  inline int size() const { return size_; }
  inline int degree(int dx) const { return degree_[dx]; }
  inline const T &coeff(int dx) const { return coeff_[dx]; }

  int DMAX() const { return (size_ == 0) ? 0 : degree_.back(); }

  void emplace_back(int dy, T c) {
    if (not degree_.empty()) {
      assert(dy > degree_.back());
    }
    degree_.push_back(std::move(dy));
    coeff_.push_back(std::move(c));
    ++size_;
  }

  // Returns the coefficient of x^dy.
  T operator[](int dy) const {
    auto it = std::lower_bound(degree_.begin(), degree_.end(), dy);
    if (it == degree_.end() or *it != dy) return (T)(0);
    int j = std::distance(degree_.begin(), it);
    return coeff_[j];
  }

  SparseFPS &operator+=(const T &scalar) {
    for (auto &x : coeff_) x += scalar;
    return *this;
  }
  friend SparseFPS operator+(const SparseFPS &x, const T &scalar) {
    SparseFPS res = x;
    res += scalar;
    return res;
  }
  SparseFPS &operator+=(const SparseFPS &other) {
    *this = this->add(other);
    return *this;
  }
  friend SparseFPS operator+(const SparseFPS &x, const SparseFPS &y) {
    return x.add(y);
  }

  SparseFPS &operator*=(const T &scalar) {
    for (auto &x : coeff_) x *= scalar;
    return *this;
  }
  friend SparseFPS operator*(const SparseFPS &x, const T &scalar) {
    SparseFPS res = x;
    res *= scalar;
    return res;
  }

  SparseFPS &operator-=(const SparseFPS &other) {
    *this = this->add(other * -1);
    return *this;
  }
  friend SparseFPS operator-(const SparseFPS &x, const SparseFPS &y) {
    return x.add(y * -1);
  }

 private:
  SparseFPS add(const SparseFPS &other) const {
    SparseFPS res;
    int j = 0;
    for (int dx = 0; dx < size();) {
      const int deg = this->degree(dx);
      for (; j < other.size() and other.degree(j) < deg; ++j) {
        res.emplace_back(other.degree(j), other.coeff(j));
      }
      T c = this->coeff(dx);
      if (j < other.size() and other.degree(j) == deg) {
        c += other.coeff(j);
        ++j;
      }
      if (c != 0) {
        res.emplace_back(deg, c);
      }
    }
    for (; j < other.size(); ++j) {
      res.emplace_back(other.degree(j), other.coeff(j));
    }
    return res;
  }
};

// Polynomial addition (dense + sparse).
template <typename ModInt, int DMAX>
DenseFPS<ModInt, DMAX> &operator+=(DenseFPS<ModInt, DMAX> &x,
                                   const SparseFPS<ModInt> &y) {
  for (int dx = 0; dx < y.size(); ++dx) {
    if (y.degree(dx) > DMAX) break;  // ignore
    x.coeff_[y.degree(dx)] += y.coeff(dx);
  }
  return x;
}
template <typename ModInt, int DMAX>
DenseFPS<ModInt, DMAX> operator+(const DenseFPS<ModInt, DMAX> &x,
                                 const SparseFPS<ModInt> &y) {
  DenseFPS<ModInt, DMAX> res = x;
  res += y;
  return res;
}
template <typename ModInt, int DMAX>
DenseFPS<ModInt, DMAX> operator+(const SparseFPS<ModInt> &x,
                                 const DenseFPS<ModInt, DMAX> &y) {
  return y + x;  // commutative
}

// Polynomial multiplication (dense * sparse).
template <typename ModInt, int DMAX>
DenseFPS<ModInt, DMAX> &operator*=(DenseFPS<ModInt, DMAX> &x,
                                   const SparseFPS<ModInt> &y) {
  if (y.size() == 0) {
    return x *= 0;
  }
  ModInt c0 = 0;
  int j0 = 0;
  if (y.degree(0) == 0) {
    c0 = y.coeff(0);
    ++j0;
  }
  const int dy_max = y.degree(y.size() - 1);
  const int dx_max = x.size() - 1;
  const int n = std::min(dx_max + dy_max + 1, DMAX + 1);
  if (x.size() < n) x.coeff_.resize(n);
  for (int dx = n - 1; dx >= 0; --dx) {
    x.coeff_[dx] *= c0;
    for (int j = j0; j < y.size(); ++j) {
      int dy = y.degree(j);
      if (dy > dx) break;
      x.coeff_[dx] += x[dx - dy] * y.coeff(j);
    }
  }
  return x;
}
template <typename ModInt, int DMAX>
DenseFPS<ModInt, DMAX> operator*(const DenseFPS<ModInt, DMAX> &x,
                                 const SparseFPS<ModInt> &y) {
  DenseFPS<ModInt, DMAX> res = x;
  res *= y;
  return res;
}
template <typename ModInt, int DMAX>
DenseFPS<ModInt, DMAX> operator*(const SparseFPS<ModInt> &x,
                                 const DenseFPS<ModInt, DMAX> &y) {
  return y * x;  // commutative
}

// Polynomial division (dense / sparse).
template <typename ModInt, int DMAX>
DenseFPS<ModInt, DMAX> &operator/=(DenseFPS<ModInt, DMAX> &x,
                                   const SparseFPS<ModInt> &y) {
  assert(y.size() > 0 and y.degree(0) == 0 and y.coeff(0) != 0);
  const ModInt inv_c0 = y.coeff(0).inv();
  const int dy_max = y.degree(y.size() - 1);
  const int dx_max = x.size() - 1;
  const int n = std::min(dx_max + dy_max + 1, DMAX + 1);
  if (x.size() < n) x.coeff_.resize(n);
  for (int dx = 0; dx < n; ++dx) {
    for (int j = 1; j < y.size(); ++j) {
      int dy = y.degree(j);
      if (dy > dx) break;
      x.coeff_[dx] -= x.coeff_[dx - dy] * y.coeff[j];
    }
    x.coeff_[dx] *= inv_c0;
  }
  return x;
}
template <typename ModInt, int DMAX>
DenseFPS<ModInt, DMAX> operator/(const DenseFPS<ModInt, DMAX> &x,
                                 const SparseFPS<ModInt> &y) {
  DenseFPS<ModInt, DMAX> res = x;
  res /= y;
  return res;
}
