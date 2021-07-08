#include <cassert>
#include <cmath>
#include <type_traits>
using i64 = long long;

// Binary search.
// Returns the boundary argument which satisfies pred(x).
//
// Usage:
//   auto opt_x = bisect(ok, ng, [&](i64 x) -> bool { return ...; });
template <class F>
i64 bisect(i64 true_x, i64 false_x, F pred) {
  static_assert(std::is_invocable_r_v<bool, F, i64>, "F must be i64->bool");
  // To allow negative values, use floor_div.
  assert(true_x >= 0 and false_x >= 0);

  while (std::abs(true_x - false_x) > 1) {
    auto mid = (true_x + false_x) / 2;
    if (pred(mid)) {
      true_x = std::move(mid);
    } else {
      false_x = std::move(mid);
    }
  }
  return true_x;
}
