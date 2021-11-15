#include <bits/stdc++.h>
using namespace std;
using Real = long double;

const Real PI = std::acos(Real(-1));

template <typename T = Real>
struct FastFourierTransform {
  static_assert(std::is_floating_point_v<T>);

  struct C {
    T x, y;

    C() : x(0), y(0) {}

    C(T x, T y) : x(x), y(y) {}

    inline C operator+(const C &c) const { return C(x + c.x, y + c.y); }

    inline C operator-(const C &c) const { return C(x - c.x, y - c.y); }

    inline C operator*(const C &c) const {
      return C(x * c.x - y * c.y, x * c.y + y * c.x);
    }

    inline C conj() const { return C(x, -y); }
  };

  int base;
  vector<C> rts;
  vector<int> rev;

  FastFourierTransform() : base(1), rts{{0, 0}, {1, 0}}, rev{0, 1} {}

  void ensure_base(int nbase) {
    if (nbase <= base) return;
    rev.resize(1 << nbase);
    rts.resize(1 << nbase);
    for (int i = 0; i < (1 << nbase); i++) {
      rev[i] = (rev[i >> 1] >> 1) + ((i & 1) << (nbase - 1));
    }
    while (base < nbase) {
      T angle = PI * 2.0 / (1 << (base + 1));
      for (int i = 1 << (base - 1); i < (1 << base); i++) {
        rts[i << 1] = rts[i];
        T angle_i = angle * (2 * i + 1 - (1 << base));
        rts[(i << 1) + 1] = C(cos(angle_i), sin(angle_i));
      }
      ++base;
    }
  }

  void fft(vector<C> &a, int n) {
    assert((n & (n - 1)) == 0);
    int zeros = __builtin_ctz(n);
    ensure_base(zeros);
    int shift = base - zeros;
    for (int i = 0; i < n; i++) {
      if (i < (rev[i] >> shift)) {
        swap(a[i], a[rev[i] >> shift]);
      }
    }
    for (int k = 1; k < n; k <<= 1) {
      for (int i = 0; i < n; i += 2 * k) {
        for (int j = 0; j < k; j++) {
          C z = a[i + j + k] * rts[j + k];
          a[i + j + k] = a[i + j] - z;
          a[i + j] = a[i + j] + z;
        }
      }
    }
  }

  vector<T> multiply(const vector<T> &a, const vector<T> &b) {
    int need = (int)a.size() + (int)b.size() - 1;
    int nbase = 1;
    while ((1 << nbase) < need) nbase++;
    ensure_base(nbase);
    int sz = 1 << nbase;
    vector<C> fa(sz);
    for (int i = 0; i < sz; i++) {
      T x = (i < (int)a.size() ? a[i] : 0);
      T y = (i < (int)b.size() ? b[i] : 0);
      fa[i] = C(x, y);
    }
    fft(fa, sz);
    C r(0, -0.25 / (sz >> 1)), s(0, 1), t(0.5, 0);
    for (int i = 0; i <= (sz >> 1); i++) {
      int j = (sz - i) & (sz - 1);
      C z = (fa[j] * fa[j] - (fa[i] * fa[i]).conj()) * r;
      fa[j] = (fa[i] * fa[i] - (fa[j] * fa[j]).conj()) * r;
      fa[i] = z;
    }
    for (int i = 0; i < (sz >> 1); i++) {
      C A0 = (fa[i] + fa[i + (sz >> 1)]) * t;
      C A1 = (fa[i] - fa[i + (sz >> 1)]) * t * rts[(sz >> 1) + i];
      fa[i] = A0 + A1 * s;
    }
    fft(fa, sz >> 1);
    vector<T> ret(need);
    for (int i = 0; i < need; i++) {
      ret[i] = (i & 1) ? fa[i >> 1].y : fa[i >> 1].x;
    }
    return ret;
  }
};
