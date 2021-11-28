// https://kopricky.github.io/code/DataStructure_Advanced/bit_2d.html

template <typename T>
class BIT_2D_RangeAdd_RangeSum {
 private:
  const int n, m;
  vector<vector<T>> bitxy, bitx, bity, bitc;

  void add(const int i, const int j, const T valxy, const T valx, const T valy,
           const T valc) {
    for (int i_ = i + 1; i_ < n; i_ += i_ & -i_)
      for (int j_ = j + 1; j_ < m; j_ += j_ & -j_)
        bitxy[i_][j_] += valxy, bitx[i_][j_] += valx, bity[i_][j_] += valy,
            bitc[i_][j_] += valc;
  }
  // [0, i] x [0, j]
  T sum(const int i, const int j) {
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
  void add(const int tlx, const int tly, const int brx, const int bry,
           const T val) {
    add(tlx, tly, val, -val * (tly - 1), -val * (tlx - 1),
        val * (tlx - 1) * (tly - 1));
    add(brx, tly, -val, val * (tly - 1), val * (brx - 1),
        -val * (brx - 1) * (tly - 1));
    add(tlx, bry, -val, val * (bry - 1), val * (tlx - 1),
        -val * (tlx - 1) * (bry - 1));
    add(brx, bry, val, -val * (bry - 1), -val * (brx - 1),
        val * (brx - 1) * (bry - 1));
  }

  // top_left×bottom_right (exclusive) の矩形領域の和を求める
  T sum(const int tlx, const int tly, const int brx, const int bry) const {
    return sum(brx - 1, bry - 1) - sum(tlx - 1, bry - 1) -
           sum(brx - 1, tly - 1) + sum(tlx - 1, tly - 1);
  }

  void print() const {
    for (int i = 0; i < n - 1; ++i) {
      for (int j = 0; j < m - 1; ++j) {
        cout << sum(i, j, i + 1, j + 1) << " ";
      }
      cout << endl;
    }
  }
};
