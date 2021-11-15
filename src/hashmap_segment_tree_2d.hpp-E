#include <bits/stdc++.h>

// Hashmap-based 2d segment tree.
// Less performant and more memory-efficient than a pointer-based
// implementation.
template <typename Monoid>
struct SegmentTree2d {
  using Int = long long;
  using T = typename Monoid::T;

  Int nrow_;
  Int ncol_;
  Int row_offset_;
  Int col_offset_;
  std::vector<std::unordered_map<Int, T>> data_;

  SegmentTree2d(Int nrow, Int ncol) : nrow_(nrow), ncol_(ncol) {
    row_offset_ = 1;
    while (row_offset_ < nrow) row_offset_ <<= 1;
    col_offset_ = 1;
    while (col_offset_ < ncol) col_offset_ <<= 1;
    data_.resize(2 * row_offset_);
    data_[1][1] = Monoid::id();
  }

  void set(Int i, Int j, T x) { set_x(i, j, std::move(x), 1, 0, nrow_); }

  T fold(Int iu, Int id, Int jl, Int jr) const {
    return fold_x(iu, id, jl, jr, 1, 0, nrow_);
  }
  T fold_all() const { return data_[1][1]; }
  T get(Int i, Int j) const { return fold(i, i + 1, j, j + 1); }

  std::vector<std::vector<T>> to_vec(Int nrow = -1, Int ncol = -1) const {
    if (nrow < 0) nrow = nrow_;
    if (ncol < 0) ncol = ncol_;
    nrow = min(nrow, nrow_);
    ncol = min(ncol, ncol_);
    auto res = std::vector(nrow, std::vector(ncol, T{}));
    for (Int i = 0; i < nrow; ++i) {
      for (Int j = 0; j < ncol; ++j) {
        res[i][j] = get(i, j);
      }
    }
    return res;
  }

 private:
  void set_y(Int k, T val, Int row, Int nu, Int nd, Int col, Int nl, Int nr) {
    if (k < nl or k >= nr) return;
    if (nl + 1 == nr) {
      if (nu + 1 == nd) {
        data_[row][col] = std::move(val);
      } else {
        auto &udata = data_[row * 2];
        auto &ddata = data_[row * 2 + 1];
        auto uit = udata.find(col);
        auto dit = ddata.find(col);
        data_[row][col] =
            Monoid::op(uit == udata.end() ? Monoid::id() : uit->second,
                       dit == ddata.end() ? Monoid::id() : dit->second);
      }
      return;
    }
    Int nm = (nl + nr) >> 1;
    if (k < nm) {
      set_y(k, std::move(val), row, nu, nd, col * 2, nl, nm);
    } else {
      set_y(k, std::move(val), row, nu, nd, col * 2 + 1, nm, nr);
    }
    auto &rdata = data_[row];
    auto lit = rdata.find(col * 2);
    auto rit = rdata.find(col * 2 + 1);
    rdata[col] = Monoid::op(lit == rdata.end() ? Monoid::id() : lit->second,
                            rit == rdata.end() ? Monoid::id() : rit->second);
  }

  void set_x(Int i, Int j, T val, Int row, Int nu, Int nd) {
    if (i < nu or i >= nd) return;
    if (nu + 1 != nd) {
      Int nm = (nu + nd) >> 1;
      if (i < nm) {
        set_x(i, j, val, row * 2, nu, nm);
      } else {
        set_x(i, j, val, row * 2 + 1, nm, nd);
      }
    }
    set_y(j, std::move(val), row, nu, nd, 1, 0, ncol_);
  }

  T fold_y(Int jl, Int jr, const unordered_map<Int, T> &rdata, Int col, Int nl,
           Int nr) const {
    if (nr <= jl or jr <= nl) return Monoid::id();
    auto cit = rdata.find(col);
    if (cit == rdata.end()) return Monoid::id();
    if (jl <= nl and nr <= jr) return cit->second;
    Int nm = (nl + nr) >> 1;
    return Monoid::op(fold_y(jl, jr, rdata, col * 2, nl, nm),
                      fold_y(jl, jr, rdata, col * 2 + 1, nm, nr));
  }

  T fold_x(Int iu, Int id, Int jl, Int jr, Int row, Int nu, Int nd) const {
    if (nd <= iu or id <= nu) return Monoid::id();
    if (data_[row].empty()) return Monoid::id();
    if (iu <= nu and nd <= id) {
      return fold_y(jl, jr, data_[row], 1, 0, ncol_);
    }
    Int nm = (nu + nd) >> 1;
    return Monoid::op(fold_x(iu, id, jl, jr, row * 2, nu, nm),
                      fold_x(iu, id, jl, jr, row * 2 + 1, nm, nd));
  }
};
