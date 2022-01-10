// https://kopricky.github.io/code/DataStructure_Advanced/bit_2d.html

template <typename T>
class BIT_2D_RangeAdd_RangeSum {
 private:
  const int n, m;
  vector<vector<T>> bitxy, bitx, bity, bitc;

  void add_(const int i, const int j, const T valxy, const T valx, const T valy,
            const T valc) {
    for (int i_ = i + 1; i_ < n; i_ += i_ & -i_)
      for (int j_ = j + 1; j_ < m; j_ += j_ & -j_)
        bitxy[i_][j_] += valxy, bitx[i_][j_] += valx, bity[i_][j_] += valy,
            bitc[i_][j_] += valc;
  }
  // [0, i] x [0, j]
  T sum_(const int i, const int j) const {
    T s = 0;
    for (int i_ = i + 1; i_ > 0; i_ -= i_ & -i_)
      for (int j_ = j + 1; j_ > 0; j_ -= j_ & -j_)
        s += bitxy[i_][j_] * i * j + bitx[i_][j_] * i + bity[i_][j_] * j +
             bitc[i_][j_];
    return s;
  }

 public:
  BIT_2D_RangeAdd_RangeSum(const int sz1, const int sz2)
      : n(sz1 + 1),
        m(sz2 + 1),
        bitxy(n, vector<T>(m, 0)),
        bitx(n, vector<T>(m, 0)),
        bity(n, vector<T>(m, 0)),
        bitc(n, vector<T>(m, 0)) {}

  // top_left×bottom_right (exclusive) の矩形領域に val を足す
  void add(int x_lo, int x_hi, int y_lo, int y_hi, T val) {
    add_(x_lo, y_lo, val, -val * (y_lo - 1), -val * (x_lo - 1),
         val * (x_lo - 1) * (y_lo - 1));
    add_(x_hi, y_lo, -val, val * (y_lo - 1), val * (x_hi - 1),
         -val * (x_hi - 1) * (y_lo - 1));
    add_(x_lo, y_hi, -val, val * (y_hi - 1), val * (x_lo - 1),
         -val * (x_lo - 1) * (y_hi - 1));
    add_(x_hi, y_hi, val, -val * (y_hi - 1), -val * (x_hi - 1),
         val * (x_hi - 1) * (y_hi - 1));
  }

  // [x_lo, x_hi) x [y_lo, y_hi) の矩形領域の和を求める
  T sum(int x_lo, int x_hi, int y_lo, int y_hi) const {
    return sum_(x_hi - 1, y_hi - 1) - sum_(x_lo - 1, y_hi - 1) -
           sum_(x_hi - 1, y_lo - 1) + sum_(x_lo - 1, y_lo - 1);
  }

  void print() const {
    for (int i = 0; i < n - 1; ++i) {
      for (int j = 0; j < m - 1; ++j) {
        cout << sum(i, j, i + 1, j + 1) << ' ';
      }
      cout << '\n';
    }
  }
};
