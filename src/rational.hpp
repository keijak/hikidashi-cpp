#include <bits/stdc++.h>
using i64 = long long;

struct Rational {
  i64 nume, deno;

  Rational() : nume(0), deno(1) {}
  explicit Rational(i64 n) : nume(n), deno(1) {}
  Rational(i64 n, i64 d) : nume(n), deno(d) {
    assert(deno != 0);
    if (nume == 0) {
      deno = 1;
    } else {
      i64 sign = 1;
      if ((nume < 0) xor (deno < 0)) {
        sign = -1;
      }
      nume = abs(nume);
      deno = abs(deno);
      i64 g = std::gcd(nume, deno);
      if (g > 1) {
        nume /= g;
        deno /= g;
      }
      nume *= sign;
    }
  }
  Rational(const Rational &) = default;
  Rational(Rational &&) = default;
  Rational &operator=(const Rational &) = default;
  Rational &operator=(Rational &&) = default;

  operator double() { return double(nume) / deno; }
  operator long double() { return (long double)(nume) / deno; }
};
bool operator<(const Rational &x, const Rational &y) {
  return __int128_t(x.nume) * y.deno < __int128_t(y.nume) * x.deno;
}
std::ostream &operator<<(std::ostream &os, const Rational &x) {
  return os << x.nume << "/" << x.deno;
}
