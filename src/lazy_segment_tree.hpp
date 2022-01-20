#include <cassert>
#include <iostream>
#include <type_traits>
#include <vector>

template <typename Monoids>
struct LazySegmentTree {
  using T = typename Monoids::T;
  using F = typename Monoids::F;

  inline int size() const { return n_; }
  inline int offset() const { return offset_; }

  explicit LazySegmentTree(int n)
      : LazySegmentTree(std::vector<T>(n, Monoids::id())) {}

  explicit LazySegmentTree(const std::vector<T> &v) : n_(int(v.size())) {
    offset_ = 1;
    for (bits_ = 0; offset_ < n_; ++bits_) {
      offset_ <<= 1;
    }
    data_.assign(2 * offset_, Monoids::id());
    lazy_ = std::vector<F>(offset_, Monoids::f_id());
    for (int i = 0; i < n_; i++) data_[offset_ + i] = v[i];
    for (int i = offset_ - 1; i >= 1; i--) {
      update(i);
    }
  }

  void set(int p, T x) {
    assert(0 <= p && p < n_);
    p += offset_;
    // Update the leaf.
    for (int i = bits_; i >= 1; i--) push(p >> i);
    data_[p] = x;
    // Update its ancestors.
    for (int i = 1; i <= bits_; i++) update(p >> i);
  }

  const T &operator[](int p) const {
    assert(0 <= p && p < n_);
    p += offset_;
    for (int i = bits_; i >= 1; i--) push(p >> i);
    return data_[p];
  }

  T fold(int l, int r) const {
    assert(0 <= l && l <= r && r <= n_);
    if (l == r) return Monoids::id();

    l += offset_;
    r += offset_;

    for (int i = bits_; i >= 1; i--) {
      if (((l >> i) << i) != l) push(l >> i);
      if (((r >> i) << i) != r) push(r >> i);
    }

    T sml = Monoids::id(), smr = Monoids::id();
    while (l < r) {
      if (l & 1) sml = Monoids::op(sml, data_[l++]);
      if (r & 1) smr = Monoids::op(data_[--r], smr);
      l >>= 1;
      r >>= 1;
    }

    return Monoids::op(sml, smr);
  }

  T fold_all() const { return data_[1]; }

  void apply(int p, F f) {
    assert(0 <= p && p < n_);
    p += offset_;
    for (int i = bits_; i >= 1; i--) push(p >> i);
    data_[p] = Monoids::apply(f, data_[p]);
    for (int i = 1; i <= bits_; i++) update(p >> i);
  }
  void apply(int l, int r, F f) {
    assert(0 <= l && l <= r && r <= n_);
    if (l == r) return;

    l += offset_;
    r += offset_;

    for (int i = bits_; i >= 1; i--) {
      if (((l >> i) << i) != l) push(l >> i);
      if (((r >> i) << i) != r) push((r - 1) >> i);
    }

    {
      int l2 = l, r2 = r;
      while (l < r) {
        if (l & 1) apply_all(l++, f);
        if (r & 1) apply_all(--r, f);
        l >>= 1;
        r >>= 1;
      }
      l = l2;
      r = r2;
    }

    for (int i = 1; i <= bits_; i++) {
      if (((l >> i) << i) != l) update(l >> i);
      if (((r >> i) << i) != r) update((r - 1) >> i);
    }
  }

  friend std::ostream &operator<<(std::ostream &os, const LazySegmentTree &st) {
    os << "[";
    for (int i = 0; i < st.size(); ++i) {
      if (i != 0) os << ", ";
      const auto &x = st[i];
      os << x;
    }
    return os << "]";
  }

  void update(int k) { data_[k] = Monoids::op(data_[2 * k], data_[2 * k + 1]); }
  void apply_all(int k, F f) const {
    data_[k] = Monoids::f_apply(f, data_[k]);
    if (k < offset_) {
      lazy_[k] = Monoids::f_compose(f, lazy_[k]);
#ifdef SEGMENT_TREE_BEATS
      if (data_[k].failed) push(k), update(k);
#endif
    }
  }
  void push(int k) const {
    apply_all(2 * k, lazy_[k]);
    apply_all(2 * k + 1, lazy_[k]);
    lazy_[k] = Monoids::f_id();
  }

  int n_, offset_, bits_;
  mutable std::vector<T> data_;
  mutable std::vector<F> lazy_;
};

template <class M, class F>
int max_right(const LazySegmentTree<M> &seg, int l, F pred) {
  static_assert(std::is_invocable_r_v<bool, F, typename M::T>,
                "predicate must be invocable on the value type");
  assert(0 <= l && l <= seg.size());
  assert(pred(M::id()));
  if (l == seg.size()) return seg.size();
  l += seg.offset_;
  for (int i = seg.bits_; i >= 1; --i) {
    seg.push(l >> i);
  }
  auto sm = M::id();
  do {
    while (l % 2 == 0) l >>= 1;
    if (not pred(M::op(sm, seg.data_[l]))) {
      while (l < seg.offset_) {
        seg.push(l);
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

template <class M, class F>
int min_left(const LazySegmentTree<M> &seg, int r, F pred) {
  static_assert(std::is_invocable_r_v<bool, F, typename M::T>,
                "predicate must be invocable on the value type");
  assert(0 <= r && r <= seg.n_);
  assert(pred(M::id()));
  if (r == 0) return 0;
  r += seg.offset_;
  for (int i = seg.bits_; i >= 1; --i) {
    seg.push((r - 1) >> i);
  }
  auto sm = M::id();
  do {
    --r;
    while (r > 1 && (r % 2)) r >>= 1;
    if (not pred(M::op(seg.data_[r], sm))) {
      while (r < seg.offset_) {
        seg.push(r);
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
