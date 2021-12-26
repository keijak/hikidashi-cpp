// Formal Power Series (Dense/Sparse)
//
// Runtime DMAX with a reference template param:
//   int kRuntimeDMax;
//   template <typename T, int& DMAX> struct NaiveMult {...};
//   using DF = DenseFPS<NaiveMult<Mint, kRuntimeDMax>>;
//
//   kRuntimeDMax = n;  // Set before creating an FPS object.
//   DF f = {1};

#include <bits/stdc++.h>
using Int = long long;

#include <atcoder/convolution>
#include <atcoder/math>
#include <atcoder/modint>

namespace multiplication {
template <typename T, int DMAX>
struct NaiveMult {
  using value_type = T;
  static constexpr int dmax() { return DMAX; }

  static std::vector<T> multiply(const std::vector<T> &x,
                                 const std::vector<T> &y) {
    const int n = std::min<int>(x.size() + y.size() - 1, DMAX + 1);
    const int mi = std::min<int>(x.size(), n);
    std::vector<T> res(n);
    for (int i = 0; i < mi; ++i) {
      for (int j = 0; j < int(y.size()); ++j) {
        if (i + j >= n) break;
        res[i + j] += x[i] * y[j];
      }
    }
    return res;
  }

  static std::vector<T> invert(const std::vector<T> &x) {
    std::vector<T> res(DMAX + 1);
    res[0] = T(1) / x[0];
    for (int i = 1; i <= DMAX; ++i) {
      T s = 0;
      const int mj = std::min<int>(i + 1, x.size());
      for (int j = 1; j < mj; ++j) {
        s += x[j] * res[i - j];
      }
      res[i] = -res[0] * s;
    }
    return res;
  }
};

// T: modint
template <typename T, int DMAX>
struct NTTMult {
  static_assert(atcoder::internal::is_modint<T>::value, "Requires ACL modint.");
  static_assert(T::mod() == 998244353, "Requires an NTT-friendly mod.");

  using value_type = T;
  static constexpr int dmax() { return DMAX; }

  static std::vector<T> multiply(const std::vector<T> &x,
                                 const std::vector<T> &y) {
    std::vector<T> res = atcoder::convolution(x, y);
    if (int(res.size()) > DMAX + 1) res.resize(DMAX + 1);  // shrink
    return res;
  }

  static std::vector<T> invert(const std::vector<T> &x, int d = -1) {
    int n = x.size();
    assert(n != 0 && x[0].val() != 0);  // must be invertible
    if (d == -1) d = n;
    assert(d >= 0);
    std::vector<T> res{x[0].inv()};
    for (int m = 1, m2 = 2; m < d; m = m2, m2 *= 2) {
      std::vector<T> f(x.begin(), x.begin() + std::min(n, m2));
      std::vector<T> g(res);
      f.resize(m2), atcoder::internal::butterfly(f);
      g.resize(m2), atcoder::internal::butterfly(g);
      for (int i = 0; i < m2; ++i) f[i] *= g[i];
      atcoder::internal::butterfly_inv(f);
      f.erase(f.begin(), f.begin() + m);
      f.resize(m2), atcoder::internal::butterfly(f);
      for (int i = 0; i < m2; ++i) f[i] *= g[i];
      atcoder::internal::butterfly_inv(f);
      T iz = T(m2).inv();
      iz *= -iz;
      for (int i = 0; i < m; ++i) f[i] *= iz;
      res.insert(res.end(), f.begin(), f.begin() + m);
    }
    res.resize(d);
    return res;
  }
};

template <int DMAX>
struct IntMult {
  using value_type = long long;
  static constexpr int dmax() { return DMAX; }

  static std::vector<value_type> multiply(const std::vector<value_type> &x,
                                          const std::vector<value_type> &y) {
    auto res = atcoder::convolution_ll(x, y);
    if (int(res.size()) > DMAX + 1) res.resize(DMAX + 1);  // shrink
    return res;
  }

  static std::vector<value_type> invert(const std::vector<value_type> &x) {
    assert(false);  // not implemented
  }
};

template <int DMAX>
struct FloatMult {
  using value_type = double;
  static constexpr int dmax() { return DMAX; }

  static std::vector<value_type> multiply(const std::vector<value_type> &x,
                                          const std::vector<value_type> &y) {
    FastFourierTransform<Real> fft;
    auto res = fft.multiply(x, y);
    if (int(res.size()) > DMAX + 1) res.resize(DMAX + 1);  // shrink
    return res;
  }

  static std::vector<value_type> invert(const std::vector<value_type> &x) {
    assert(false);  // not implemented
  }
};

// T: modint
template <typename T, int DMAX>
struct ArbitraryModMult {
  using value_type = T;
  static_assert(atcoder::internal::is_modint<T>::value);

  static constexpr int dmax() { return DMAX; }

