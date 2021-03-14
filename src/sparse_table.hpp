#include <vector>

template <class SemiLattice>
struct SparseTable {
  using T = typename SemiLattice::T;

  explicit SparseTable(const std::vector<T> &vec) { init(vec); }

  // Queries by [l,r) range (0-indexed, half-open interval).
  T fold(int l, int r) const {
    l = std::max(l, 0);
    r = std::min(r, n_);
    if (l >= r) {
      return SemiLattice::id();
    }
    const T &lval = data_[height_[r - l]][l];
    const T &rval = data_[height_[r - l]][r - (1 << height_[r - l])];
    return SemiLattice::op(lval, rval);
  }

  // Returns i-th value (0-indexed).
  T operator[](int i) const {
    assert(0 <= i and i < n_);
    return data_[height_[1]][i];
  }

 private:
  void init(const std::vector<T> &vec) {
    int n = vec.size(), h = 0;
    n_ = n;
    while ((1 << h) <= n) ++h;
    data_.assign(h, std::vector<T>(1 << h, SemiLattice::id()));
    height_.assign(n + 1, 0);
    for (int i = 2; i <= n; i++) {
      height_[i] = height_[i >> 1] + 1;
    }
    for (int i = 0; i < n; ++i) {
      data_[0][i] = vec[i];
    }
    for (int i = 1; i < h; ++i)
      for (int j = 0; j < n; ++j)
        data_[i][j] = SemiLattice::op(
            data_[i - 1][j], data_[i - 1][std::min(j + (1 << (i - 1)), n - 1)]);
  }

  int n_;  // number of elements.
  std::vector<std::vector<T>> data_;
  std::vector<int> height_;
};
