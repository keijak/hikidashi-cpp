#include <bits/stdc++.h>

template <typename LazyMonoid>
struct PersistentLazySegmentTree {
  using Int = long long;
  using T = typename LazyMonoid::T;
  using F = typename LazyMonoid::F;
  struct Node;
  using NodePtr = Node *;

  struct Node {
    T data;
    F lazy;
    NodePtr l, r;
    Node() {}
    Node(const T &data, const F &lazy)
        : data(data), lazy(lazy), l(nullptr), r(nullptr) {}
  };

  struct NodePool {
    static constexpr size_t kInitialBlockSize = 1u << 12;
    static constexpr double kBlockSizeGrowthRate = 1.5;  // Try smaller rate if MLE.

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

  explicit PersistentLazySegmentTree(const std::vector<T> &v,
                                     NodePool *pool = NO_DELETE())
      : nil_(make_nil()), size_((Int)v.size()), pool_(pool) {
    root_ = build(v);
  }

  explicit PersistentLazySegmentTree(Int n, NodePool *pool = NO_DELETE())
      : nil_(make_nil()), root_(nil_), size_(n), pool_(pool) {}

  PersistentLazySegmentTree set(Int k, const T &x) const {
    NodePtr new_root = set_(k, x, root_, 0, size_);
    return {new_root, size_, pool_};
  }

  T fold(Int kl, Int kr) const {
    return fold_(kl, kr, LazyMonoid::f_id(), root_, 0, size_);
  }
  T fold_all() const { return root_->data; }
  T operator[](Int k) const {
    return fold_(k, k + 1, LazyMonoid::f_id(), root_, 0, size_);
  }

  PersistentLazySegmentTree apply(Int kl, Int kr, const F &f) const {
    NodePtr new_root = apply_(kl, kr, f, root_, 0, size_);
    return {new_root, size_, pool_};
  }

  std::vector<T> to_vec(Int size = -1) const {
    if (size < 0) size = size_;
    std::vector<T> res((size_t)min(size, size_));
    for (Int i = 0; i < size; ++i) {
      res[i] = (*this)[i];
    }
    return res;
  }

 private:
  PersistentLazySegmentTree(NodePtr root, Int n, NodePool *pool)
      : nil_(make_nil()), root_(root), size_(n), pool_(pool) {}

  NodePtr build(const std::vector<T> &v) { return build(0, (Int)v.size(), v); }

  NodePtr build(Int l, Int r, const std::vector<T> &v) {
    if (l + 1 == r) return make_leaf(v[l]);
    Int m = (l + r) >> 1;
    return merge(build(l, m, v), build(m, r, v));
  }

  static NodePtr make_nil() {
    static Node nil_node(LazyMonoid::id(), LazyMonoid::f_id());
    nil_node.l = nil_node.r = &nil_node;
    return &nil_node;
  }

  NodePtr make_leaf(T data) const {
    NodePtr p = pool_->new_node();
    p->data = std::move(data);
    p->lazy = LazyMonoid::f_id();
    p->l = p->r = nil_;
    return p;
  }

  NodePtr merge(NodePtr l, NodePtr r) const {
    NodePtr p = pool_->new_node();
    p->data = LazyMonoid::op(l->data, r->data);
    p->lazy = LazyMonoid::f_id();
    p->l = l;
    p->r = r;
    return p;
  }

  NodePtr set_(Int k, const T &val, NodePtr np, Int l, Int r) const {
    if (l + 1 == r) return make_leaf(val);
    NodePtr ltmp = apply_one(np->lazy, np->l);
    NodePtr rtmp = apply_one(np->lazy, np->r);
    Int m = (l + r) >> 1;
    if (k < m) {
      return merge(set_(k, val, ltmp, l, m), rtmp);
    } else {
      return merge(ltmp, set_(k, val, np->r, m, rtmp));
    }
  }

  T fold_(Int kl, Int kr, const F &f, NodePtr np, Int l, Int r) const {
    if (np == nil_) return LazyMonoid::id();
    if (r <= kl or kr <= l) return LazyMonoid::id();
    if (kl <= l and r <= kr) return LazyMonoid::f_apply(f, np->data);
    F f_down = LazyMonoid::f_compose(f, np->lazy);
    Int m = (l + r) >> 1;
    return LazyMonoid::op(fold_(kl, kr, f_down, np->l, l, m),
                          fold_(kl, kr, f_down, np->r, m, r));
  }

  NodePtr apply_one(const F &f, NodePtr np) const {
    NodePtr p = pool_->new_node();
    p->data = LazyMonoid::f_apply(f, np->data);
    p->lazy = LazyMonoid::f_compose(f, np->lazy);
    p->l = np->l;
    p->r = np->r;
    return p;
  }

  NodePtr apply_(Int kl, Int kr, const F &f, NodePtr np, Int l, Int r) const {
    if (r <= kl or kr <= l) return np;
    if (l + 1 == r) {  // leaf
      return make_leaf(LazyMonoid::f_apply(f, np->data));
    }
    if (kl <= l and r <= kr) {
      return apply_one(f, np);
    }
    auto m = (l + r) >> 1;
    NodePtr l2 = apply_(kl, kr, f, apply_one(np->lazy, np->l), l, m);
    NodePtr r2 = apply_(kl, kr, f, apply_one(np->lazy, np->r), m, r);
    return merge(l2, r2);
  }

  static NodePool *NO_DELETE() {
    static NodePool kNoDeletePool;
    return &kNoDeletePool;
  }
};
