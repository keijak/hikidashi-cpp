#include <bits/stdc++.h>
// Runtime MOD:
//   unsigned MOD = 998244353;  // modifiable.
//   template <unsigned& M> struct ModInt { ... };
//   using Mint = ModInt<MOD>;

template <unsigned M>
struct ModInt {
  constexpr ModInt() : _v{0} {}
  constexpr ModInt(long long val) : _v{(unsigned)((val % M + M) % M)} {}

  static constexpr int mod() { return M; }
  static constexpr unsigned umod() { return M; }
  inline unsigned val() const { return _v; }

  ModInt &operator++() {
    _v++;
    if (_v == umod()) _v = 0;
    return *this;
  }
  ModInt &operator--() {
    if (_v == 0) _v = umod();
    _v--;
    return *this;
  }
  ModInt operator++(int) {
    auto result = *this;
    ++*this;
    return result;
  }
  ModInt operator--(int) {
    auto result = *this;
    --*this;
    return result;
  }

  constexpr ModInt operator-() const { return ModInt(umod() - _v); }

  constexpr ModInt &operator+=(const ModInt &a) {
    if ((_v += a._v) >= M) _v -= M;
    return *this;
  }
  constexpr ModInt &operator-=(const ModInt &a) {
    if ((_v += M - a._v) >= M) _v -= M;
    return *this;
  }
  constexpr ModInt &operator*=(const ModInt &a) {
    _v = ((unsigned long long)(_v)*a._v) % M;
    return *this;
  }
  constexpr ModInt pow(long long t) const {
    if (_v == 0) {
      return 0;  // corner case: 0^0 = ?
    }
    if (t < 0) {
      return this->inv().pow(-t);
    }
    ModInt base = *this;
    ModInt res = 1;
    while (t) {
      if (t & 1) res *= base;
      base *= base;
      t >>= 1;
    }
    return res;
  }

  ModInt inv() const {
    static constexpr unsigned kPrecalcSize = 10000;
    static const std::array<ModInt, kPrecalcSize> *kPrecalc = []() {
      auto *a = new std::array<ModInt, kPrecalcSize>;
      for (ModInt i = 1; i.val() < kPrecalcSize; i += 1) {
        (*a)[i.val()] = i.inv_internal();
      }
      return a;
    }();
    if (_v < kPrecalcSize) return (*kPrecalc)[_v];
    return inv_internal();
  }
  ModInt &operator/=(const ModInt &a) { return *this *= a.inv(); }

  friend constexpr ModInt operator+(const ModInt &a, const ModInt &b) {
    ModInt r = a;
    r += b;
    return r;
  }
  friend constexpr ModInt operator-(const ModInt &a, const ModInt &b) {
    ModInt r = a;
    r -= b;
    return r;
  }
  friend constexpr ModInt operator*(const ModInt &a, const ModInt &b) {
    ModInt r = a;
    r *= b;
    return r;
  }
  friend constexpr ModInt operator/(const ModInt &a, const ModInt &b) {
    ModInt r = a;
    r /= b;
    return r;
  }
  friend constexpr bool operator==(const ModInt &a, const ModInt &b) {
    return a._v == b._v;
  }
  friend constexpr bool operator!=(const ModInt &a, const ModInt &b) {
    return a._v != b._v;
  }
  friend std::istream &operator>>(std::istream &is, ModInt &a) {
    return is >> a._v;
  }
  friend std::ostream &operator<<(std::ostream &os, const ModInt &a) {
    return os << a._v;
  }

 private:
  ModInt inv_internal() const {
    long long b = 1, a = _v;
    while (a > 1) {
      long long q = M / a;
      a = M - a * q;
      b = -b * q % M;
    }
    assert(a == 1);  // if a = 0, _v and M are not coprime.
    if (b < 0) b += M;
    ModInt ret;
    ret._v = (unsigned)b;
    return ret;
  }

  unsigned _v;  // raw value
};
// constexpr unsigned MOD = int(1e9) + 7;
// constexpr unsigned MOD = 998244353;
// using Mint = ModInt<MOD>;
