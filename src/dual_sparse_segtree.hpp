#include <bits/stdc++.h>

// range-apply point-get.
template <typename Monoid>
struct DualSparseSegTree {
  using T = typename Monoid::T;
  using i64 = long long;

  i64 size_;
  std::unordered_map<i64, T> data_;

  explicit DualSparseSegTree(i64 n) : size_(n) {}

  inline i64 size() const { return size_; }

  // Returns i-th value.
  T operator[](i64 i) const {
    i64 k = size_ + i;
    auto it = data_.find(k);
    T res = (it != data_.end()) ? it->second : Monoid::id();
    while (k > 1) {
      k >>= 1;
      it = data_.find(k);
      if (it != data_.end()) {
        res = Monoid::op(res, it->second);
      }
    }
    return res;
  }

  // Applies a value to [l,r) range.
  void apply(i64 l, i64 r, T val) {
    l += size_;
    r += size_;
    for (; l < r; l >>= 1, r >>= 1) {
      if (l & 1) {
        if (auto it = data_.find(l); it != data_.end()) {
          data_[l] = Monoid::op(it->second, val);
        } else {
          data_[l] = val;
        }
        ++l;
      }
      if (r & 1) {
        --r;
        if (auto it = data_.find(r); it != data_.end()) {
          data_[r] = Monoid::op(it->second, val);
        } else {
          data_[r] = val;
        }
      }
    }
  }

  friend std::ostream &operator<<(std::ostream &os,
                                  const DualSparseSegTree &st) {
    static const int kMaxOutput = 100;
    os << "[";
    for (i64 i = 0; i < std::min<i64>(st.n(), kMaxOutput); ++i) {
      if (i != 0) os << ", ";
      os << st[i];
    }
    if (st.n() > kMaxOutput) {
      os << ", ...";
    }
    return os << "]";
  }
};
