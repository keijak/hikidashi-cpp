#include <bits/stdc++.h>
using Int = long long;
using Float = double;

// Ternary search over a real-valued open interval.
// Finds x in (low, high) s.t. f(x) is minimal.
template <class F, class T = std::invoke_result_t<F, Float>>
Float find_min_ternary_search(Float low, Float high, F f) {
  static_assert(std::is_invocable_v<F, Float>);
  std::less<T> compare;  // Change it to greater<T> if you want max.
  Float l = low, r = high;
  for (int iter = 0; iter < 100; ++iter) {
    auto ll = (l + l + r) / 3.0;
    auto rr = (l + r + r) / 3.0;
    if (compare(f(ll), f(rr))) {
      r = rr;
    } else {
      l = ll;
    }
  }
  return (l + r) / 2.0;
}

// Ternary search over an integer interval.
// Finds x in [low, high) s.t. f(x) is minimal.
//
// Note: If the input range size is 1 (e.g. [1, 2)), this function returns the
// result without evaluating `f(x)` at all. To get the minimal value instead of
// the argmin, memoise `f` and call `f(result)`.
template <class F, class T = std::invoke_result_t<F, Int>>
Int find_min_ternary_search(Int low, Int high, F f) {
  static_assert(std::is_invocable_v<F, Int>);
  std::less<T> compare;  // Change it to greater<T> if you want max.

  --low;  // Make it an open interval: (low, high).
  Int l = low, r = high;
  while (r - l > 2) {
    Int ll = (l + l + r) / 3;
    Int rr = (l + r + r) / 3;
    if (compare(f(ll), f(rr))) {
      r = rr;
    } else {
      l = ll;
    }
  }
  return (l + r) / 2;
}

// Golden Section Search over an integer interval.
// Finds x in [low, high) s.t. func(x) is minimal.
// Unlike ternary search, `f(x)` is not required to be convex.
template <class F, class T = std::invoke_result_t<F, Int>>
Int find_min_golden_section_search(Int low, Int high, F f) {
  static_assert(std::is_invocable_v<F, Int>);
  std::less<T> compare;  // Change it to greater<T> if you want max.

  --low;             // Make it an open interval: (low, high).
  Int l = 1, r = 1;  // Left and right offsets from `low`.
  while (l + r < high - low) {
    l += r;
    std::swap(l, r);
  }
  auto lval = f(low + l), rval = f(low + r);
  while (l < r) {
    r -= l;
    std::swap(l, r);
    if (compare(lval, rval)) {
      rval = std::move(lval);
      lval = f(low + l);
    } else {
      low += r;
      if (low + r < high) {
        lval = std::move(rval);
        rval = f(low + r);
      } else {
        r -= l;
        std::swap(l, r);
        lval = f(low + l);
      }
    }
  }
  return low + 1;
}
