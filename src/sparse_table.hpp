#include <cassert>
#include <limits>
#include <vector>

// Generic Sparse Table on a semilattice operation.
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

// Range Min/Max Query based on Sparse Table.
// - Initialization: O(n*log(n))
// - Query: O(1)
//
// Usage:
//   SparseTableRMQ rmq(a.size(), [&](int i, int j){ return a[i] < a[j]; });
//   auto minval = a[rmq.fold(l, r)];
template <class BetterOp>
struct SparseTableRMQ {
  static_assert(std::is_invocable_r_v<bool, BetterOp, int, int>);

  int n_;                 // sequence size
  BetterOp better_than_;  // checks if lhs is strictly better than rhs.
  std::vector<std::vector<int>> sparse_table_;

  SparseTableRMQ(int n, BetterOp better)
      : n_(n),
        better_than_(std::move(better)),
        sparse_table_(n == 0 ? 1 : (msb_log(n) + 1), std::vector<int>(n)) {
    for (int j = 0; j < n; ++j) {
      sparse_table_[0][j] = j;
    }
    for (int i = 0, height = int(sparse_table_.size()) - 1; i < height; ++i) {
      for (int j = 0; j < n; ++j) {
        sparse_table_[i + 1][j] =
            better_index(sparse_table_[i][j],
                         sparse_table_[i][std::min(j + (1 << i), n - 1)]);
      }
    }
  }

  // Returns the index of the best value in [l, r) (half-open interval).
  inline int fold(int l, int r) const {
    assert(0 <= l and l < r);
    assert(1 <= r and r <= n_);
    const int k = msb_log(r - l);
    const int l2 = r - (1 << k) + 1;
    return better_index(sparse_table_[k][l], sparse_table_[k][l2]);
  }

 private:
  // Log base 2 of the most significant bit that is set to 1.
  static inline int msb_log(unsigned x) {
    return std::numeric_limits<unsigned>::digits - __builtin_clz(x) - 1;
  }

  inline int better_index(int i, int j) const {
    return better_than_(i, j) ? i : j;
  }
};
