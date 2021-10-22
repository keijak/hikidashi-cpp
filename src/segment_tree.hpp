#include <cassert>
#include <iostream>
#include <type_traits>
#include <vector>

template <typename Monoid>
struct SegmentTree {
  using T = typename Monoid::T;

  int n_;                // number of valid leaves.
  int offset_;           // where leaves start
  std::vector<T> data_;  // data size: 2*offset_

  inline int size() const { return n_; }
  inline int offset() const { return offset_; }

  explicit SegmentTree(int n) : n_(n) {
    offset_ = 1;
    while (offset_ < n_) offset_ <<= 1;
    data_.assign(2 * offset_, Monoid::id());
  }

  explicit SegmentTree(const std::vector<T> &leaves) : n_(leaves.size()) {
    offset_ = 1;
    while (offset_ < n_) offset_ <<= 1;
    data_.assign(2 * offset_, Monoid::id());
    for (int i = 0; i < n_; ++i) {
      data_[offset_ + i] = leaves[i];
    }
    for (int i = offset_ - 1; i > 0; --i) {
      data_[i] = Monoid::op(data_[i * 2], data_[i * 2 + 1]);
    }
  }

  // Sets i-th value (0-indexed) to x.
  void set(int i, const T &x) {
    int k = offset_ + i;
    data_[k] = x;
    // Update its ancestors.
    while (k > 1) {
      k >>= 1;
      data_[k] = Monoid::op(data_[k * 2], data_[k * 2 + 1]);
    }
  }

  // Queries by [l,r) range (0-indexed, half-open interval).
  T fold(int l, int r) const {
    l = std::max(l, 0) + offset_;
    r = std::min(r, offset_) + offset_;
    T vleft = Monoid::id(), vright = Monoid::id();
    for (; l < r; l >>= 1, r >>= 1) {
      if (l & 1) vleft = Monoid::op(vleft, data_[l++]);
      if (r & 1) vright = Monoid::op(data_[--r], vright);
    }
    return Monoid::op(vleft, vright);
  }

  T fold_all() const { return data_[1]; }

  // Returns i-th value (0-indexed).
  T operator[](int i) const { return data_[offset_ + i]; }

  friend std::ostream &operator<<(std::ostream &os, const SegmentTree &st) {
    os << "[";
    for (int i = 0; i < st.size(); ++i) {
      if (i != 0) os << ", ";
      const auto &x = st[i];
      os << x;
    }
    return os << "]";
  }
};

// Fix the left bound, extend the right bound as much as possible.
template <class M, class F>
int max_right(const SegmentTree<M> &seg, int l, F pred) {
  static_assert(std::is_invocable_r_v<bool, F, typename M::T>,
                "predicate must be invocable on the value type");
  assert(0 <= l && l <= seg.size());
  assert(pred(M::id()));
  if (l == seg.size()) return seg.size();
  l += seg.offset_;
  auto sm = M::id();
  do {
    while (l % 2 == 0) l >>= 1;
    if (not pred(M::op(sm, seg.data_[l]))) {
      while (l < seg.offset_) {
        l <<= 1;
        if (pred(M::op(sm, seg.data_[l]))) {
          sm = M::op(sm, seg.data_[l]);
          ++l;
        }
      }
      return l - seg.offset_;
    }
    sm = M::op(sm, seg.data_[l]);
    ++l;
  } while ((l & -l) != l);
  return seg.size();
}

// Fix the right bound, extend the left bound as much as possible.
template <class M, class F>
int min_left(const SegmentTree<M> &seg, int r, F pred) {
  static_assert(std::is_invocable_r_v<bool, F, typename M::T>,
                "predicate must be invocable on the value type");
  assert(0 <= r && r <= seg.size());
  assert(pred(M::id()));
  r += seg.offset_;
  auto sm = M::id();
  do {
    --r;
    while (r > 1 && (r % 2)) r >>= 1;
    if (not pred(M::op(seg.data_[r], sm))) {
      while (r < seg.offset_) {
        r = 2 * r + 1;
        if (pred(M::op(seg.data_[r], sm))) {
          sm = M::op(seg.data_[r], sm);
          --r;
        }
      }
      return r + 1 - seg.offset_;
    }
    sm = M::op(seg.data_[r], sm);
  } while ((r & -r) != r);
  return 0;
}
