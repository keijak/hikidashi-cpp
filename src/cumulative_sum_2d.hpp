#include <bits/stdc++.h>

template <class T>
struct CumSum2d {
  template <class U>
  using Vec = std::vector<U>;

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

  // [x_lo, x_hi) x [y_lo, y_hi)
  T sum(int x_lo, int x_hi, int y_lo, int y_hi) const {
    assert(x_lo <= x_hi and y_lo <= y_hi);
    return cum[x_hi][y_hi] - cum[x_hi][y_lo] - cum[x_lo][y_hi] +
           cum[x_lo][y_lo];
  }
};
