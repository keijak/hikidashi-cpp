// Merge Segment Tree
//
// 二次元座標上の点集合を扱う
// - 矩形領域内にある点の数を返す: O(N(logN)^2)
// - 矩形領域内にある点を列挙する: O(N(logN)^2 + |output|)
// 座標の第一要素(X)はセグメントツリーのインデックスとする。必要に応じて座標圧縮すること。
// 座標の第二要素(Y)は座標圧縮する必要はない。
// 点に座標以外の情報を持たせる場合は T = pair(Y, index) として collect()
// してから 元データに index でアクセスするとよい。
//
// 参考
// https://kopricky.github.io/code/SegmentTrees/orthogonal_range_report.html

template <typename T>
struct MergeSegmentTree {
  int n_;
  std::vector<std::vector<T>> data_;  // Rows sorted by T.

  explicit MergeSegmentTree(std::vector<std::vector<T>> data)
      : n_(data.size()), data_(2 * n_) {
    for (int i = n_, n2 = n_ * 2; i < n2; ++i) {
      data_[i] = std::move(data[i - n_]);
      std::sort(data_[i].begin(), data_[i].end());
    }
    for (int i = n_ - 1; i >= 1; --i)
      std::merge(data_[i << 1 | 0].begin(), data_[i << 1 | 0].end(),
                 data_[i << 1 | 1].begin(), data_[i << 1 | 1].end(),
                 std::back_inserter(data_[i]));
  }

  // Returns the nubmer of points in the range [x_lo, x_hi) x [y_lo, y_hi).
  // O(N(logN)^2)
  int count(int x_lo, int x_hi, T y_lo, T y_hi) const {
    int res = 0;
    for (x_lo += n_, x_hi += n_; x_lo < x_hi; x_lo >>= 1, x_hi >>= 1) {
      if (x_lo & 1) res += count_(x_lo++, y_lo, y_hi);
      if (x_hi & 1) res += count_(--x_hi, y_lo, y_hi);
    }
    return res;
  }

  // Returns all points in the range [x_lo, x_hi) x [y_lo, y_hi).
  // O(N(logN)^2 + |output|)
  std::vector<T> collect(int x_lo, int x_hi, T y_lo, T y_hi) const {
    std::vector<T> res;
    for (x_lo += n_, x_hi += n_; x_lo < x_hi; x_lo >>= 1, x_hi >>= 1) {
      if (x_lo & 1) collect_(x_lo++, y_lo, y_hi, res);
      if (x_hi & 1) collect_(--x_hi, y_lo, y_hi, res);
    }
    return res;
  }

 private:
  int count_(int i, T y_lo, T y_hi) const {
    auto lo_it = std::lower_bound(data_[i].begin(), data_[i].end(), y_lo);
    auto hi_it = std::lower_bound(data_[i].begin(), data_[i].end(), y_hi);
    return int(hi_it - lo_it);
  }
  void collect_(int i, T y_lo, T y_hi, std::vector<T> &out) const {
    auto lo_it = std::lower_bound(data_[i].begin(), data_[i].end(), y_lo);
    auto hi_it = std::lower_bound(data_[i].begin(), data_[i].end(), y_hi);
    std::copy(lo_it, hi_it, std::back_inserter(out));
  }
};
