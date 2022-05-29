#include <bits/stdc++.h>

template <typename Monoid>
struct PersistentSegmentTree {
  using Int = long long;
  using T = typename Monoid::T;
  struct Node;
  using NodePtr = Node *;

  struct Node {
    T data;
    NodePtr l, r;
    Node() {}
    Node(const T &_data) : data(_data), l(nullptr), r(nullptr) {}
  };

  struct NodePool {
    static constexpr size_t kInitialBlockSize = 1u << 12;
    // Try smaller growth rate if MLE.
    static constexpr double kBlockSizeGrowthRate = 1.5;

    std::vector<std::unique_ptr<Node[]>> blocks_;
    size_t bsize_;
    size_t bi_;
    size_t ni_;

    NodePool() : bsize_(kInitialBlockSize), bi_(0), ni_(0) {
      blocks_.emplace_back(new Node[kInitialBlockSize]);
    }

    NodePtr new_node() {
      if (ni_ == bsize_) {
        bi_++;
        ni_ = 0;
        bsize_ *= kBlockSizeGrowthRate;
        blocks_.emplace_back(new Node[bsize_]);
      }
      return &blocks_[bi_][ni_++];
    }
  };

  NodePtr nil_;
  NodePtr root_;
  Int size_;
  NodePool *pool_;

  explicit PersistentSegmentTree(const std::vector<T> &v,
                                 NodePool *pool = NO_DELETE())
      : nil_(make_nil()), size_((Int)v.size()), pool_(pool) {
    root_ = build(v);
  }

  explicit PersistentSegmentTree(Int n, NodePool *pool = NO_DELETE())
      : nil_(make_nil()), root_(nil_), size_(n), pool_(pool) {}

  PersistentSegmentTree set(Int k, T x) const {
    assert(0 <= k and k < size_);
    NodePtr new_root = set_(k, std::move(x), root_, 0, size_);
    return {new_root, size_, pool_};
  }

  T fold(Int kl, Int kr) const { return fold_(kl, kr, root_, 0, size_); }
  T fold_all() const { return root_->data; }
  T operator[](Int k) const {
    assert(0 <= k and k < size_);
    return fold_(k, k + 1, root_, 0, size_);
  }

  std::vector<T> to_vec(Int size = -1) const {
    if (size < 0) size = size_;
    std::vector<T> res((size_t)min(size, size_));
    for (Int i = 0; i < size; ++i) {
      res[i] = (*this)[i];
    }
    return res;
  }

  template <class F>
  Int min_left(Int r, F pred) const {
    static_assert(std::is_invocable_r_v<bool, F, typename Monoid::T, int>,
                  "predicate must be invocable on (folded, left index).");
    assert(0 <= r && r <= size_);
    const auto &fpos = min_left_(r, pred, Monoid::id(), root_, 0, size_).first;
    return fpos.value_or(-1) + 1;
  }

  template <class F>
  Int max_right(Int l, F pred) const {
    static_assert(std::is_invocable_r_v<bool, F, typename Monoid::T, int>,
                  "predicate must be invocable on (folded, right index).");
    assert(0 <= l && l < size_);
    const auto &fpos = max_right_(l, pred, Monoid::id(), root_, 0, size_).first;
    return fpos.value_or(size_ + 1) - 1;
  }

 private:
  PersistentSegmentTree(NodePtr root, Int n, NodePool *pool)
      : nil_(make_nil()), root_(root), size_(n), pool_(pool) {}

  NodePtr build(const std::vector<T> &v) { return build(0, (Int)v.size(), v); }

  NodePtr build(Int l, Int r, const std::vector<T> &v) {
    if (l + 1 == r) return make_leaf(v[l]);
    Int m = (l + r) >> 1;
    return merge(build(l, m, v), build(m, r, v));
  }

  static NodePtr make_nil() {
    static Node nil_node(Monoid::id());
    nil_node.l = nil_node.r = &nil_node;
    return &nil_node;
  }

  NodePtr make_leaf(T data) const {
    NodePtr p = pool_->new_node();
    p->data = std::move(data);
    p->l = p->r = nil_;
    return p;
  }

  NodePtr merge(NodePtr l, NodePtr r) const {
    NodePtr p = pool_->new_node();
    p->data = Monoid::op(l->data, r->data);
    p->l = l;
    p->r = r;
    return p;
  }

  NodePtr set_(Int k, T val, NodePtr np, Int nl, Int nr) const {
    if (nl + 1 == nr) return make_leaf(std::move(val));
    Int nm = (nl + nr) >> 1;
    if (k < nm) return merge(set_(k, std::move(val), np->l, nl, nm), np->r);
    return merge(np->l, set_(k, std::move(val), np->r, nm, nr));
  }

  T fold_(Int kl, Int kr, NodePtr np, Int nl, Int nr) const {
    if (np == nil_) return Monoid::id();
    if (nr <= kl or kr <= nl) return Monoid::id();
    if (kl <= nl and nr <= kr) return np->data;
    Int nm = (nl + nr) >> 1;
    return Monoid::op(fold_(kl, kr, np->l, nl, nm),
                      fold_(kl, kr, np->r, nm, nr));
  }

  template <class F>
  std::pair<std::optional<Int>, typename Monoid::T> min_left_(
      Int kr, F pred, const typename Monoid::T &prval, NodePtr np, Int nl,
      Int nr) const {
    if (kr <= nl) return {std::nullopt, Monoid::id()};
    if (nr <= kr) {
      auto val = Monoid::op(np->data, prval);
      if (pred(val, nl)) {
        return {std::nullopt, std::move(val)};
      }
      if (nl + 1 == nr) {
        return {nl, std::move(val)};
      }
    }
    const Int nm = (nl + nr) >> 1;
    const auto rsub = min_left_(kr, pred, prval, np->r, nm, nr);
    if (rsub.first) {
      return rsub;
    }
    return min_left_(kr, pred, std::move(rsub.second), np->l, nl, nm);
  }

  template <class F>
  std::pair<std::optional<Int>, typename Monoid::T> max_right_(
      Int kl, F pred, const typename Monoid::T &plval, NodePtr np, Int nl,
      Int nr) const {
    if (nr <= kl) return {std::nullopt, Monoid::id()};
    if (kl <= nl) {
      auto val = Monoid::op(plval, np->data);
      if (pred(val, nl)) {
        return {std::nullopt, std::move(val)};
      }
      if (nl + 1 == nr) {
        return {nl, std::move(val)};
      }
    }
    const Int nm = (nl + nr) >> 1;
    const auto lsub = max_right_(kl, pred, plval, np->l, nl, nm);
    if (lsub.first) {
      return lsub;
    }
    return max_right_(kl, pred, std::move(lsub.second), np->r, nm, nr);
  }

  static NodePool *NO_DELETE() {
    static NodePool kNoDeletePool;
    return &kNoDeletePool;
  }
};
