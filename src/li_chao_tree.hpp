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

    inline T eval(T x) const { return a * x + b; }
  };

  struct Node {
    Line f;
    Node *l, *r;

    Node(Line f) : f{std::move(f)}, l{nullptr}, r{nullptr} {}
  };

  Node *root_;
  T x_low_, x_high_;  // Domain of the lines: x_low <= x <= x_high

  SparseLiChaoTree(T x_low, T x_high)
      : root_{nullptr}, x_low_{std::move(x_low)}, x_high_{std::move(x_high)} {}

  void add_line(const T &a, const T &b) {
    Line f(a, b);
    root_ =
        add_line(root_, f, x_low_, x_high_, f.eval(x_low_), f.eval(x_high_));
  }

  void add_segment(const T &l, const T &r, const T &a, const T &b) {
    Line f(a, b);
    root_ = add_segment(root_, f, l, r - 1, x_low_, x_high_, f.eval(x_low_),
                        f.eval(x_high_));
  }

  // Returns the minimum value at x.
  T query(const T &x) const { return query(root_, x_low_, x_high_, x); }

 private:
  Node *add_line(Node *t, Line &f, const T &l, const T &r, const T &f_l,
                 const T &f_r) {
    if (!t) return new Node(f);

    T t_l = t->f.eval(l), t_r = t->f.eval(r);

    if (t_l <= f_l && t_r <= f_r) {
      return t;
    } else if (t_l >= f_l && t_r >= f_r) {
      t->f = f;
      return t;
    } else {
      T m = (l + r) / 2;
      if (m == r) --m;
      T t_m = t->f.eval(m), f_m = f.eval(m);
      if (t_m > f_m) {
        std::swap(t->f, f);
        if (f_l >= t_l) {
          t->l = add_line(t->l, f, l, m, t_l, t_m);
        } else {
          t->r = add_line(t->r, f, m + 1, r, t_m + f.a, t_r);
        }
      } else {
        if (t_l >= f_l) {
          t->l = add_line(t->l, f, l, m, f_l, f_m);
        } else {
          t->r = add_line(t->r, f, m + 1, r, f_m + f.a, f_r);
        }
      }
      return t;
    }
  }

  Node *add_segment(Node *t, Line &f, const T &a, const T &b, const T &l,
                    const T &r, const T &f_l, const T &f_r) {
    if (r < a || b < l) return t;
    if (a <= l && r <= b) {
      Line f2{f};
      return add_line(t, f2, l, r, f_l, f_r);
    }
    if (t) {
      T t_l = t->f.eval(l), t_r = t->f.eval(r);
      if (t_l <= f_l && t_r <= f_r) return t;
    } else {
      t = new Node(Line(0, min_id));
    }
    T m = (l + r) / 2;
    if (m == r) --m;
    T f_m = f.eval(m);
    t->l = add_segment(t->l, f, a, b, l, m, f_l, f_m);
    t->r = add_segment(t->r, f, a, b, m + 1, r, f_m + f.a, f_r);
    return t;
  }

  // l <= x <= r
  T query(const Node *t, const T &l, const T &r, const T &x) const {
    assert(l <= x and x <= r);
    if (!t) return min_id;
    if (l == r) return t->f.eval(x);
    T m = (l + r) / 2;
    if (m == r) --m;
    if (x <= m) {
      return std::min(t->f.eval(x), query(t->l, l, m, x));
    } else {
      return std::min(t->f.eval(x), query(t->r, m + 1, r, x));
    }
  }
};
