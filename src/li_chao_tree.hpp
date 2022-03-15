#include <algorithm>
#include <cassert>
#include <limits>
#include <type_traits>
#include <unordered_map>
#include <vector>

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

// y = ax + b
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

// Sparse (dynamic) Li-Chao Tree using gp_hash_table.

#include <ext/pb_ds/assoc_container.hpp>

template <class K, class V>
using gp_hash_table = __gnu_pbds::gp_hash_table<
    K, V, std::hash<K>, std::equal_to<K>,
    __gnu_pbds::direct_mask_range_hashing<K>, __gnu_pbds::linear_probe_fn<>,
    __gnu_pbds::hash_standard_resize_policy<
        __gnu_pbds::hash_exponential_size_policy<>,
        __gnu_pbds::hash_load_check_resize_trigger<true>, true>>;

template <class Op>  // Min or Max
struct SparseLiChaoTree {
  using Y = typename Op::T;  // y-coordinate
  using X = long long;       // x-coordinate
  static_assert(std::is_arithmetic<Y>::value, "must be number");
  static_assert(std::is_integral<X>::value, "must be integer");

  // Creates a Li-Chao Tree with x-coordinate bounds [x_low, x_high).
  // Dynamically creates nodes.
  SparseLiChaoTree(X x_low, X x_high) : x_low_(x_low), n_(x_high - x_low) {
    assert(x_low <= x_high);
    offset_ = 1;
    while (offset_ < n_) offset_ <<= 1;
    lines_.resize(1 << 20);
  }

  // Adds y = g(x).
  void add_line(Line<Y> g) { update(std::move(g), 0, n_); }

  // Adds y = g(x) in xl <= x < xr.
  void add_segment(Line<Y> g, X xl, X xr) {
    auto l = xl - x_low_;
    auto r = xr - x_low_;
    update(std::move(g), l, r);
  }

  // Returns the minimum/maximum f(x) at x.
  Y query(X x) const {
    assert(x < n_);
    Y y = Op::id();
    for (X i = x - x_low_ + offset_; i > 0; i >>= 1) {
      auto it = lines_.find(i);
      if (it != lines_.end()) {
        y = Op::op(y, it->second.eval(x));
      }
    }
    return y;
  }

 private:
  void update(Line<Y> g, X l, X r) {
    for (l += offset_, r += offset_; l < r; l >>= 1, r >>= 1) {
      if (l & 1) descend(g, l++);
      if (r & 1) descend(g, --r);
    }
  }

  void descend(Line<Y> g, X i) {
    X l = i, r = i + 1;
    while (l < offset_) {
      l <<= 1;
      r <<= 1;
    }
    while (l < r) {
      auto c = (l + r) >> 1;
      auto xl = l - offset_ + x_low_;
      auto xc = c - offset_ + x_low_;
      auto xr = r - 1 - offset_ + x_low_;
      auto fit = lines_.find(i);
      if (fit == lines_.end()) {
        lines_.insert({i, std::move(g)});
        return;
      }
      Line<Y> &f = fit->second;
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

  X x_low_, n_, offset_;
  gp_hash_table<X, Line<Y>> lines_;
};
