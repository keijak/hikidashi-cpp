// Y combinator.
// ref: https://stackoverflow.com/a/40873657
//
// auto f = Rec([&](auto& f, int n) -> int {
//     if (n <= 1) return n;
//     return f(n-1) + f(n-2);
// });
// cout << f(5) << endl;
//
#include <functional>

template <class F>
struct Rec {
  F f_;
  explicit Rec(F f) : f_(std::move(f)) {}
  template <class... Args>
  decltype(auto) operator()(Args&&... args) {
    return f_(*this, std::forward<Args>(args)...);
  }
};

// deduction guide (>= C++17)
template <class F>
Rec(F) -> Rec<F>;

// helper function (C++14)
template <class F>
Rec<std::decay_t<F>> fix(F&& f) {
  return {std::forward<F>(f)};
}
