#include <bits/stdc++.h>
using i64 = long long;

template <typename Monoid>
struct PersistentSegmentTree {
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
    static constexpr size_t kInitBlockSize = 1u << 12;
    static constexpr double kBlockSizeGrowthRate = 1.5;  // Decrease if MLE.

    std::vector<std::unique_ptr<Node[]>> blocks_;
    size_t bsize_;
    size_t bi_;
    size_t ni_;

    NodePool() : bsize_(kInitBlockSize), bi_(0), ni_(0) {
      blocks_.emplace_back(new Node[kInitBlockSize]);
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
  i64 size_;
  NodePool *pool_;

  explicit PersistentSegmentTree(const std::vector<T> &v,
                                 NodePool *pool = NO_DELETE())
      : nil_(make_nil()), size_((i64)v.size()), pool_(pool) {
    root_ = build(v);
  }

  explicit PersistentSegmentTree(i64 n, NodePool *pool = NO_DELETE())
      : nil_(make_nil()), root_(nil_), size_(n), pool_(pool) {}

  PersistentSegmentTree set(i64 k, T x) const {
    NodePtr new_root = set_(k, std::move(x), root_, 0, size_);
    return {new_root, size_, pool_};
  }

  T fold(i64 kl, i64 kr) const { return fold_(kl, kr, root_, 0, size_); }
  T operator[](i64 k) const { return fold_(k, k + 1, root_, 0, size_); }

 private:
  PersistentSegmentTree(NodePtr root, i64 n, NodePool *pool)
      : nil_(make_nil()), root_(root), size_(n), pool_(pool) {}

  NodePtr build(const std::vector<T> &v) { return build(0, (i64)v.size(), v); }

  NodePtr build(i64 l, i64 r, const std::vector<T> &v) {
    if (l + 1 == r) return make_leaf(v[l]);
    i64 m = (l + r) >> 1;
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

  NodePtr set_(i64 k, T val, NodePtr n, i64 l, i64 r) const {
    if (l + 1 == r) return make_leaf(std::move(val));
    i64 m = (l + r) >> 1;
    if (k < m) return merge(set_(k, std::move(val), n->l, l, m), n->r);
    return merge(n->l, set_(k, std::move(val), n->r, m, r));
  }

  T fold_(i64 kl, i64 kr, NodePtr n, i64 l, i64 r) const {
    if (n == nil_) return Monoid::id();
    if (r <= kl or kr <= l) return Monoid::id();
    if (kl <= l and r <= kr) return n->data;
    i64 m = (l + r) >> 1;
    return Monoid::op(fold_(kl, kr, n->l, l, m), fold_(kl, kr, n->r, m, r));
  }

  static NodePool *NO_DELETE() {
    static NodePool kNoDeletePool;
    return &kNoDeletePool;
  }
};
