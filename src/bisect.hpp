#include <cmath>
#include <type_traits>

// Binary search.
// Returns the boundary argument which satisfies pred(x).
//
// Usage:
//   auto opt_x = bisect(ok, ng, [&](int x) -> bool { return ...; });
template <class I, class F>
I bisect(I true_x, I false_x, F pred) {
  static_assert(std::is_integral_v<I>, "I must be integer");
  static_assert(std::is_invocable_r_v<bool, F, I>, "F must be invocable");

  while (std::abs(true_x - false_x) > I(1)) {
    I mid = (true_x + false_x) / I(2);
    if (pred(mid)) {
      true_x = std::move(mid);
    } else {
      false_x = std::move(mid);
    }
  }
  return true_x;
}
