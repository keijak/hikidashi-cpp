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
  DenseFPS(std::initializer_list<T> init) : coeff_(init.begin(), init.end()) {
    assert((int)init.size() <= DMAX + 1);
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

  // Returns the coefficient of x^k.
  T operator[](int k) const {
    if (k >= size()) return 0;
    return coeff_[k];
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
    for (int i = 0; i < other.size(); ++i) coeff_[i] += other[i];
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
    for (int i = 0; i < other.size(); ++i) coeff_[i] -= other[i];
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
  friend DenseFPS operator*(const T &scalar, const DenseFPS &y) {
    DenseFPS res = {scalar};
    res *= y;
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
  friend DenseFPS operator/(const T &scalar, const DenseFPS &y) {
    DenseFPS res = {scalar};
    res /= y;
    return res;
  }
  DenseFPS &operator/=(const DenseFPS &other) {
    return *this *= other.inv_naive();
  }
  friend DenseFPS operator/(const DenseFPS &x, const DenseFPS &y) {
    return x * y.inv_naive();
  }

  // Naive inverse. O(N^2)
  DenseFPS inv_naive() const {
    std::vector<T> res(DMAX + 1);
    res[0] = coeff_[0].inv();
    for (int i = 1; i <= DMAX; ++i) {
      T s = 0;
      const int mi = std::min(i + 1, size());
      for (int j = 1; j < mi; ++j) {
        s += coeff_[j] * res[i - j];
      }
      res[i] = -res[0] * s;
    }
    return DenseFPS(std::move(res));
  }

 private:
  // Naive multiplication. O(N^2)
  DenseFPS mul_naive(const DenseFPS &other) const {
    const int n = std::min(size() + other.size() - 1, DMAX + 1);
    std::vector<T> res(n);
    for (int i = 0; i < size(); ++i) {
      for (int j = 0; j < other.size(); ++j) {
        if (i + j >= n) break;
        res[i + j] += coeff_[i] * other.coeff_[j];
      }
    }
    return DenseFPS(std::move(res));
  }
};

namespace fps {

// Fast polynomial multiplication by single NTT.
// T: modint
template <typename T, int DMAX>
DenseFPS<T, DMAX> mul_ntt(const DenseFPS<T, DMAX> &x,
                          const DenseFPS<T, DMAX> &y) {
  static_assert(T::mod() != 1'000'000'007);  // Must be a NTT-friendly MOD!
  auto res = atcoder::convolution(x.coeff_, y.coeff_);
  if (res.size() > DMAX + 1) res.resize(DMAX + 1);  // shrink
  return DenseFPS<T, DMAX>(std::move(res));
}

// Polynomial multiplication by NTT + Garner (arbitrary mod).
// T: modint
template <typename T, int DMAX>
DenseFPS<T, DMAX> mul_mod(const DenseFPS<T, DMAX> &x,
                          const DenseFPS<T, DMAX> &y) {
  std::vector<i64> xll(x.size()), yll(y.size());
  for (int i = 0; i < x.size(); ++i) xll[i] = x[i].val();
  for (int i = 0; i < y.size(); ++i) yll[i] = y[i].val();
  auto zll = atcoder::convolution_ll(move(xll), move(yll));
  int n = std::min<int>(zll.size(), DMAX + 1);
  std::vector<T> res(n);
  for (int i = 0; i < n; ++i) res[i] = zll[i];
  return DenseFPS<T, DMAX>(std::move(res));
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
// T: modint
template <typename T, int DMAX>
DenseFPS<T, DMAX> inv_ntt(const DenseFPS<T, DMAX> &x) {
  assert(x[0].val() != 0);  // must be invertible
  const int n = x.size();
  std::vector<T> res(n);
  res[0] = x[0].inv();
  for (int i = 1; i < n; i <<= 1) {
    vector<mint> f(2 * i), g(2 * i);
    for (int j = 0, m = min(n, 2 * i); j < m; ++j) f[j] = x[j];
    for (int j = 0; j < i; ++j) g[j] = res[j];
    f = atcoder::convolution(f, g);
    f.resize(2 * i);
    for (int j = 0; j < i; ++j) f[j] = 0;
    f = atcoder::convolution(f, g);
    for (int j = i, m = min(2 * i, n); j < m; ++j) res[j] = -f[j];
  }
  return DenseFPS<T, DMAX>(std::move(res));
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

// T: modint
template <typename T, int DMAX>
DenseFPS<T, DMAX> pow_ntt(const DenseFPS<T, DMAX> &x, u64 t) {
  return pow_generic(x, t, mul_ntt);
}

// T: modint
template <typename T, int DMAX>
DenseFPS<T, DMAX> pow_mod(const DenseFPS<T, DMAX> &x, u64 t) {
  return pow_generic(x, t, mul_mod);
}

template <int DMAX>
DenseFPS<i64, DMAX> pow_ll(const DenseFPS<i64, DMAX> &x, u64 t) {
  return pow_generic(x, t, mul_ll);
}

template <typename T, int DMAX>
DenseFPS<T, DMAX> derivative(const DenseFPS<T, DMAX> &x) {
  std::vector<T> res(x.size() - 1);
  for (int i = 1; i < x.size(); ++i) {
    res[i - 1] = x[i] * i;
  }
  return DenseFPS(std::move(res));
}

template <typename T, int DMAX>
DenseFPS<T, DMAX> integral(const DenseFPS<T, DMAX> &x, const T &c) {
  std::vector<T> res(x.size() + 1);
  res[0] = c;
  for (int i = 1; i <= x.size(); ++i) {
    res[i] = x[i - 1] / i;
  }
  return DenseFPS(std::move(res));
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
    for (int i = 0; i < size_; ++i) {
      degree_[i] = terms[i].first;
      coeff_[i] = terms[i].second;
    }
  }

  inline int size() const { return size_; }
  inline const T &coeff(int i) const { return coeff_[i]; }
  inline int degree(int i) const { return degree_[i]; }
  int max_degree() const { return (size_ == 0) ? 0 : degree_.back(); }

  void emplace_back(int d, T c) {
    if (not degree_.empty()) {
      assert(d > degree_.back());
    }
    degree_.push_back(std::move(d));
    coeff_.push_back(std::move(c));
    ++size_;
  }

  // Returns the coefficient of x^d.
  T operator[](int d) const {
    auto it = std::lower_bound(degree_.begin(), degree_.end(), d);
    if (it == degree_.end() or *it != d) return (T)(0);
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
    int j = 0;  // two pointers (i, j)
    for (int i = 0; i < size(); ++i) {
      const int deg = this->degree(i);
      for (; j < other.size() and other.degree(j) < deg; ++j) {
        res.emplace_back(other.degree(j), other.coeff(j));
      }
      T c = this->coeff(i);
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
template <typename T, int DMAX>
DenseFPS<T, DMAX> &operator+=(DenseFPS<T, DMAX> &x, const SparseFPS<T> &y) {
  for (int i = 0; i < y.size(); ++i) {
    if (y.degree(i) > DMAX) break;  // ignore
    x.coeff_[y.degree(i)] += y.coeff(i);
  }
  return x;
}
template <typename T, int DMAX>
DenseFPS<T, DMAX> operator+(const DenseFPS<T, DMAX> &x, const SparseFPS<T> &y) {
  DenseFPS<T, DMAX> res = x;
  res += y;
  return res;
}
template <typename T, int DMAX>
DenseFPS<T, DMAX> operator+(const SparseFPS<T> &x, const DenseFPS<T, DMAX> &y) {
  return y + x;  // commutative
}

// Polynomial multiplication (dense * sparse).
template <typename T, int DMAX>
DenseFPS<T, DMAX> &operator*=(DenseFPS<T, DMAX> &x, const SparseFPS<T> &y) {
  if (y.size() == 0) {
    return x *= 0;
  }
  T c0 = 0;
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
template <typename T, int DMAX>
DenseFPS<T, DMAX> operator*(const DenseFPS<T, DMAX> &x, const SparseFPS<T> &y) {
  DenseFPS<T, DMAX> res = x;
  res *= y;
  return res;
}
template <typename T, int DMAX>
DenseFPS<T, DMAX> operator*(const SparseFPS<T> &x, const DenseFPS<T, DMAX> &y) {
  return y * x;  // commutative
}

// Polynomial division (dense / sparse).
template <typename T, int DMAX>
DenseFPS<T, DMAX> &operator/=(DenseFPS<T, DMAX> &x, const SparseFPS<T> &y) {
  assert(y.size() > 0 and y.degree(0) == 0 and y.coeff(0) != 0);
  const T inv_c0 = y.coeff(0).inv();
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
template <typename T, int DMAX>
DenseFPS<T, DMAX> operator/(const DenseFPS<T, DMAX> &x, const SparseFPS<T> &y) {
  DenseFPS<T, DMAX> res = x;
  res /= y;
  return res;
}
