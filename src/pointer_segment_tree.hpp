#include <bits/stdc++.h>

template <typename Monoid>
struct SegmentTree {
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

  NodePtr root_;
  Int size_;
  NodePool *pool_;

  explicit SegmentTree(Int n, NodePool *pool = NO_DELETE())
      : size_(n), pool_(pool) {
    root_ = make_node(Monoid::id());
  }

  void set(Int k, T x) { set_(k, std::move(x), root_, 0, size_); }

  T fold(Int kl, Int kr) const { return fold_(kl, kr, root_, 0, size_); }
  T fold_all() const { return root_->data; }
  T operator[](Int k) const { return fold_(k, k + 1, root_, 0, size_); }

  std::vector<T> to_vec(Int size = -1) const {
    if (size < 0) size = size_;
    std::vector<T> res((size_t)min(size, size_));
    for (Int i = 0; i < size; ++i) {
      res[i] = (*this)[i];
    }
    return res;
  }

 private:
  NodePtr make_node(T data) const {
    NodePtr p = pool_->new_node();
    p->data = std::move(data);
    p->l = p->r = nullptr;
    return p;
  }

  void set_(Int k, T val, NodePtr np, Int nl, Int nr) {
    if (nl + 1 == nr) {
      np->data = std::move(val);
      return;
    }
    Int nm = (nl + nr) >> 1;
    if (k < nm) {
      if (np->l == nullptr) np->l = make_node(Monoid::id());
      set_(k, std::move(val), np->l, nl, nm);
    } else {
      if (np->r == nullptr) np->r = make_node(Monoid::id());
      set_(k, std::move(val), np->r, nm, nr);
    }
    np->data = Monoid::op(np->l == nullptr ? Monoid::id() : np->l->data,
                          np->r == nullptr ? Monoid::id() : np->r->data);
  }

  T fold_(Int kl, Int kr, NodePtr np, Int nl, Int nr) const {
    if (np == nullptr) return Monoid::id();
    if (nr <= kl or kr <= nl) return Monoid::id();
    if (kl <= nl and nr <= kr) return np->data;
    Int nm = (nl + nr) >> 1;
    return Monoid::op(fold_(kl, kr, np->l, nl, nm),
                      fold_(kl, kr, np->r, nm, nr));
  }

  static NodePool *NO_DELETE() {
    static NodePool kNoDeletePool;
    return &kNoDeletePool;
  }
};
