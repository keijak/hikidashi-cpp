// Sparse Table 2D
// base:
// https://kopricky.github.io/code/DataStructure_Advanced/sparse_table_2D.html
#include <bits/stdc++.h>

template <typename T>
struct Array4d {
  std::unique_ptr<T[]> data_;
  size_t b0_ = 0, b1_ = 0, b2_ = 0;

  Array4d(size_t d0, size_t d1, size_t d2, size_t d3)
      : data_(new T[d0 * d1 * d2 * d3]),
        b0_(d1 * d2 * d3),
        b1_(d2 * d3),
        b2_(d3) {}

  inline T &get(size_t i0, size_t i1, size_t i2, size_t i3) const {
    return data_[i0 * b0_ + i1 * b1_ + i2 * b2_ + i3];
  }
};

template <class SemiLattice>
struct SparseTable2d {
  using T = typename SemiLattice::T;

  int nrow_, ncol_;
  Array4d<T> table_;

  SparseTable2d(const std::vector<std::vector<T>> &v)
      : nrow_((int)v.size()),
        ncol_(nrow_ == 0 ? 0 : (int)v[0].size()),
        table_(log_base2(nrow_) + 1, log_base2(ncol_) + 1, nrow_, ncol_) {
    for (int i = 0; i < nrow_; ++i) {
      for (int j = 0; j < ncol_; ++j) {
        table_.get(0, 0, i, j) = v[i][j];
      }
    }
    for (int k = 1; (1 << k) <= nrow_; ++k) {
      for (int i = 0; i + (1 << k) <= nrow_; ++i) {
        for (int j = 0; j < ncol_; ++j) {
          table_.get(k, 0, i, j) =
              SemiLattice::op(table_.get(k - 1, 0, i, j),
                              table_.get(k - 1, 0, i + (1 << (k - 1)), j));
        }
      }
    }
    for (int k = 0; (1 << k) <= nrow_; ++k) {
      for (int l = 1; (1 << l) <= ncol_; ++l) {
        for (int i = 0; i + (1 << k) <= nrow_; ++i) {
          for (int j = 0; j + (1 << l) <= ncol_; ++j) {
            table_.get(k, l, i, j) =
                SemiLattice::op(table_.get(k, l - 1, i, j),
                                table_.get(k, l - 1, i, j + (1 << (l - 1))));
          }
        }
      }
    }
  }

  T fold(int x_lo, int x_hi, int y_lo, int y_hi) const {
    const int kx = log_base2(x_hi - x_lo);
    const int ky = log_base2(y_hi - y_lo);
    return SemiLattice::op(
        {table_.get(kx, ky, x_lo, y_lo),
         table_.get(kx, ky, x_hi - (1 << kx), y_lo),
         table_.get(kx, ky, x_lo, y_hi - (1 << ky)),
         table_.get(kx, ky, x_hi - (1 << kx), y_hi - (1 << ky))});
  }

 private:
  static inline int log_base2(int x) {
    return (x <= 0) ? 0
                    : (std::numeric_limits<unsigned>::digits -
                       __builtin_clz((unsigned)x) - 1);
  }
};

struct MaxOp {
  using T = int;
  static inline T op(const T &x, const T &y) { return std::max(x, y); }
  static inline T op(std::initializer_list<T> args) { return std::max(args); }
};

struct MinOp {
  using T = int;
  static inline T op(const T &x, const T &y) { return std::min(x, y); }
  static inline T op(std::initializer_list<T> args) { return std::min(args); }
};
