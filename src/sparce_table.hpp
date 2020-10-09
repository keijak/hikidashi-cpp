template <class SemiLattice>
struct SparseTable {
  using T = typename SemiLattice::T;

  explicit SparseTable(const std::vector<T> &vec) { init(vec); }

  T fold(int l, int r) const {
    const T &lval = data_[height_[r - l]][l];
    const T &rval = data_[height_[r - l]][r - (1 << height_[r - l])];
    return SemiLattice::op(lval, rval);
  }

 private:
  void init(const std::vector<T> &vec) {
    int n = vec.size(), h = 0;
    while ((1 << h) < n) ++h;
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

  std::vector<std::vector<T>> data_;
  std::vector<int> height_;
};
