#include <bits/stdc++.h>

struct MinOp {
  using T = long long;
  static T op(const T &x, const T &y) { return std::min(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::max(); }
};

struct MaxOp {
  using T = long long;
  static T op(const T &x, const T &y) { return std::max(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::lowest(); }
};

struct SumOp {
  using T = long long;
  static T op(const T &x, const T &y) {
    return x + y;
    // alt: saturating_add(x, y)
  }
  static constexpr T id() { return 0; }
  static T invert(const T &x) { return -x; }
};

struct ProdOp {
  using T = long long;
  static T op(const T &x, const T &y) {
    return x * y;
    // alt: saturating_mul(x, y)
  }
  static constexpr T id() { return 1; }
};

struct XorOp {
  using T = unsigned long long;
  static T op(const T &x, const T &y) { return x ^ y; }
  static constexpr T id() { return 0; }
  static T invert(const T &x) { return x; }
};

struct SumCountOp {
  struct T {
    long long sum;
    int count;
  };
  static T op(const T &x, const T &y) {
    return {x.sum + y.sum, x.count + y.count};
  }
  static constexpr T id() { return {0, 0}; }
};

// Keeps the value with the latest time.
struct AssignOp {
  // Value with a timestamp.
  struct T {
    unsigned time;
    long long value;
  };
  static T op(const T &x, const T &y) { return (x.time > y.time) ? x : y; }
  static T id() { return {0, 0}; }
};

struct GCDOp {
  using T = long long;
  static T op(const T &x, const T &y) {
    if (x == id()) return y;
    if (y == id()) return x;
    return std::gcd(x, y);
  }
  static constexpr T id() { return 0; }
};

// 2 smallest items in an interval.
struct Min2Op {
  using value_type = long long;
  using T = std::array<value_type, 2>;  // [0] = smallest, [1] = second smallest

  static T op(const T &x, const T &y) {
    std::array<value_type, 4> a = {x[0], x[1], y[0], y[1]};
    std::sort(std::begin(a), std::end(a));
    return {a[0], a[1]};
  }
  static constexpr T id() {
    return {std::numeric_limits<value_type>::max(),
            std::numeric_limits<value_type>::max()};
  }
};

// 2 largest items in an interval.
struct Max2Op {
  using value_type = long long;
  using T = std::array<value_type, 2>;  // [0] = largest, [1] = second largest

  static T op(const T &x, const T &y) {
    std::array<value_type, 4> a = {x[0], x[1], y[0], y[1]};
    std::sort(std::rbegin(a), std::rend(a));
    return {a[0], a[1]};
  }
  static constexpr T id() {
    return {std::numeric_limits<value_type>::lowest(),
            std::numeric_limits<value_type>::lowest()};
  }
};

struct AddMinOp {
  using T = long long;
  using F = long long;

  // Fold: Min
  static T op(const T &x, const T &y) { return std::min(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::max(); }

  // Update: Add
  static T f_apply(const F &f, const T &x) { return f + x; }
  static F f_compose(const F &f, const F &g) { return f + g; }
  static constexpr F f_id() { return 0; }
};

struct AddMaxOp {
  using T = long long;
  using F = long long;

  // Fold: Max
  static T op(const T &x, const T &y) { return std::max(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::lowest(); }

  // Update: Add
  static T f_apply(const F &f, const T &x) { return f + x; }
  static F f_compose(const F &f, const F &g) { return f + g; }
  static constexpr F f_id() { return 0; }
};

struct AddSumOp {
  struct T {
    long long sum;
    int width;  // NOTE: Must be initialized with width=1!
  };
  using F = long long;

  // Fold: Sum
  static T op(const T &x, const T &y) {
    return {x.sum + y.sum, x.width + y.width};
  }
  static constexpr T id() { return {0, 0}; }

  // Update: Add
  static T f_apply(const F &f, const T &x) {
    return {x.sum + f * x.width, x.width};
  }
  static F f_compose(const F &f, const F &g) { return f + g; }
  static constexpr F f_id() { return 0; }
};

// Range Flip Range Sum over 0-1 sequence.
struct XorSumOp {
  struct T {
    int ones;
    int width;  // NOTE: Must be initialized with width=1!
  };
  using F = int;

  // Fold: Sum
  static T op(const T &x, const T &y) {
    return {x.ones + y.ones, x.width + y.width};
  }
  static T id() { return {0, 0}; }

  // Update: 0-1 flip by f=1.
  static T f_apply(const F &f, const T &x) {
    if (f == 0) return x;
    return {x.width - x.ones, x.width};
  }
  static F f_compose(const F &f, const F &g) { return f ^ g; }
  static constexpr F f_id() { return 0; }
};

struct MulSumOp {
  using T = long long;
  using F = long long;

  // Fold: Sum
  static T op(const T &x, const T &y) { return x + y; }
  static T id() { return 0; }

  // Update: Multiply
  static T f_apply(const F &f, const T &x) { return x * f; }
  static F f_compose(const F &f, const F &g) { return f * g; }
  static F f_id() { return 1; }
};

struct AssignMinOp {
  using T = long long;
  using F = std::optional<long long>;