  static std::vector<T> convolution(const std::vector<T> &x,
                                    const std::vector<T> &y, int size_limit) {
    std::vector<int> xv(x.size());
    std::vector<int> yv(y.size());
    for (int i = 0; i < (int)x.size(); ++i) xv[i] = x[i].val();
    for (int i = 0; i < (int)y.size(); ++i) yv[i] = y[i].val();

    constexpr int M1 = 167772161, M2 = 469762049, M3 = 1224736769;
    const auto z1 = atcoder::convolution<M1>(xv, yv);
    const auto z2 = atcoder::convolution<M2>(xv, yv);
    const auto z3 = atcoder::convolution<M3>(xv, yv);

    const Int m1_inv_m2 = atcoder::inv_mod(M1, M2);
    const Int m12_inv_m3 = atcoder::inv_mod(Int(M1) * M2, M3);
    const Int m12 = Int(M1) * M2 % T::mod();
    const int n = std::min<int>(x.size() + y.size() - 1, size_limit);
    std::vector<T> res(n);
    for (int i = 0; i < n; ++i) {
      atcoder::static_modint<M2> v1 = z2[i] - z1[i];
      v1 *= m1_inv_m2;
      const Int w1 = v1.val() * Int(M1);
      atcoder::static_modint<M3> v2 = z3[i] - z1[i] - w1;
      v2 *= m12_inv_m3;
      res[i] = z1[i] + w1 + (v2.val() * m12);
    }
    return res;
  }

  static std::vector<T> multiply(const std::vector<T> &x,
                                 const std::vector<T> &y) {
    return convolution(x, y, dmax() + 1);
  }

  static std::vector<T> invert(const std::vector<T> &x) {
    assert(x[0].val() != 0);  // must be invertible
    const int n = x.size();
    std::vector<T> res(n);
    res[0] = T(1) / x[0];
    for (int i = 1; i < n; i <<= 1) {
      const int m = std::min(2 * i, n);
      std::vector<T> f(2 * i), g(2 * i);
      for (int j = 0; j < m; ++j) f[j] = x[j];
      for (int j = 0; j < i; ++j) g[j] = res[j];
      f = convolution(f, g, 2 * i);
      f.resize(2 * i);
      for (int j = 0; j < i; ++j) f[j] = 0;
      f = convolution(f, g, 2 * i);
      for (int j = i; j < m; ++j) res[j] = -f[j];
    }
    return res;
  }
};

}  // namespace multiplication

// Formal Power Series (dense format).
template <typename Mult>
struct DenseFPS {
  using T = typename Mult::value_type;
  static constexpr int dmax() { return Mult::dmax(); }

  // Coefficients of terms from x^0 to x^DMAX.
  std::vector<T> coeff_;

  DenseFPS() : coeff_(1, 0) {}  // zero

  explicit DenseFPS(std::vector<T> c) : coeff_(std::move(c)) {
    while (size() > dmax() + 1) coeff_.pop_back();
    assert(size() > 0);
  }
  DenseFPS(std::initializer_list<T> c) : coeff_(c.begin(), c.end()) {
    while (size() > dmax() + 1) coeff_.pop_back();
    assert(size() > 0);
  }

  // size <= dmax + 1
  inline int size() const { return static_cast<int>(coeff_.size()); }

  // Returns the coefficient of x^k.
  inline T operator[](int k) const { return (k >= size()) ? 0 : coeff_[k]; }

  DenseFPS &operator+=(const T &scalar) {
    coeff_[0] += scalar;
    return *this;
  }
  friend DenseFPS operator+(const DenseFPS &f, const T &scalar) {
    return DenseFPS(f) += scalar;
  }
  DenseFPS &operator+=(const DenseFPS &other) {
    if (size() < other.size()) coeff_.resize(other.size());
    for (int i = 0; i < other.size(); ++i) coeff_[i] += other[i];
    return *this;
  }
  friend DenseFPS operator+(const DenseFPS &f, const DenseFPS &g) {
    return DenseFPS(f) += g;
  }

  DenseFPS &operator-=(const DenseFPS &other) {
    if (size() < other.size()) coeff_.resize(other.size());
    for (int i = 0; i < other.size(); ++i) coeff_[i] -= other[i];
    return *this;
  }
  friend DenseFPS operator-(const DenseFPS &f, const DenseFPS &g) {
    return DenseFPS(f) -= g;
  }

  DenseFPS operator-() const { return *this * -1; }

