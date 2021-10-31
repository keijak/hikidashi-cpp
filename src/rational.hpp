#include <bits/stdc++.h>

// Rational number
template <typename T, bool allow_infinity = false>
struct Rational {
 public:
  // #include <boost/multiprecision/cpp_int.hpp>
  // using multip = boost::multiprecision;
  // using BigInt = boost::multiprecision::checked_int128_t;
  using BigInt = __int128_t;

  T nume, deno;

  Rational() : nume(0), deno(1) {}

  Rational(T n) : nume(n), deno(1) {}

  Rational(BigInt n, BigInt d) {
    if constexpr (allow_infinity) {
      assert(d != 0 or n != 0);  // 0/0 is undefined
    } else {
      assert(d != 0);
    }
    if (allow_infinity and d == 0) {
      n = (n < 0) ? -1 : 1;  // infinity
    } else if (n == 0) {
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
      n *= sign;
    }
    nume = static_cast<T>(n);
    deno = static_cast<T>(d);
  }

  Rational(const Rational &x) = default;
  Rational(Rational &&x) = default;
  Rational &operator=(const Rational &x) = default;
  Rational &operator=(Rational &&x) = default;

  // Cast to a floating point number.
  template <typename Float>
  explicit operator Float() const {
    static_assert(std::is_floating_point<Float>::value);
    return (Float)nume / (Float)deno;
  }

  Rational operator+() const { return *this; }
  Rational operator-() const {
    Rational ret;
    ret.nume = -nume;
    ret.deno = deno;
    return ret;
  }

  friend bool operator==(const Rational &x, const Rational &y) {
    return (x.nume == y.nume) and (x.deno == y.deno);
  }
  friend bool operator!=(const Rational &x, const Rational &y) {
    return not(x == y);
  }
  friend bool operator<(const Rational &x, const Rational &y) {
    if constexpr (allow_infinity) {
      if (x.deno == 0 and y.deno == 0) return x.nume < y.nume;
    }
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
    if constexpr (allow_infinity) {
      if (x.deno == 0 and y.deno == 0) {
        assert(x.nume == y.nume);  // (infinity - infinity) is undefined
        return x;
      }
      if (x.deno == 0) return x;
      if (y.deno == 0) return y;
    }
    auto g = gcd_(x.deno, y.deno);
    BigInt xd = x.deno / g;
    BigInt yd = y.deno / g;
    BigInt zn = x.nume * yd + y.nume * xd;
    BigInt zd = xd * y.deno;
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
    // return std::abs(x);
    // return boost::multiprecision::abs(x);
    return (x < 0) ? -x : x;
  }
  template <typename U>
  static U gcd_(const U &a, const U &b) {
    // return std::gcd(a, b);
    return (b == 0) ? a : gcd_(b, a % b);
  }
};
using Rat = Rational<long long, false>;
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
