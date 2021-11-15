#include <bits/stdc++.h>

template <typename Monoid>
struct DualSegmentTree {
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

  NodePtr root_;
  Int size_;
  NodePool *pool_;

  explicit DualSegmentTree(Int n, NodePool *pool = NO_DELETE())
      : size_(n), pool_(pool) {
    root_ = make_leaf(Monoid::id());
  }

  void apply(Int kl, Int kr, T x) { apply_(kl, kr, x, root_, 0, size_); }
  T operator[](Int k) const { return get_(k, root_, 0, size_); }

  std::vector<T> to_vec(Int size = -1) const {
    if (size < 0) size = size_;
    std::vector<T> res((size_t)min(size, size_));
    for (Int i = 0; i < size; ++i) {
      res[i] = (*this)[i];
    }
    return res;
  }

 private:
  NodePtr make_leaf(T data) const {
    NodePtr p = pool_->new_node();
    p->data = std::move(data);
    p->l = p->r = nullptr;
    return p;
  }

  void apply_(Int kl, Int kr, const T &val, NodePtr np, Int nl, Int nr) {
    if (np == nullptr) return;
    if (nr <= kl or kr <= nl) return;
    if (kl <= nl and nr <= kr) {
      np->data = Monoid::op(np->data, val);
      return;
    }
    Int nm = (nl + nr) >> 1;
    if (np->l == nullptr) np->l = make_leaf(Monoid::id());
    apply_(kl, kr, val, np->l, nl, nm);
    if (np->r == nullptr) np->r = make_leaf(Monoid::id());
    apply_(kl, kr, val, np->r, nm, nr);
  }

  T get_(Int k, NodePtr np, Int nl, Int nr) const {
    if (np == nullptr) return Monoid::id();
    if (nl + 1 == nr) return np->data;
    Int nm = (nl + nr) >> 1;
    if (k < nm) {
      return Monoid::op(get_(k, np->l, nl, nm), np->data);
    } else {
      return Monoid::op(get_(k, np->r, nm, nr), np->data);
    }
  }

  static NodePool *NO_DELETE() {
    static NodePool kNoDeletePool;
    return &kNoDeletePool;
  }
};

template <class M>
void set_value(DualSegmentTree<M> &seg, Int k, const typename M::T &val) {
  auto pval = seg[k];
  auto delta = M::op(M::invert(pval), val);
  seg.apply(k, k + 1, delta);
}
