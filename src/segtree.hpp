template <typename Monoid>
struct SegTree {
  using T = typename Monoid::T;

  inline int n() const { return n_; }
  inline int offset() const { return offset_; }

  explicit SegTree(int n) : n_(n) {
    offset_ = 1;
    while (offset_ < n_) offset_ <<= 1;
    data_.assign(2 * offset_, Monoid::id());
  }

  explicit SegTree(const std::vector<T> &leaves) : n_(leaves.size()) {
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

  // Returns i-th value (0-indexed).
  T operator[](int i) const { return data_[offset_ + i]; }

  friend std::ostream &operator<<(std::ostream &os, const SegTree &st) {
    os << "[";
    for (int i = 0; i < st.n(); ++i) {
      if (i != 0) os << ", ";
      const auto &x = st[i];
      os << x;
    }
    return os << "]";
  }

  template <bool (*pred)(const T &)>
  int max_right(int l) {
    return max_right(l, [](const T &x) -> bool { return pred(x); });
  }
  template <class Predicate>
  int max_right(int l, Predicate pred) {
    assert(0 <= l && l <= n_);
    assert(pred(Monoid::id()));
    if (l == n_) return n_;
    l += offset_;
    T sm = Monoid::id();
    do {
      while (l % 2 == 0) l >>= 1;
      if (!pred(Monoid::op(sm, data_[l]))) {
        while (l < offset_) {
          l = (2 * l);
          if (pred(Monoid::op(sm, data_[l]))) {
            sm = Monoid::op(sm, data_[l]);
            l++;
          }
        }
        return l - offset_;
      }
      sm = Monoid::op(sm, data_[l]);
      l++;
    } while ((l & -l) != l);
    return n_;
  }

  template <bool (*pred)(const T &)>
  int min_left(int r) {
    return min_left(r, [](const T &x) -> bool { return pred(x); });
  }
  template <class Predicate>
  int min_left(int r, Predicate pred) {
    assert(0 <= r && r <= n_);
    assert(pred(Monoid::id()));
    if (r == 0) return 0;
    r += offset_;
    T sm = Monoid::id();
    do {
      r--;
      while (r > 1 && (r % 2)) r >>= 1;
      if (!pred(Monoid::op(data_[r], sm))) {
        while (r < offset_) {
          r = (2 * r + 1);
          if (pred(Monoid::op(data_[r], sm))) {
            sm = Monoid::op(data_[r], sm);
            r--;
          }
        }
        return r + 1 - offset_;
      }
      sm = Monoid::op(data_[r], sm);
    } while ((r & -r) != r);
    return 0;
  }

 private:
  int n_;                // number of valid leaves.
  int offset_;           // where leaves start
  std::vector<T> data_;  // data size: 2*offset_
};
