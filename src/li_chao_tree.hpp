#include <algorithm>
#include <limits>
#include <type_traits>
#include <vector>

// Line-like function.
//
// Not necessarily a line, but must hold the transcending property:
// Two curves cross at most once in the specified range.
template <class T>
struct Line {
  T a;
  T b;

  Line(T a, T b) : a(std::move(a)), b(std::move(b)) {}

  T eval(T x) const { return a * x + b; }

  static Line constant(T value) { return Line(0, value); }
};

template <class Op>  // Min or Max
struct LiChaoTree {
  using T = typename Op::T;
  static_assert(std::is_arithmetic<T>::value, "must be number");

  // Creates a Li-Chao Tree with query candidates it can answer.
  explicit LiChaoTree(const std::vector<T> &xs) : n_(0), xs_(xs) {
    std::sort(xs_.begin(), xs_.end());
    xs_.erase(std::unique(xs_.begin(), xs_.end()), xs_.end());
    n_ = (int)xs_.size();
    lines_.assign(n_ << 1, Line<T>::constant(Op::id()));
  }

  // Adds y = g(x).
  void add_line(Line<T> g) { update(std::move(g), 0, n_); }

  // Adds y = g(x) in xl <= x < xr.
  void add_segment(Line<T> g, T xl, T xr) {
    int l = std::lower_bound(xs_.begin(), xs_.end(), xl) - xs_.begin();
    int r = std::lower_bound(xs_.begin(), xs_.end(), xr) - xs_.begin();
    update(std::move(g), l, r);
  }

  // Returns the minimum/maximum f(x) at x.
  T query(T x) const {
    int i = std::lower_bound(xs_.begin(), xs_.end(), x) - xs_.begin();
    assert(i != n_ && x == xs_[i]);
    T y = Op::id();
    for (i += n_; i > 0; i >>= 1) y = Op::op(y, lines_[i].eval(x));
    return y;
  }

 private:
  void update(Line<T> g, int l, int r) {
    for (l += n_, r += n_; l < r; l >>= 1, r >>= 1) {
      if (l & 1) descend(g, l++);
      if (r & 1) descend(g, --r);
    }
  }

  void descend(Line<T> g, int i) {
    int l = i, r = i + 1;
    while (l < n_) {
      l <<= 1;
      r <<= 1;
    }
    while (l < r) {
      int c = (l + r) >> 1;
      T xl = xs_[l - n_];
      T xc = xs_[c - n_];
      T xr = xs_[r - 1 - n_];
      Line<T> &f = lines_[i];
      if (not Op::less(g.eval(xl), f.eval(xl)) and
          not Op::less(g.eval(xr), f.eval(xr))) {
        return;
      }
      if (not Op::less(f.eval(xl), g.eval(xl)) and
          not Op::less(f.eval(xr), g.eval(xr))) {
        f = std::move(g);
        return;
      }
      if (Op::less(g.eval(xc), f.eval(xc))) {
        std::swap(f, g);
      }
      if (Op::less(g.eval(xl), f.eval(xl))) {
        i = (i << 1) | 0;
        r = c;
      } else {
        i = (i << 1) | 1;
        l = c;
      }
    }
  }

  int n_;
  std::vector<T> xs_;
  std::vector<Line<T>> lines_;
};

struct Min {
  using T = long long;
  static bool less(const T &x, const T &y) { return x < y; }
  static T op(const T &x, const T &y) { return std::min(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::max(); }
};

struct Max {
  using T = long long;
  static bool less(const T &x, const T &y) { return x > y; }
  static T op(const T &x, const T &y) { return std::max(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::lowest(); }
};

using LCT = LiChaoTree<Min>;
