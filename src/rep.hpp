#include <ranges>

auto rep(int n) { return std::views::iota(0, std::max(n, 0)); };
auto rep(int a0, int n) { return std::views::iota(a0, std::max(n, a0)); };
