#include <bits/stdc++.h>

#include <atcoder/convolution>
#include <atcoder/modint>

using i64 = long long;
using u64 = unsigned long long;

// Formal Power Series (dense format).
template <typename T, int MAX_DEGREE>
struct DenseFPS {
  // Coefficients of terms from x^0 to x^MAX_DEGREE.
  std::vector<T> coeff_;

  DenseFPS() : coeff_(MAX_DEGREE + 1) {}  // zero-initialized
  explicit DenseFPS(std::vector<T> c) : coeff_(std::move(c)) {
    assert((int)coeff_.size() == MAX_DEGREE + 1);
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

  static constexpr int size() { return MAX_DEGREE + 1; }

  // Returns the coefficient of x^d.
  const T &operator[](int d) const { return coeff_[d]; }

  DenseFPS &operator+=(const T &scalar) {
    for (auto &x : coeff_) x += scalar;
    return *this;
  }
  friend DenseFPS operator+(const DenseFPS &x, const T &scalar) {
    DenseFPS res = x;
    res += scalar;
    return res;
  }
  DenseFPS &operator+=(const DenseFPS &other) {
    for (int i = 0; i < size(); ++i) coeff_[i] += other[i];
    return *this;
  }
  friend DenseFPS operator+(const DenseFPS &x, const DenseFPS &y) {
    DenseFPS res = x;
    res += y;
    return res;
  }

  DenseFPS &operator-=(const DenseFPS &other) {
    for (int i = 0; i < size(); ++i) coeff_[i] -= other[i];
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

 private:
  // Naive multiplication. O(N^2)
  DenseFPS mul_naive(const DenseFPS &other) const {
    DenseFPS res;
    for (int i = 0; i < size(); ++i) {
      for (int j = 0; i + j < size(); ++j) {
        res.coeff_[i + j] += (*this)[i] * other[j];
      }
    }
    return res;
  }
};

// Fast polynomial multiplication by single NTT.
template <typename ModInt, int MAX_DEGREE>
DenseFPS<ModInt, MAX_DEGREE> fps_mul_ntt(
    const DenseFPS<ModInt, MAX_DEGREE> &x,
    const DenseFPS<ModInt, MAX_DEGREE> &y) {
  static_assert(ModInt::mod() != 1'000'000'007);  // Must be NTT-friendly MOD!
  auto z = atcoder::convolution(x.coeff_, y.coeff_);
  z.resize(MAX_DEGREE + 1);  // Maybe shrink.
  return {std::move(z)};
}

// Polynomial multiplication by NTT + Garner (arbitrary mod).
template <typename ModInt, int MAX_DEGREE>
DenseFPS<ModInt, MAX_DEGREE> fps_mul_mod(
    const DenseFPS<ModInt, MAX_DEGREE> &x,
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
    res.coeff_[i] = zll[i];
  }
  return res;
}

// Polynomial multiplication by NTT + Garner (long long).
template <int MAX_DEGREE>
DenseFPS<i64, MAX_DEGREE> fps_mul_ll(const DenseFPS<i64, MAX_DEGREE> &x,
                                     const DenseFPS<i64, MAX_DEGREE> &y) {
  auto z = atcoder::convolution_ll(x.coeff_, y.coeff_);
  z.resize(MAX_DEGREE + 1);  // Maybe shrink.
  return DenseFPS<i64, MAX_DEGREE>(std::move(z));
}

template <typename T, int MAX_DEGREE, typename Func>
DenseFPS<T, MAX_DEGREE> fps_pow_generic(const DenseFPS<T, MAX_DEGREE> &x, u64 t,
                                        Func mulfunc) {
  DenseFPS<T, MAX_DEGREE> base = x, res;
  res.coeff_[0] = 1;
  while (t) {
    if (t & 1) res = mulfunc(res, base);
    base = mulfunc(base, base);
    t >>= 1;
  }
  return res;
}

template <typename ModInt, int MAX_DEGREE>
DenseFPS<ModInt, MAX_DEGREE> fps_pow_ntt(const DenseFPS<ModInt, MAX_DEGREE> &x,
                                         u64 t) {
  return fps_pow_generic(x, t, fps_mul_ntt);
}

template <typename ModInt, int MAX_DEGREE>
DenseFPS<ModInt, MAX_DEGREE> fps_pow_mod(const DenseFPS<ModInt, MAX_DEGREE> &x,
                                         u64 t) {
  return fps_pow_generic(x, t, fps_mul_mod);
}

template <int MAX_DEGREE>
DenseFPS<i64, MAX_DEGREE> fps_pow_ll(const DenseFPS<i64, MAX_DEGREE> &x,
                                     u64 t) {
  return fps_pow_generic(x, t, fps_mul_ll);
}

// Formal Power Series (sparse format).
template <typename T>
class SparseFPS {
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
  inline int degree(int i) const { return degree_[i]; }
  inline const T &coeff(int i) const { return coeff_[i]; }

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
    int j = 0;
    for (int i = 0; i < size();) {
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
template <typename ModInt, int MAX_DEGREE>
DenseFPS<ModInt, MAX_DEGREE> &operator+=(DenseFPS<ModInt, MAX_DEGREE> &x,
                                         const SparseFPS<ModInt> &y) {
  for (int i = 0; i < y.size(); ++i) {
    if (y.degree(i) > MAX_DEGREE) break;  // ignore
    x.coeff_[y.degree(i)] += y.coeff(i);
  }
  return x;
}
template <typename ModInt, int MAX_DEGREE>
DenseFPS<ModInt, MAX_DEGREE> operator+(const DenseFPS<ModInt, MAX_DEGREE> &x,
                                       const SparseFPS<ModInt> &y) {
  DenseFPS<ModInt, MAX_DEGREE> res = x;
  res += y;
  return res;
}
template <typename ModInt, int MAX_DEGREE>
DenseFPS<ModInt, MAX_DEGREE> operator+(const SparseFPS<ModInt> &x,
                                       const DenseFPS<ModInt, MAX_DEGREE> &y) {
  return y + x;  // commutative
}

// Polynomial multiplication (dense * sparse).
template <typename ModInt, int MAX_DEGREE>
DenseFPS<ModInt, MAX_DEGREE> &operator*=(DenseFPS<ModInt, MAX_DEGREE> &x,
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
  for (int i = MAX_DEGREE; i >= 0; --i) {
    x.coeff_[i] *= c0;
    for (int j = j0; j < y.size(); ++j) {
      int d = y.degree(j);
      if (d > i) break;
      x.coeff_[i] += x.coeff_[i - d] * y.coeff[j];
    }
  }
  return x;
}
template <typename ModInt, int MAX_DEGREE>
DenseFPS<ModInt, MAX_DEGREE> operator*(const DenseFPS<ModInt, MAX_DEGREE> &x,
                                       const SparseFPS<ModInt> &y) {
  DenseFPS<ModInt, MAX_DEGREE> res = x;
  res *= y;
  return res;
}
template <typename ModInt, int MAX_DEGREE>
DenseFPS<ModInt, MAX_DEGREE> operator*(const SparseFPS<ModInt> &x,
                                       const DenseFPS<ModInt, MAX_DEGREE> &y) {
  return y * x;  // commutative
}

// Polynomial division (dense * sparse).
template <typename ModInt, int MAX_DEGREE>
DenseFPS<ModInt, MAX_DEGREE> &operator/=(DenseFPS<ModInt, MAX_DEGREE> &x,
                                         const SparseFPS<ModInt> &y) {
  assert(y.size() > 0 and y.degree(0) == 0 and y.coeff(0) != 0);
  ModInt inv_c0 = y.coeff(0).inv();
  for (int i = 0; i < x.size(); ++i) {
    for (int j = 1; j < y.size(); ++j) {
      int d = y.degree(j);
      if (d > i) break;
      x.coeff_[i] -= x.coeff_[i - d] * y.coeff[j];
    }
    x.coeff_[i] *= inv_c0;
  }
  return x;
}
template <typename ModInt, int MAX_DEGREE>
DenseFPS<ModInt, MAX_DEGREE> operator/(const DenseFPS<ModInt, MAX_DEGREE> &x,
                                       const SparseFPS<ModInt> &y) {
  DenseFPS<ModInt, MAX_DEGREE> res = x;
  res /= y;
  return res;
}