  // Fold: Min
  static T op(const T &x, const T &y) { return std::min(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::max(); }

  // Update: Assign
  static T f_apply(const F &f, const T &x) { return f ? *f : x; }
  static F f_compose(const F &f, const F &g) { return f ? f : g; }
  static constexpr F f_id() { return std::nullopt; }
};

struct AssignMaxOp {
  using T = long long;
  using F = std::optional<long long>;

  // Fold: Max
  static T op(const T &x, const T &y) { return std::max(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::lowest(); }

  // Update: Assign
  static T f_apply(const F &f, const T &x) { return f ? *f : x; }
  static F f_compose(const F &f, const F &g) { return f ? f : g; }
  static constexpr F f_id() { return std::nullopt; }
};

struct AssignSumOp {
  struct T {
    long long sum;
    int width;  // NOTE: Must be initialized with width=1!
  };
  using F = std::optional<long long>;

  // Fold: Sum
  static T op(const T &x, const T &y) {
    return {x.sum + y.sum, x.width + y.width};
  }
  static constexpr T id() { return {0, 0}; }

  // Update: Assign
  static T f_apply(const F &f, const T &x) {
    return f ? T{(*f) * x.width, x.width} : x;
  }
  static F f_compose(const F &f, const F &g) { return f ? f : g; }
  static constexpr F f_id() { return std::nullopt; }
};

struct ChminMinOp {
  using T = long long;
  using F = long long;

  // Fold: Min
  static T op(const T &x, const T &y) { return std::min(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::max(); }

  // Update: Change Min
  static T f_apply(const F &f, const T &x) { return std::min(f, x); }
  static F f_compose(const F &f, const F &g) { return std::min(f, g); }
  static constexpr F f_id() { return std::numeric_limits<T>::max(); }
};

struct ChmaxMaxOp {
  using T = long long;
  using F = long long;

  // Fold: Max
  static T op(const T &x, const T &y) { return std::max(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::lowest(); }

  // Update: Change Max
  static T f_apply(const F &f, const T &x) { return std::max(f, x); }
  static F f_compose(const F &f, const F &g) { return std::max(f, g); }
  static constexpr F f_id() { return std::numeric_limits<T>::lowest(); }
};

// Arighmetic Progression: (Assign, Min+Max+Sum)
// ref. https://null-mn.hatenablog.com/entry/2021/08/22/064325
struct ArithmeticProgressionAssignMinMaxSumOp {
  using Int = long long;
  struct Line {  // a*x + b
    Int a;
    Int b;
  };
  static constexpr Int kBig = std::numeric_limits<Int>::max() / 2;

  struct T {
    Int min;
    Int max;
    Int sum;
    // closed interval [l, r]
    int l;
    int r;
  };
  using F = std::optional<Line>;

  // Fold: Max, Min, Sum
  static T op(const T &x, const T &y) {
    return T{
        std::min(x.min, y.min), std::max(x.max, y.max), x.sum + y.sum, x.l, y.r,
    };
  }

  static T id() { return {kBig, -kBig, 0LL, -1, -1}; }

  // Update: Assign a*x + b
  static T f_apply(const F &f, const T &x) {
    if (not f) return x;
    auto [a, b] = *f;
    Int width = x.r - x.l;
    Int lval = a * x.l + b;
    Int rval = a * x.r + b;
    return T{std::min(lval, rval), std::max(lval, rval),
             (width + 1) * (lval + rval) / 2, x.l, x.r};
  }
  static F f_compose(const F &f, const F &g) { return f ? f : g; }
  static F f_id() { return std::nullopt; }
};
std::ostream &operator<<(std::ostream &os,
                         const ArithmeticProgressionAssignMinMaxSumOp::T &a) {
  return os << "{min:" << a.min << ", max:" << a.max << ", sum:" << a.sum
            << ", l:" << a.l << ", r:" << a.r << "}";
}

// Arighmetic Progression: (Add, Sum)
// ref. https://null-mn.hatenablog.com/entry/2021/08/22/064325
struct ArithmeticProgressionAddSumOp {
  using Int = long long;
  struct Line {  // a*x + b
    Int a;
    Int b;
  };

  struct T {
    Int sum;
    // closed interval [l, r]
    int l;
    int r;
  };
  using F = Line;

  // Fold: Sum
  static T op(const T &x, const T &y) { return T{x.sum + y.sum, x.l, y.r}; }

  static T id() { return {0LL, -1, -1}; }

  // Update: Add a*x + b
  static T f_apply(const F &f, const T &s) {
    // static const Mint kHalf = Mint(1) / 2;
    auto sum = s.sum + (f.a * (s.l + s.r) + f.b * 2) * (s.r - s.l + 1) / 2;
    return {sum, s.l, s.r};
  }
  static F f_compose(const F &f, const F &g) {
    return Line{f.a + g.a, f.b + g.b};
  }
  static F f_id() { return Line{0, 0}; }
};
std::ostream &operator<<(std::ostream &os,
                         const ArithmeticProgressionAddSumOp::T &a) {
  return os << "{sum:" << a.sum << ", l:" << a.l << ", r:" << a.r << "}";
}
