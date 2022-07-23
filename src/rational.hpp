#include <bits/stdc++.h>

// Rational number with lazy reduction.
struct LazyRational {
  using i64 = long long;
  using i128 = __int128_t;

  mutable i64 nume_;
  mutable i64 deno_;
  mutable bool reduced_;

  static constexpr i64 kReduceThreshold = i64(1) << 62;

  LazyRational() : nume_(0), deno_(1), reduced_(true) {}
  LazyRational(i64 n) : nume_(n), deno_(1), reduced_(true) {}
  LazyRational(i64 n, i64 d, bool reduced)
      : nume_(n), deno_(d), reduced_(reduced) {}

  template <typename T>
  LazyRational(T n, T d) : reduced_(false) {
    assert(d != 0);  // zero division
    if (n == 0) {
      d = 1;
      reduced_ = true;
    } else {
      reduced_ = light_reduce_(n, d);
    }
    nume_ = static_cast<i64>(n);
    deno_ = static_cast<i64>(d);
  }

  i64 numerator() const {
    if (not reduced_) {
      reduce_(nume_, deno_);
      reduced_ = true;
    }
    return nume_;
  }

  i64 denominator() const {
    if (not reduced_) {
      reduce_(nume_, deno_);
      reduced_ = true;
    }
    return deno_;
  }

  // Cast to a floating point number.
  template <typename Float>
  explicit operator Float() const {
    static_assert(std::is_floating_point<Float>::value);
    return (Float)nume_ / (Float)deno_;
  }

  const LazyRational &operator+() const { return *this; }

  LazyRational operator-() const {
    return LazyRational(-nume_, deno_, reduced_);
  }

  LazyRational &operator+=(const LazyRational &y) {
    if (y.nume_ == 0) return *this;
    if (this->nume_ == 0) {
      *this = y;
      return *this;
    }
    i128 zn = i128(this->nume_) * y.deno_ + i128(y.nume_) * this->deno_;
    i128 zd = i128(this->deno_) * y.deno_;
    this->reduced_ = light_reduce_(zn, zd);
    this->nume_ = static_cast<i64>(zn);
    this->deno_ = static_cast<i64>(zd);
    return *this;
  }
  friend LazyRational operator+(LazyRational x, const LazyRational &y) {
    x += y;
    return x;
  }

  friend LazyRational operator-(const LazyRational &x, const LazyRational &y) {
    return x + (-y);
  }
  LazyRational &operator-=(const LazyRational &y) {
    *this += -y;
    return *this;
  }

  LazyRational &operator*=(const LazyRational &y) {
    if (this->nume_ == 0) return *this;
    if (y.nume_ == 0) {
      this->nume_ = 0;
      this->deno_ = 1;
      this->reduced_ = true;
    } else {
      i128 zn = i128(this->nume_) * y.nume_;
      i128 zd = i128(this->deno_) * y.deno_;
      this->reduced_ = light_reduce_(zn, zd);
      this->nume_ = static_cast<i64>(zn);
      this->deno_ = static_cast<i64>(zd);
    }
    return *this;
  }
  friend LazyRational operator*(LazyRational x, const LazyRational &y) {
    x *= y;
    return x;
  }

  LazyRational inv() const {
    int sign = this->nume_ >= 0 ? 1 : -1;
    return LazyRational(this->deno_ * sign, this->nume_ * sign, this->reduced_);
  }
  friend LazyRational operator/(const LazyRational &x, const LazyRational &y) {
    return x * y.inv();
  }
  LazyRational &operator/=(const LazyRational &y) {
    *this *= y.inv();
    return *this;
  }

  friend bool operator==(const LazyRational &x, const LazyRational &y) {
    return i128(x.nume_) * y.deno_ == i128(x.deno_) * y.nume_;
  }
  friend bool operator!=(const LazyRational &x, const LazyRational &y) {
    return not(x == y);
  }
  friend bool operator<(const LazyRational &x, const LazyRational &y) {
    return i128(x.nume_) * y.deno_ < i128(x.deno_) * y.nume_;
  }
  friend bool operator>(const LazyRational &x, const LazyRational &y) {
    return y < x;
  }
  friend bool operator<=(const LazyRational &x, const LazyRational &y) {
    return not(x > y);
  }
  friend bool operator>=(const LazyRational &x, const LazyRational &y) {
    return not(x < y);
  }

  friend std::ostream &operator<<(std::ostream &os, const LazyRational &x) {
    return os << x.numerator() << "/" << x.denominator();
  }

 private:
  template <typename T>
  static T abs_(const T &x) {
    if constexpr (std::is_same_v<T, i128>) {
      return x < 0 ? -x : x;
    } else {
      return std::abs(x);
    }
  }

  template <typename T>
  static void reduce_(T &n, T &d) {
    if (n == 0) {
      d = 1;
      return;
    }
    i128 a = abs_(n), b = d;
    while (b) {
      a %= b;
      std::swap(a, b);
    }
    if (a > 1) {
      n /= static_cast<T>(a);
      d /= static_cast<T>(a);
    }
  }

  // Returns whether the args are reduced.
  template <typename T>
  static bool light_reduce_(T &n, T &d) {
    if (std::max<T>(abs_(n), d) <= kReduceThreshold) {
      return false;
    }
    reduce_(n, d);
    if constexpr (std::is_same_v<T, i128>) {
      if (std::max<T>(abs_(n), d) > std::numeric_limits<i64>::max()) {
        throw std::overflow_error("cannot fit in 64 bits");
      }
    }
    return true;
  }
};
using Rat = LazyRational;

// Parse a decimal number into a rational.
// (e.g. "0.2029" => 2029/10000 )
std::istream &operator>>(std::istream &is, Rat &x) {
  long long nume = 0, deno = 1;
  char ch;
  while (is.get(ch)) {
    if (not std::isspace(ch)) break;
  }
  int sgn = 1;
  if (ch == '-') {
    sgn = -1;
    is.get(ch);
  }
  bool in_frac = false;
  while (true) {
    if (not std::isdigit(ch)) {
      is.unget();
      break;
    }
    nume = (nume * 10) + int(ch - '0');
    if (in_frac) deno *= 10;
    if (not(is.get(ch))) break;
    if (ch == '.') {
      in_frac = true;
      if (not(is.get(ch))) break;
    }
  }
  x = Rat(nume * sgn, deno);
  return is;
}
