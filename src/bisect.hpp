#include <bits/stdc++.h>

// Safe (a+b)/2 on integers
template <class T>
T midpoint(T a, T b) {
  using U = std::make_unsigned_t<T>;
  int sign = 1;
  U m = a, M = b;
  if (a > b) sign = -1, m = b, M = a;
  return a + sign * T(U(M - m) >> 1);
}

// Binary search over integers
template <class T, class F>
auto bisect(T truthy, T falsy, F pred) -> T {
  static_assert(std::is_integral_v<T>);
  static_assert(std::is_invocable_r_v<bool, F, T>);
  while (std::max(truthy, falsy) - std::min(truthy, falsy) > 1) {
    auto mid = midpoint(truthy, falsy);
    auto ok = pred(mid);
    (ok ? truthy : falsy) = std::move(mid);
  }
  return truthy;
}

// Binary search over floating numbers
template <class Float, class F>
Float bisect_float(Float truthy, Float falsy, F pred) {
  static_assert(std::is_floating_point_v<Float>);
  static_assert(std::is_invocable_r_v<bool, F, Float>);
  for (int iter = 0; iter < 80; ++iter) {
    auto mid = (truthy + falsy) * Float(0.5);
    auto ok = pred(mid);
    (ok ? truthy : falsy) = std::move(mid);
  }
  return truthy;
}
