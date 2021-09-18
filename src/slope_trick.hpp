// Slope Trick on piecewise linear convex curve.
// https://maspypy.com/slope-trick-1-%E8%A7%A3%E8%AA%AC%E7%B7%A8
// https://ei1333.github.io/library/structure/others/generalized-slope-trick.cpp
#include <bits/stdc++.h>

template <typename T = long long>
struct PWLConvexCurve {
  static constexpr T kInf = std::numeric_limits<T>::max() / 2;

  T fmin;
  std::priority_queue<T, std::vector<T>, std::less<>> lq;
  std::priority_queue<T, std::vector<T>, std::greater<>> rq;

  PWLConvexCurve() : fmin{0} {
    lq.push(-kInf);
    rq.push(kInf);
  }

  void add_constant(T c) { fmin += c; }

  // Add g(x) = max(x-a, 0)
  void add_x_minus_a(T a) {
    fmin += std::max<T>(lq.top() - a, 0);
    lq.push(a);
    rq.push(lq.top());
    lq.pop();
  }

  // Add g(x) = max(a-x, 0)
  void add_a_minus_x(T a) {
    fmin += std::max<T>(a - rq.top(), 0);
    rq.push(a);
    lq.push(rq.top());
    rq.pop();
  }

  // Add g(x) = abs(x - a)
  void add_abs(T a) {
    add_x_minus_a(a);
    add_a_minus_x(a);
  }

  // Flatten the left slope.
  void cum_max() {
    while (not lq.empty()) lq.pop();
  }

  // Flatten the right slope.
  void cum_min() {
    while (not rq.empty()) rq.pop();
  }
};
template <typename T>
std::ostream &operator<<(std::ostream &os, const PWLConvexCurve<T> &curve) {
  return os << "fmin=" << curve.fmin << " l0=" << curve.lq.top()
            << " r0=" << curve.rq.top();
}
