// Slope Trick on piecewise linear convex curve.
// https://maspypy.com/slope-trick-1-%E8%A7%A3%E8%AA%AC%E7%B7%A8
// https://ei1333.github.io/library/structure/others/generalized-slope-trick.cpp
#include <bits/stdc++.h>

template <typename T = long long>
struct PWLConvexCurve {
  static constexpr T kInf = std::numeric_limits<T>::max() / 2;

 private:
  std::priority_queue<T, std::vector<T>, std::less<>> lq_;
  std::priority_queue<T, std::vector<T>, std::greater<>> rq_;
  T min_;
  T l_offset_;
  T r_offset_;

 public:
  PWLConvexCurve() : min_{0}, l_offset_{0}, r_offset_{0} {}

  T min() const { return min_; }

  // Adds g(x) = c
  void add_constant(T c) { min_ += c; }

  // Adds g(x) = max(x-a, 0)
  void add_x_minus_a(T a) {
    min_ += std::max<T>(top_l() - a, 0);
    push_l(a);
    push_r(top_l());
    lq_.pop();
  }

  // Adds g(x) = max(a-x, 0)
  void add_a_minus_x(T a) {
    min_ += std::max<T>(a - top_r(), 0);
    push_r(a);
    push_l(top_r());
    rq_.pop();
  }

  // Adds g(x) = abs(x - a)
  void add_abs(T a) {
    add_x_minus_a(a);
    add_a_minus_x(a);
  }

  // Flattens the left slope.
  void cum_max() {
    while (not lq_.empty()) lq_.pop();
  }

  // Flattens the right slope.
  void cum_min() {
    while (not rq_.empty()) rq_.pop();
  }

  // Shifts the left points toward right by a.
  // Shifts the right points toward right by b.
  // Can widen the flat interval.
  // \_/ => \___/
  void shift(T a, T b) {
    assert(a <= b);
    l_offset_ += a;
    r_offset_ += b;
  }

  // Shifts all points toward right by a.
  void shift(T a) { shift(a, a); }

  friend std::ostream &operator<<(std::ostream &os,
                                  const PWLConvexCurve &curve) {
    return os << "min=" << curve.min() << " l0=" << curve.top_l()
              << " r0=" << curve.top_r();
  }

 private:
  void push_l(T a) { lq_.push(a - l_offset_); }
  void push_r(T a) { rq_.push(a - r_offset_); }
  T top_l() const {
    if (lq_.empty()) return -kInf;
    return lq_.top() + l_offset_;
  }
  T top_r() const {
    if (rq_.empty()) return kInf;
    return rq_.top() + r_offset_;
  }
};
