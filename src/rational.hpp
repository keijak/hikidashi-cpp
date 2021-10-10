#include <bits/stdc++.h>

// #include <boost/multiprecision/cpp_int.hpp>
// using multip = boost::multiprecision;

template <typename T>
struct Rational {
 public:
  using BigInt = __int128_t;
  // using BigInt = multip::checked_int128_t; // for testing overflow

  T nume, deno;

  Rational() : nume(0), deno(1) {}

  Rational(T n) : nume(n), deno(1) {}

  Rational(BigInt n, BigInt d) {
    assert(d != 0);
    if (n == 0) {
      d = 1;
    } else {
      const int sign = ((n < 0) xor (d < 0)) ? -1 : 1;
      if (n < 0) n = -n;
      if (d < 0) d = -d;
      const auto g = gcd_(n, d);
      if (g > 1) {
        n /= g;
        d /= g;
      }

      // In Codeforces, numeric_limits<__int128_t>::max() returns 0 :-(
      // assert(n <= std::numeric_limits<T>::max());
      // assert(d <= std::numeric_limits<T>::max());

      n *= sign;
    }
    nume = static_cast<T>(n);
    deno = static_cast<T>(d);
  }

  Rational(const Rational &x) = default;
  Rational(Rational &&x) = default;
  Rational &operator=(const Rational &x) = default;
  Rational &operator=(Rational &&x) = default;

  explicit operator double() const { return (double)nume / (double)deno; }
  explicit operator long double() const {
    return (long double)nume / (long double)deno;
  }

  Rational operator+() const { return *this; }
  Rational operator-() const { return Rational(-nume, deno); }

  friend bool operator==(const Rational &x, const Rational &y) {
    return (x.nume == y.nume) and (x.deno == y.deno);
  }
  friend bool operator!=(const Rational &x, const Rational &y) {
    return not(x == y);
  }
  friend bool operator<(const Rational &x, const Rational &y) {
    return static_cast<BigInt>(x.nume) * y.deno <
           static_cast<BigInt>(y.nume) * x.deno;
  }
  friend bool operator>(const Rational &x, const Rational &y) { return y < x; }
  friend bool operator<=(const Rational &x, const Rational &y) {
    return not(x > y);
  }
  friend bool operator>=(const Rational &x, const Rational &y) {
    return not(x < y);
  }

  friend Rational operator+(const Rational &x, const Rational &y) {
    auto g = gcd_(x.deno, y.deno);
    auto zn = (static_cast<BigInt>(x.nume) * (y.deno / g)) +
              (static_cast<BigInt>(y.nume) * (x.deno / g));
    auto zd = static_cast<BigInt>(x.deno / g) * y.deno;
    return Rational(std::move(zn), std::move(zd));
  }
  Rational &operator+=(const Rational &x) { return (*this = *this + x); }

  friend Rational operator-(const Rational &x, const Rational &y) {
    return x + (-y);
  }
  Rational &operator-=(const Rational &x) { return (*this = *this - x); }

  friend Rational operator*(const Rational &x, const Rational &y) {
    if (x.nume == 0 or y.nume == 0) return Rational(0);
    auto g1 = gcd_(abs_(x.nume), y.deno);
    auto g2 = gcd_(abs_(y.nume), x.deno);
    return Rational(BigInt(x.nume / g1) * BigInt(y.nume / g2),
                    BigInt(x.deno / g2) * BigInt(y.deno / g1));
  }
  Rational &operator*=(const Rational &x) { return (*this = *this * x); }

  Rational inv() const { return Rational(deno, nume); }

  friend Rational operator/(const Rational &x, const Rational &y) {
    return x * y.inv();
  }
  Rational &operator/=(const Rational &x) { return (*this = *this / x); }

  friend std::ostream &operator<<(std::ostream &os, const Rational &x) {
    return os << x.nume << "/" << x.deno;
  }

 private:
  template <typename U>
  static inline U abs_(const U &x) {
    return std::abs(x);
    // return (x < 0) ? -x : x;
  }
  template <typename U>
  static inline U gcd_(const U &a, const U &b) {
    return std::gcd(a, b);
    // return (b == 0) ? a : gcd_(b, a % b);
  }
};
using Rat = Rational<long long>;
// using Rat = Rational<multip::checked_int128_t>; // for testing overflow

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
