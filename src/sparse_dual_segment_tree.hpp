#include <bits/stdc++.h>

// range-apply point-get.
template <typename Monoid>
struct SparseDualSegmentTree {
  using T = typename Monoid::T;
  using Int = long long;

  Int size_;
  std::unordered_map<Int, T> data_;

  explicit SparseDualSegmentTree(Int n) : size_(n) {}

  inline Int size() const { return size_; }

  // Returns i-th value.
  T operator[](Int i) const {
    Int k = size_ + i;
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
  void apply(Int l, Int r, T val) {
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
                                  const SparseDualSegmentTree &st) {
    static const int kMaxOutput = 100;
    os << "[";
    for (Int i = 0; i < std::min<Int>(st.n(), kMaxOutput); ++i) {
      if (i != 0) os << ", ";
      os << st[i];
    }
    if (st.n() > kMaxOutput) {
      os << ", ...";
    }
    return os << "]";
  }
};
