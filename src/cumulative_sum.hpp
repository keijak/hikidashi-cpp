#include <bits/stdc++.h>

template <class T>
struct CumSum2d {
  template <class X>
  using Vec = std::vector<X>;

  int n, m;
  Vec<Vec<T>> cum;

  template <class U>
  explicit CumSum2d(const Vec<Vec<U>> &data)
      : n(data.size()), m(data.at(0).size()), cum(n + 1, Vec<T>(m + 1, T(0))) {
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) {
        cum[i + 1][j + 1] =
            cum[i][j + 1] + cum[i + 1][j] - cum[i][j] + data[i][j];
      }
    }
  }

  // (r0, c0) := top-left point
  // (r1, c1) := bottom-right point
  T sum(int r0, int c0, int r1, int c1) const {
    assert(r0 <= r1 and c0 <= c1);
    return cum[r1][c1] - cum[r1][c0] - cum[r0][c1] + cum[r0][c0];
  }
};
