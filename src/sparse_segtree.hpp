#include <iostream>
#include <unordered_map>

// SegTree implemented with an unordered_map.
// Lazily creates node values as necessary.
// Supports a broad range of indices without compression.

template <typename Monoid>
struct SparseSegTree {
  using T = typename Monoid::T;
  using i64 = long long;

 private:
  i64 n_;                            // number of valid leaves.
  i64 offset_;                       // where leaves start
  std::unordered_map<i64, T> data_;  // data size: 2*offset_

 public:
  inline i64 n() const { return n_; }
  inline i64 offset() const { return offset_; }

  explicit SparseSegTree(i64 n) : n_(n) {
    offset_ = 1;
    while (offset_ < n_) offset_ <<= 1;

    data_.reserve(1 << 20);
    data_.max_load_factor(0.25);
  }

  // Sets i-th value (0-indexed) to x.
  void set(i64 i, const T &x) {
    i64 k = offset_ + i;
    if (x == Monoid::id()) {
      data_.erase(k);
    } else {
      data_[k] = x;
    }
    // Update its ancestors.
    while (k > 1) {
      k >>= 1;
      const auto it0 = data_.find(k * 2);
      const auto it1 = data_.find(k * 2 + 1);
      if (it0 != data_.end() and it1 != data_.end()) {
        data_[k] = Monoid::op(it0->second, it1->second);
      } else if (it0 != data_.end()) {
        data_[k] = it0->second;
      } else if (it1 != data_.end()) {
        data_[k] = it1->second;
      } else {
        data_.erase(k);
      }
    }
  }

  // Queries by [l,r) range (0-indexed, half-open interval).
  T fold(i64 l, i64 r) const {
    l = std::max(l, 0LL) + offset_;
    r = std::min(r, offset_) + offset_;
    T vleft = Monoid::id(), vright = Monoid::id();
    for (; l < r; l >>= 1, r >>= 1) {
      if (l & 1) {
        if (auto it = data_.find(l++); it != data_.end()) {
          vleft = Monoid::op(vleft, it->second);
        }
      }
      if (r & 1) {
        if (auto it = data_.find(--r); it != data_.end()) {
          vright = Monoid::op(it->second, vright);
        }
      }
    }
    return Monoid::op(vleft, vright);
  }

  T fold_all() const {
    if (auto it = data_.find(1); it != data_.end()) {
      return it->second;
    } else {
      return Monoid::id();
    }
  }

  // Returns i-th value (0-indexed).
  T operator[](i64 i) const {
    if (auto it = data_.find(offset_ + i); it != data_.end()) {
      return it->second;
    } else {
      return Monoid::id();
    }
  }

  friend std::ostream &operator<<(std::ostream &os, const SparseSegTree &st) {
    static const int kMaxOutput = 100;
    os << "[";
    for (i64 i = 0; i < std::min<i64>(st.n(), kMaxOutput); ++i) {
      if (i != 0) os << ", ";
      const auto &x = st[i];
      os << x;
    }
    if (st.n() > kMaxOutput) {
      os << ", ...";
    }
    return os << "]";
  }
};
