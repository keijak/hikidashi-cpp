using i64 = long long;

// Ternary search over integer interval.
// Finds x in [low, high) s.t. func(x) is minimal.
i64 find_min_ternary_search(i64 low, i64 high, function<i64(i64)> func) {
  i64 l = low - 1, r = high;
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
