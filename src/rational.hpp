#include <bits/stdc++.h>

template <typename T = long long>
struct Rational {
 private:
  using BigInt = __int128_t;

 public:
  T nume, deno;

  Rational() : nume(0), deno(1) {}

  explicit Rational(T n) : nume(n), deno(1) {}

  Rational(T n, T d) : nume(n), deno(d) {
    assert(deno != 0);
    if (nume == 0) {
      deno = 1;
    } else {
      int sign = 1;
      if ((nume < 0) xor (deno < 0)) {
        sign = -1;
      }
      nume = abs(nume);
      deno = abs(deno);
      auto g = std::gcd(nume, deno);
      if (g > 1) {
        nume /= g;
        deno /= g;
      }
      nume *= sign;
    }
  }

  operator double() { return (double)(nume) / deno; }
  operator long double() { return (long double)(nume) / deno; }

  friend bool operator<(const Rational &x, const Rational &y) {
    return (BigInt)(x.nume) * y.deno < (BigInt)(y.nume) * x.deno;
  }

  friend bool operator==(const Rational &x, const Rational &y) {
    return std::tie(x.nume, x.deno) == std::tie(y.nume, y.deno);
  }

  friend Rational operator+(const Rational &x, const Rational &y) {
    auto g = std::gcd(x.deno, y.deno);
    auto zn = x.nume * (y.deno / g) + y.nume * (x.deno / g);
    auto zd = (x.deno / g) * y.deno;
    return Rational(zn, zd);
  }

  friend Rational operator*(const Rational &x, const Rational &y) {
    auto g1 = std::gcd(std::abs(x.nume), y.deno);
    auto g2 = std::gcd(std::abs(y.nume), x.deno);
    return Rational((x.nume / g1) * (y.nume / g2),
                    (x.deno / g2) * (y.deno / g1));
  }

  friend std::ostream &operator<<(std::ostream &os, const Rational &x) {
    return os << x.nume << "/" << x.deno;
  }
};