  DenseFPS &operator*=(const T &scalar) {
    for (auto &x : coeff_) x *= scalar;
    return *this;
  }
  friend DenseFPS operator*(const DenseFPS &f, const T &scalar) {
    return DenseFPS(f) *= scalar;
  }
  friend DenseFPS operator*(const T &scalar, const DenseFPS &g) {
    return DenseFPS{scalar} *= g;
  }
  DenseFPS &operator*=(const DenseFPS &other) {
    return *this =
               DenseFPS(Mult::multiply(std::move(this->coeff_), other.coeff_));
  }
  friend DenseFPS operator*(const DenseFPS &f, const DenseFPS &g) {
    return DenseFPS(Mult::multiply(f.coeff_, g.coeff_));
  }

  DenseFPS &operator/=(const T &scalar) {
    for (auto &x : coeff_) x /= scalar;
    return *this;
  }
  friend DenseFPS operator/(const DenseFPS &f, const T &scalar) {
    return DenseFPS(f) /= scalar;
  }
  friend DenseFPS operator/(const T &scalar, const DenseFPS &g) {
    return DenseFPS{scalar} /= g;
  }
  DenseFPS &operator/=(const DenseFPS &other) {
    return *this *= DenseFPS(Mult::invert(other.coeff_));
  }
  friend DenseFPS operator/(const DenseFPS &f, const DenseFPS &g) {
    return f * DenseFPS(Mult::invert(g.coeff_));
  }

  DenseFPS pow(long long t) const {
    assert(t >= 0);
    DenseFPS res = {1}, base = *this;
    while (t) {
      if (t & 1) res *= base;
      base *= base;
      t >>= 1;
    }
    return res;
  }

  // Multiplies by (1 + c * x^k).
  void multiply2_inplace(int k, int c) {
    assert(k > 0);
    if (size() <= dmax()) {
      coeff_.resize(min(size() + k, dmax() + 1), 0);
    }
    for (int i = size() - 1; i >= k; --i) {
      coeff_[i] += coeff_[i - k] * c;
    }
  }
  // Multiplies by (1 + c * x^k).
  DenseFPS multiply2(int k, int c) const {
    DenseFPS res = *this;
    res.multiply2_inplace(k, c);
    return res;
  }

  // Divides by (1 + c * x^k).
  void divide2_inplace(int k, int c) {
    assert(k > 0);
    for (int i = k; i < size(); ++i) {
      coeff_[i] -= coeff_[i - k] * c;
    }
  }
  // Divides by (1 + c * x^k).
  DenseFPS divide2(int k, int c) const {
    DenseFPS res = *this;
    res.divide2_inplace(k, c);
    return res;
  }

  // Multiplies by x^k.
  void shift_inplace(int k) {
    if (k > 0) {
      if (size() <= dmax()) {
        coeff_.resize(min(size() + k, dmax() + 1), 0);
      }
      for (int i = size() - 1; i >= k; --i) {
        coeff_[i] = coeff_[i - k];
      }
      for (int i = k - 1; i >= 0; --i) {
        coeff_[i] = 0;
      }
    } else if (k < 0) {
      k *= -1;
      for (int i = k; i < size(); ++i) {
        coeff_[i - k] = coeff_[i];
      }
      for (int i = size() - k; i < size(); ++i) {
        // If coefficients of degrees higher than dmax() were truncated
        // beforehand, you lose the information. Ensure dmax() is big enough.
        coeff_[i] = 0;
      }
    }
  }
  // Multiplies by x^k.
  DenseFPS shift(int k) const {
    DenseFPS res = *this;
    res.shift_inplace(k);
    return res;
  }

  T eval(const T &a) const {
    T res = 0, x = 1;
    for (auto c : coeff_) {
      res += c * x;
      x *= a;
    }
    return res;
  }
};

// Formal Power Series (sparse format).
template <typename T>
struct SparseFPS {
  int size_;
  std::vector<int> degree_;
  std::vector<T> coeff_;

  SparseFPS() : size_(0) {}  // zero

  explicit SparseFPS(std::vector<std::pair<int, T>> terms)
      : size_(terms.size()), degree_(size_), coeff_(size_) {
    // Sort by degree.
    std::sort(terms.begin(), terms.end(),
              [](const auto &x, const auto &y) { return x.first < y.first; });
    for (int i = 0; i < size_; ++i) {
      auto [d, c] = terms[i];
      assert(d >= 0);
      degree_[i] = d;
      coeff_[i] = c;
    }
  }

  SparseFPS(std::initializer_list<std::pair<int, T>> terms)
      : SparseFPS(std::vector<std::pair<int, T>>(terms.begin(), terms.end())) {}

  inline int size() const { return size_; }
  inline const T &coeff(int i) const { return coeff_[i]; }
  inline int degree(int i) const { return degree_[i]; }
  int max_degree() const { return (size_ == 0) ? 0 : degree_.back(); }

