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

  static std::vector<T> invert(const std::vector<T> &x, int sz = -1) {
    if (sz == -1) sz = dmax() + 1;
    std::vector<T> res(sz);
    res[0] = T(1) / x[0];
    for (int i = 1; i < sz; ++i) {
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

  static std::vector<T> invert(const std::vector<T> &x, int sz = -1) {
    int n = x.size();
    assert(n != 0 && x[0].val() != 0);  // must be invertible
    if (sz == -1) sz = dmax() + 1;
    assert(sz > 0);
    std::vector<T> res{x[0].inv()};
    for (int m = 1, m2 = 2; m < sz; m = m2, m2 *= 2) {
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
    res.resize(sz);
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

  static std::vector<T> invert(const std::vector<T> &x, int sz = -1) {
    const int n = int(x.size());
    assert(n != 0 && x[0] != 0);
    if (sz == -1) sz = dmax() + 1;
    assert(sz > 0);
    std::vector<T> res{x[0].inv()};
    while (int(res.size()) < sz) {
      const int m = res.size();
      const int m2 = m << 1;
      std::vector<T> f(x.begin(), x.begin() + std::min<int>(n, m2));
      f.resize(m2);
      res.resize(m2);
      std::vector<T> s = convolution(f, res, m2);
      s.resize(m2);
      for (int i = 0; i < m2; i++) s[i] = -s[i];
      s[0] += 2;
      res = convolution(s, res, m2);
      res.resize(m2);
    }
    res.resize(sz);
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

  // Removes trailing zeros.
  void shrink() {
    while (coeff_.size() > 1 and coeff_.back() == T(0)) coeff_.pop_back();
  }

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

  // Multiplies by x^k (with truncation).
  void shift_inplace(int k) {
    if (k > 0) {
      if (size() <= dmax()) {
        coeff_.resize(std::min(size() + k, dmax() + 1), 0);
      }
      for (int i = size() - 1; i >= k; --i) {
        coeff_[i] = coeff_[i - k];
      }
      for (int i = k - 1; i >= 0; --i) {
        coeff_[i] = 0;
      }
    } else if (k < 0) {
      // If coefficients of degrees higher than dmax() were truncated
      // beforehand, you lose the information. Ensure dmax() is big enough.
      coeff_.erase(coeff_.begin(), coeff_.begin() + std::min(-k, size()));
    }
  }
  // Multiplies by x^k.
  DenseFPS shift(int k) const {
    DenseFPS res = *this;
    res.shift_inplace(k);
    return res;
  }

  DenseFPS inv(int size = -1) const {}
  DenseFPS &operator/=(const T &scalar) {
    T d_inv = T(1) / scalar;
    for (auto &x : coeff_) x *= d_inv;
    return *this;
  }
  friend DenseFPS operator/(const DenseFPS &f, const T &scalar) {
    return DenseFPS(f) /= scalar;
  }
  friend DenseFPS operator/(const T &scalar, const DenseFPS &g) {
    return DenseFPS{scalar} /= g;
  }
  DenseFPS &operator/=(const DenseFPS &other) {
    int z = 0;
    const int msz = std::min(size(), other.size());
    while (z < msz and (*this)[z] == T(0) and other[z] == T(0)) ++z;
    if (z == size()) {
      return *this;  // (0/y) == 0 regardless of y.
    }
    if (z == 0) {
      return *this *= DenseFPS(Mult::invert(other.coeff_));
    } else {
      shift_inplace(-z);
      std::vector<T> y(other.coeff_.begin() + std::min(z, other.size()),
                       other.coeff_.end());
      return *this *= DenseFPS(Mult::invert(std::move(y)));
    }
  }
  friend DenseFPS operator/(const DenseFPS &f, const DenseFPS &g) {
    return DenseFPS(f) /= g;
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

// Multiplies by (1 + c * x^k).
template <typename FPS>
void multiply2_inplace(FPS &f, int k, int c) {
  assert(k > 0);
  if (f.size() <= FPS::dmax()) {
    f.coeff_.resize(std::min(f.size() + k, FPS::dmax() + 1), 0);
  }
  for (int i = f.size() - 1; i >= k; --i) {
    f.coeff_[i] += f.coeff_[i - k] * c;
  }
}
// Multiplies by (1 + c * x^k).
template <typename FPS>
FPS multiply2(FPS f, int k, int c) {
  auto res = std::move(f);
  res.multiply2_inplace(k, c);
  return res;
}

// Divides by (1 + c * x^k).
template <typename FPS>
void divide2_inplace(FPS &f, int k, int c) {
  assert(k > 0);
  for (int i = k; i < f.size(); ++i) {
    f.coeff_[i] -= f.coeff_[i - k] * c;
  }
}
// Divides by (1 + c * x^k).
template <typename FPS>
FPS divide2(FPS f, int k, int c) {
  auto res = std::move(f);
  res.divide2_inplace(k, c);
  return res;
}

// Computes `f(a)` evaluating an FPS as a polynomial.
template <typename FPS, typename T = typename FPS::T>
T eval(const FPS &f, const T a) {
  T res = 0, x = 1;
  for (auto c : f.coeff_) {
    res += c * x;
    x *= a;
  }
  return res;
}

template <typename FPS, typename T = typename FPS::T>
FPS derivative(const FPS &f) {
  std::vector<T> res(std::max<int>(f.size() - 1, 1));
  for (int i = 1; i < f.size(); ++i) {
    res[i - 1] = f[i] * i;
  }
  return FPS(std::move(res));
}

template <typename FPS, typename T = typename FPS::T>
FPS integral(const FPS &f, T c0 = 0) {
  const int n = int(f.size());
  assert(n >= 1);
  std::vector<T> res(n + 1);
  res[0] = c0;
  res[1] = f[0];
  if (n >= 2) {
    std::vector<T> inv(n + 1);
    inv[1] = 1;
    for (int i = 2; i <= n; ++i) {
      auto d = std::div(T::mod(), i);
      inv[i] = -inv[d.rem] * d.quot;
    }
    for (int i = 2; i <= n; ++i) {
      res[i] = f.coeff_[i - 1] * inv[i];
    }
  }
  return FPS(std::move(res));
}

template <typename FPS, typename T = typename FPS::T>
FPS log(const FPS &f, int sz = -1) {
  if (sz == -1) sz = FPS::dmax() + 1;
  FPS g = derivative(f) * f.inv(sz);
  g.coeff_.resize(sz);
  return integral(g);
}

template <typename FPS, typename T = typename FPS::T>
FPS exp(FPS g) {
  int n = g.size();
  FPS ret = {1};
  for (int i = 1; i < n; i <<= 1) {
    FPS f(std::vector<T>(i, 0));
    FPS logr = log(ret, i << 1);
    for (int j = 0; j < i; ++j) {
      f.coeff_[j] += g[i + j] - logr[i + j];
    }
    f *= ret;
    std::copy(f.coeff_.begin(), f.coeff_.begin() + i,
              std::back_inserter(ret.coeff_));
  }
  ret.coeff_.resize(n);
  return ret;
}

template <typename FPS, typename T = typename FPS::T>
FPS exp_ntt(FPS f) {
  static_assert(T::mod() == 998244353, "Requires an NTT-friendly mod.");
  const int n = int(f.size());
  assert(n > 0 && f[0] == 0);
  std::vector<T> h_drv = std::move(derivative(f).coeff_);
  std::vector<T> g{1}, g_fft{0}, res(std::move(f.coeff_));
  res[0] = 1;
  for (int m = 1; m < n; m <<= 1) {
    const int m2 = m << 1;
    const T m2_inv = Mint(m2).inv();
    std::vector<T> f_fft(res.begin(), res.begin() + m);
    std::vector<T> t = std::move(derivative(FPS(f_fft)).coeff_);
    t.resize(m);
    f_fft.resize(m2);
    atcoder::internal::butterfly(f_fft);

    // Step 2.a'
    if (m > 1) {
      std::vector<T> _f(m);
      for (int i = 0; i < m; ++i) _f[i] = f_fft[i] * g_fft[i];
      atcoder::internal::butterfly_inv(_f);
      _f.erase(_f.begin(), _f.begin() + (m >> 1));
      _f.resize(m);
      atcoder::internal::butterfly(_f);
      for (int i = 0; i < m; ++i) _f[i] *= g_fft[i];
      atcoder::internal::butterfly_inv(_f);
      _f.resize(m / 2);
      T di = (T(-m) * m).inv();
      for (int i = 0, q = m >> 1; i < q; ++i) {
        g.push_back(_f[i] * di);
      }
    }

    // Step 2.b'--d'
    {
      // Step 2.b'
      std::vector<T> r(h_drv.begin(), h_drv.begin() + (m - 1));
      // Step 2.c'
      r.resize(m);
      atcoder::internal::butterfly(r);
      for (int i = 0; i < m; ++i) r[i] *= f_fft[i];
      atcoder::internal::butterfly_inv(r);
      // Step 2.d'
      auto mm = Mint(-m).inv();
      for (int i = 0; i < m; ++i) t[i] += r[i] * mm;
      std::rotate(t.begin(), t.begin() + (m - 1), t.end());
    }

    // Step 2.e'
    t.resize(m2);
    atcoder::internal::butterfly(t);
    g_fft = g;
    g_fft.resize(m2);
    atcoder::internal::butterfly(g_fft);
    for (int i = 0; i < m2; ++i) t[i] *= g_fft[i];
    atcoder::internal::butterfly_inv(t);
    t.resize(m);
    for (auto &x : t) x *= m2_inv;

    // Step 2.f'
    std::vector<T> v(res.begin() + m, res.begin() + std::min(n, m2));
    v.resize(m);
    t.insert(t.begin(), m - 1, 0);
    t = std::move(integral(FPS(std::move(t))).coeff_);
    for (int i = 0; i < m; ++i) v[i] -= t[m + i];

    // Step 2.g'
    v.resize(m2);
    atcoder::internal::butterfly(v);
    for (int i = 0; i < m2; ++i) v[i] *= f_fft[i];
    atcoder::internal::butterfly_inv(v);
    v.resize(m);
    for (auto &x : v) x *= m2_inv;

    // Step 2.h'
    for (int i = 0, q = std::min(n - m, m); i < q; ++i) {
      res[m + i] = v[i];
    }
  }
  return FPS(std::move(res));
}

template <typename FPS>
FPS pow_binexp(FPS base, long long t) {
  assert(t >= 0);
  FPS res = {1};
  while (t) {
    if (t & 1) res *= base;
    base *= base;
    t >>= 1;
  }
  return res;
}

// Fast power
template <typename FPS, typename T = typename FPS::T>
FPS pow_logexp(FPS f, long long k) {
  assert(k >= 0);
  if (k == 0) return FPS{1};
  if (k == 1) return f;
  const int n = int(f.size());
  int l = 0;
  while (l < n && f[l] == 0) ++l;
  if (l > (n - 1) / k) return FPS{};
  T c = f[l];
  if (l != 0) f.coeff_.erase(f.coeff_.begin(), f.coeff_.begin() + l);
  if (c != 1) f /= c;
  f = log(std::move(f));
  const long long ssz = FPS::dmax() + 1 - l * k;
  if ((long long)f.size() > ssz) f.coeff_.resize(ssz);  // shrink
  f = exp(std::move(f) * k);
  if (c != 1) f *= c.pow(k);
  if (l != 0) f.coeff_.insert(f.coeff_.begin(), size_t(l * k), T(0));
  return f;
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

namespace fps_internal {

std::optional<long long> sqrt_mod(long long a, int p) {
  if (a == 0) return 0;
  if (p == 2) return a;
  if (atcoder::pow_mod(a, (p - 1) >> 1, p) != 1) return std::nullopt;
  long long b = 1;
  while (atcoder::pow_mod(b, (p - 1) >> 1, p) == 1) ++b;
  long long e = 0, m = p - 1;
  while (m % 2 == 0) m >>= 1, ++e;
  auto x = atcoder::pow_mod(a, (m - 1) >> 1, p);
  auto y = a * (x * x % p) % p;
  (x *= a) %= p;
  auto z = atcoder::pow_mod(b, m, p);
  while (y != 1) {
    long long j = 0, t = y;
    while (t != 1) {
      j += 1;
      (t *= t) %= p;
    }
    z = atcoder::pow_mod(z, 1LL << (e - j - 1), p);
    (x *= z) %= p;
    (z *= z) %= p;
    (y *= z) %= p;
    e = j;
  }
  return x;
}

template <typename FPS, typename T = typename FPS::T>
FPS sqrt_fft(const FPS &f_square) {
  static_assert(T::mod() == 998244353, "Requires an NTT-friendly mod.");
  assert(f_square[0] == T(1));
  static const T kHalf = T(1) / 2;
  std::vector<T> f{1}, g{1}, z{1};
  T n2_inv = 1;
  for (int n = 1; n <= FPS::dmax(); n *= 2) {
    for (int i = 0; i < n; ++i) z[i] *= z[i];
    atcoder::internal::butterfly_inv(z);
    for (int i = 0; i < n; ++i) z[i] *= n2_inv;

    const int n2 = n * 2;
    n2_inv *= kHalf;

    std::vector<T> delta(n2);
    for (int i = 0; i < n; ++i)
      delta[n + i] = z[i] - f_square[i] - f_square[n + i];
    atcoder::internal::butterfly(delta);

    std::vector<T> gbuf(n2);
    for (int i = 0; i < n; ++i) gbuf[i] = g[i];
    atcoder::internal::butterfly(gbuf);

    for (int i = 0; i < n2; ++i) delta[i] *= gbuf[i];
    atcoder::internal::butterfly_inv(delta);
    for (int i = 0; i < n2; ++i) delta[i] *= n2_inv;
    f.resize(n2);
    for (int i = n; i < n2; ++i) f[i] = -delta[i] * kHalf;
    if (n2 > FPS::dmax()) break;

    z = f;
    atcoder::internal::butterfly(z);

    std::vector<T> eps = gbuf;
    for (int i = 0; i < n2; ++i) eps[i] *= z[i];
    atcoder::internal::butterfly_inv(eps);
    for (int i = 0; i < n; ++i) eps[i] = 0;
    for (int i = n; i < n2; ++i) eps[i] *= n2_inv;
    atcoder::internal::butterfly(eps);
    for (int i = 0; i < n2; ++i) eps[i] *= gbuf[i];
    atcoder::internal::butterfly_inv(eps);
    for (int i = 0; i < n2; ++i) eps[i] *= n2_inv;
    g.resize(n2);
    for (int i = n; i < n2; ++i) g[i] -= eps[i];
  }
  if ((int)f.size() > FPS::dmax() + 1) {
    f.resize(FPS::dmax() + 1);
  }
  return FPS(std::move(f));
}
}  // namespace fps_internal

template <typename FPS, typename T = typename FPS::T>
std::optional<FPS> sqrt(const FPS &f) {
  // fast path
  if (f[0].val() == 1) return fps_internal::sqrt_fft(f);

  int z = 0;
  while (z < f.size() && f[z].val() == 0) ++z;
  if (z == f.size()) return FPS{0};
  if (z % 2 == 1) return std::nullopt;
  const auto f0 = f[z];
  const auto oc0 = fps_internal::sqrt_mod(f0.val(), T::mod());
  if (not oc0.has_value()) return std::nullopt;
  const auto c0 = oc0.value();
  auto g = FPS(std::vector<T>(f.coeff_.begin() + z, f.coeff_.end()));
  g = fps_internal::sqrt_fft(g / f0);
  const int zhalf = z / 2;
  const int sz = std::min<int>(g.size() + zhalf, FPS::dmax() + 1);
  std::vector<T> res(sz);
  for (int i = zhalf; i < sz; ++i) res[i] = g[i - zhalf] * c0;
  return FPS(std::move(res));
}
