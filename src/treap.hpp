// Treap implements a sequence.
//
// - split: O(log N)
// - merge (concatenation): O(log N)
// - random access: O(log N)
// - insert at pos: O(log N)
// - erase at pos: O(log N)
//
// base:
// https://github.com/maze1230/Library/blob/c20e57b9cc290664407146438c4f62712a7fcb81/Tree/Treap.cpp

#include <bits/stdc++.h>

template <class T, std::size_t NODE_NUM = 1000000>
class Treap {
 private:
  struct Node {
   public:
    T v;
    size_t pri;
    int size;
    Node *lch = nullptr, *rch = nullptr;

    Node() : v(), pri(0), size(1) {}
    Node(T v, size_t pri) : v(v), pri(pri), size(1) {}
  };
  using NodePtr = struct Node *;
  NodePtr root;

  explicit Treap(NodePtr p) : root(p) {}

  static NodePtr create_node(const T v, size_t pri) {
    static std::unique_ptr<Node[]> pool(new Node[NODE_NUM]);
    static size_t i = 0;
    return &(pool[i++] = Node(v, pri));
  }

  static size_t random_priority() {
    static std::mt19937_64 engine(std::random_device{}());
    static std::uniform_int_distribution<size_t> dist(0);
    return dist(engine);
  }

  static int count_(NodePtr p) { return p == nullptr ? 0 : p->size; }

  static NodePtr update_(NodePtr p) {
    p->size = count(p->lch) + count(p->rch) + 1;
    return p;
  }

  static NodePtr merge_(NodePtr l, NodePtr r) {
    if (l == nullptr || r == nullptr) return l == nullptr ? r : l;
    if (l->pri > r->pri) {
      l->rch = merge_(l->rch, r);
      return update_(l);
    } else {
      r->lch = merge_(l, r->lch);
      return update_(r);
    }
  }

  static std::pair<NodePtr, NodePtr> split_(NodePtr p, int k) {
    if (p == nullptr) return {nullptr, nullptr};

    if (k <= count(p->lch)) {
      auto ret = split_(p->lch, k);
      p->lch = ret.second;
      return {ret.first, update_(p)};
    } else {
      auto ret = split_(p->rch, k - count(p->lch) - 1);
      p->rch = ret.first;
      return {update_(p), ret.second};
    }
  }

  void insert_(int k, T v, size_t pri) {
    if (root == nullptr) {
      root = create_node(v, pri);
      return;
    }
    NodePtr l, r;
    std::tie(l, r) = split_(root, k);
    root = merge_(l, create_node(std::move(v), pri));
    root = merge_(root, r);
  }

  void to_vec_(NodePtr p, typename std::vector<T>::iterator itr) {
    if (p == nullptr) return;
    to_vec_(p->lch, itr);
    *(itr + count(p->lch)) = p->v;
    to_vec_(p->rch, itr + count(p->lch) + 1);
  }

 public:
  Treap() : root(nullptr) {}

  int size() { return count_(root); }

  static Treap merge(Treap l, Treap r) { return Treap(merge_(l.root, r.root)); }

  std::pair<Treap, Treap> split(int k) {
    auto ret = split_(root, k);
    return {Treap(ret.first), Treap(ret.second)};
  }

  void insert(int k, T v) { insert_(k, std::move(v), random_priority()); }

  T erase(int k) {
    Treap l, _, r;
    std::tie(l, r) = split(k + 1);
    std::tie(l, _) = l.split(k);
    root = merge(l.root, r.root);
    return _.root->v;
  }

  T operator[](int k) {
    Treap l, _, r;
    std::tie(l, r) = split(k + 1);
    std::tie(l, _) = l.split(k);
    T ret = _.root->v;
    root = merge_(merge_(l.root, _.root), r.root);
    return ret;
  }

  void set(int k, T v) {
    Treap l, _, r;
    std::tie(l, r) = split(k + 1);
    std::tie(l, _) = l.split(k);
    _.root->v = std::move(v);
    root = merge_(merge_(l.root, _.root), r.root);
  }

  std::vector<T> to_vec(NodePtr p = nullptr) {
    if (p == nullptr) p = root;
    std::vector<T> v(count(p));
    to_vec_(p, v.begin());
    return v;
  }
};