  void emplace_back(int d, T c) {
    assert(d >= 0);
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
  friend SparseFPS operator+(const SparseFPS &f, const T &scalar) {
    SparseFPS res = f;
    res += scalar;
    return res;
  }
  SparseFPS &operator+=(const SparseFPS &other) {
    *this = this->add(other);
    return *this;
  }
  friend SparseFPS operator+(const SparseFPS &f, const SparseFPS &g) {
    return f.add(g);
  }

  SparseFPS &operator*=(const T &scalar) {
    for (auto &x : coeff_) x *= scalar;
    return *this;
  }
  friend SparseFPS operator*(const SparseFPS &f, const T &scalar) {
    SparseFPS res = f;
    res *= scalar;
    return res;
  }

  SparseFPS &operator-=(const SparseFPS &other) {
    *this = this->add(other * -1);
    return *this;
  }
  friend SparseFPS operator-(const SparseFPS &f, const SparseFPS &g) {
    return f.add(g * -1);
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
template <typename FPS, typename T = typename FPS::T>
FPS &operator+=(FPS &f, const SparseFPS<T> &g) {
  for (int i = 0; i < g.size(); ++i) {
    if (g.degree(i) > FPS::dmax()) break;  // ignore
    f.coeff_[g.degree(i)] += g.coeff(i);
  }
  return f;
}
template <typename FPS, typename T = typename FPS::T>
FPS operator+(const FPS &f, const SparseFPS<T> &g) {
  auto res = f;
  res += g;
  return res;
}
template <typename FPS, typename T = typename FPS::T>
FPS operator+(const SparseFPS<T> &f, const FPS &g) {
  return g + f;  // commutative
}

// Polynomial multiplication (dense * sparse).
template <typename FPS, typename T = typename FPS::T>
FPS &operator*=(FPS &f, const SparseFPS<T> &g) {
  if (g.size() == 0) {
    return f *= 0;
  }
  const int gd_max = g.degree(g.size() - 1);
  const int fd_max = f.size() - 1;
  const int n = std::min(fd_max + gd_max, FPS::dmax()) + 1;
  if (f.size() < n) f.coeff_.resize(n);

  T c0 = 0;
  int j0 = 0;
  if (g.degree(0) == 0) {
    c0 = g.coeff(0);
    j0 = 1;
  }

  for (int fd = n - 1; fd >= 0; --fd) {
    f.coeff_[fd] *= c0;
    for (int j = j0; j < g.size(); ++j) {
      int gd = g.degree(j);
      if (gd > fd) break;
      f.coeff_[fd] += f[fd - gd] * g.coeff(j);
    }
  }
  return f;
}
template <typename FPS, typename T = typename FPS::T>
FPS operator*(const FPS &f, const SparseFPS<T> &g) {
  auto res = f;
  res *= g;
  return res;
}
template <typename FPS, typename T = typename FPS::T>
FPS operator*(const SparseFPS<T> &f, const FPS &g) {
  return g * f;  // commutative
}

// Polynomial division (dense / sparse).
template <typename FPS, typename T = typename FPS::T>
FPS &operator/=(FPS &f, const SparseFPS<T> &g) {
  assert(g.size() > 0 and g.degree(0) == 0 and g.coeff(0) != 0);
  const auto ic0 = T(1) / g.coeff(0);
  for (int fd = 0; fd < f.size(); ++fd) {
    for (int j = 1; j < g.size(); ++j) {
      int gd = g.degree(j);
      if (fd < gd) break;
      f.coeff_[fd] -= f.coeff_[fd - gd] * g.coeff(j);
    }
    f.coeff_[fd] *= ic0;
  }
  return f;
}
template <typename FPS, typename T = typename FPS::T>
FPS operator/(const FPS &f, const SparseFPS<T> &g) {
  FPS res = f;
  res /= g;
  return res;
}

template <typename FPS, typename T = typename FPS::T>
FPS derivative(const FPS &f) {
  std::vector<T> res(f.size() - 1);
  for (int i = 1; i < f.size(); ++i) {
    res[i - 1] = f[i] * i;
  }
  return FPS(std::move(res));
}

template <typename FPS, typename T = typename FPS::T>
FPS integral(const FPS &f, const T &c) {
  std::vector<T> res(f.size() + 1);
  res[0] = c;
  for (int i = 1; i <= f.size(); ++i) {
    res[i] = f[i - 1] / i;
  }
  return FPS(std::move(res));
}

// log(f(x)) = ∫ f'(x)/f(x) dx
template <typename FPS, typename T = typename FPS::T>
FPS log(const FPS &f) {
  return integral(derivative(f) / f);
}

// (1 - x)^-n in O(D)
template <typename Factorials, typename FPS, typename T = typename FPS::T>
FPS negative_binom(int n, const Factorials &fs) {
  std::vector<T> coeff(FPS::dmax() + 1);
  for (int i = 0; i < (int)coeff.size(); ++i) {
    coeff[i] = fs.C(n - 1 + i, i);
  }
  return FPS(std::move(coeff));
}
