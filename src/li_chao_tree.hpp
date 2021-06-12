#include <algorithm>
#include <limits>
#include <type_traits>

// Li-Chao Tree
// - Finds min values.
// - Dynamically creates nodes.
template <typename T>
struct SparseLiChaoTree {
  static_assert(std::is_arithmetic_v<T>, "must be number");

  // Identity value of the Min monoid.
  static constexpr T min_id = std::numeric_limits<T>::max();

  struct Line {
    T a, b;

    Line(T a, T b) : a(a), b(b) {}

    inline T get(T x) const { return a * x + b; }
  };

  struct Node {
    Line x;
    Node *l, *r;

    Node(const Line &x) : x{x}, l{nullptr}, r{nullptr} {}
  };

  Node *root_;
  T x_low_, x_high_;  // Domain of the lines: x_low <= x <= x_high

  SparseLiChaoTree(T x_low, T x_high)
      : root_{nullptr}, x_low_{std::move(x_low)}, x_high_{std::move(x_high)} {}

  Node *add_line(Node *t, Line &x, const T &l, const T &r, const T &x_l,
                 const T &x_r) {
    if (!t) return new Node(x);

    T t_l = t->x.get(l), t_r = t->x.get(r);

    if (t_l <= x_l && t_r <= x_r) {
      return t;
    } else if (t_l >= x_l && t_r >= x_r) {
      t->x = x;
      return t;
    } else {
      T m = (l + r) / 2;
      if (m == r) --m;
      T t_m = t->x.get(m), x_m = x.get(m);
      if (t_m > x_m) {
        std::swap(t->x, x);
        if (x_l >= t_l) {
          t->l = add_line(t->l, x, l, m, t_l, t_m);
        } else {
          t->r = add_line(t->r, x, m + 1, r, t_m + x.a, t_r);
        }
      } else {
        if (t_l >= x_l) {
          t->l = add_line(t->l, x, l, m, x_l, x_m);
        } else {
          t->r = add_line(t->r, x, m + 1, r, x_m + x.a, x_r);
        }
      }
      return t;
    }
  }

  void add_line(const T &a, const T &b) {
    Line x(a, b);
    root_ = add_line(root_, x, x_low_, x_high_, x.get(x_low_), x.get(x_high_));
  }

  Node *add_segment(Node *t, Line &x, const T &a, const T &b, const T &l,
                    const T &r, const T &x_l, const T &x_r) {
    if (r < a || b < l) return t;
    if (a <= l && r <= b) {
      Line y{x};
      return add_line(t, y, l, r, x_l, x_r);
    }
    if (t) {
      T t_l = t->x.get(l), t_r = t->x.get(r);
      if (t_l <= x_l && t_r <= x_r) return t;
    } else {
      t = new Node(Line(0, min_id));
    }
    T m = (l + r) / 2;
    if (m == r) --m;
    T x_m = x.get(m);
    t->l = add_segment(t->l, x, a, b, l, m, x_l, x_m);
    t->r = add_segment(t->r, x, a, b, m + 1, r, x_m + x.a, x_r);
    return t;
  }

  void add_segment(const T &l, const T &r, const T &a, const T &b) {
    Line x(a, b);
    root_ = add_segment(root_, x, l, r - 1, x_low_, x_high_, x.get(x_low_),
                        x.get(x_high_));
  }

  T query(const Node *t, const T &l, const T &r, const T &x) const {
    if (!t) return min_id;
    if (l == r) return t->x.get(x);
    T m = (l + r) / 2;
    if (m == r) --m;
    if (x <= m) {
      return std::min(t->x.get(x), query(t->l, l, m, x));
    } else {
      return std::min(t->x.get(x), query(t->r, m + 1, r, x));
    }
  }

  T query(const T &x) const { return query(root_, x_low_, x_high_, x); }
};
