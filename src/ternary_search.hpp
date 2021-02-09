#include <bits/stdc++.h>
using i64 = long long;

// Ternary search over an integer interval.
// Finds x in [low, high) s.t. func(x) is minimal.
i64 find_min_ternary_search(i64 low, i64 high, function<i64(i64)> func) {
  --low;  // Make it an open interval: (low, high).
  i64 l = low, r = high;
  while (r - l > 2) {
    i64 ll = (l + l + r) / 3;
    i64 rr = (l + r + r) / 3;
    if (func(ll) < func(rr)) {
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
i64 find_min_golden_section_search(i64 low, i64 high, function<i64(i64)> f) {
  --low;             // Make it an open interval: (low, high).
  i64 l = 1, r = 1;  // Left and right offsets from `low`.
  while (l + r < high - low) {
    l += r;
    std::swap(l, r);
  }
  auto lval = f(low + l), rval = f(low + r);
  while (l < r) {
    r -= l;
    std::swap(l, r);
    if (lval < rval) {
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
