#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

template <typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}

template <typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}

template <class T>
T ceil_div(T x, T y) {
  assert(y != 0);
  return x / y + (((x ^ y) >= 0) and (x % y));
}

template <class T>
T floor_div(T x, T y) {
  assert(y != 0);
  return x / y - (((x ^ y) < 0) and (x % y));
}

// Binary search.
// Returns the boundary argument which satisfies pred(x).
//
// Usage:
//   auto ok_bound = bisect(ok, ng, [&](i64 x) -> bool { return ...; });
template <class F>
i64 bisect(i64 true_x, i64 false_x, F pred) {
  static_assert(std::is_invocable_r_v<bool, F, i64>, "F must be: i64 -> bool");
  assert(std::max<i64>(true_x, false_x) <= std::numeric_limits<i64>::max() / 2);
  // To allow negative values, use floor_div() in the loop.
  assert(true_x >= -1 and false_x >= -1);
  using u64 = unsigned long long;

  while (std::abs(true_x - false_x) > 1) {
    i64 mid = ((u64)true_x + (u64)false_x) / 2;
    if (pred(mid)) {
      true_x = std::move(mid);
    } else {
      false_x = std::move(mid);
    }
  }
  return true_x;
}

template <class T>
using MinHeap = std::priority_queue<T, std::vector<T>, std::greater<T>>;

template <typename T>
void distinct(std::vector<T> &v) {
  std::sort(v.begin(), v.end());
  v.erase(std::unique(v.begin(), v.end()), v.end());
}

auto adjascent(int x, int y) -> std::vector<std::pair<int, int>> {
  static const int dx[4] = {1, 0, -1, 0};
  static const int dy[4] = {0, 1, 0, -1};
  std::vector<std::pair<int, int>> ret(4);
  for (int d = 0; d < 4; ++d) {
    ret[d] = {x + dx[d], y + dy[d]};
  }
  return ret;
}
