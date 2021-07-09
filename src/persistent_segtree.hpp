#include <bits/stdc++.h>
using i64 = long long;

template <typename Monoid, int NODES = 20000000>
struct PersistentSegmentTree {
  using T = typename Monoid::T;

  struct Node {
    T data;
    Node *l, *r;
    Node() {}
    Node(const T &_data) : data(_data), l(nullptr), r(nullptr) {}
  };

  Node *pool;
  int pid;
  i64 N;
  Node *nil;
  std::vector<Node *> roots;

  explicit PersistentSegmentTree(const std::vector<T> &v)
      : pid(0), nil(nullptr) {
    pool = new Node[NODES];
    nil = my_new(Monoid::id());
    nil->l = nil->r = nil;
    roots.reserve(262144);
    roots.push_back(build(v));
  }

  explicit PersistentSegmentTree(i64 N_) : pid(0), N(N_), nil(nullptr) {
    pool = new Node[NODES];
    nil = my_new(Monoid::id());
    nil->l = nil->r = nil;
    roots.reserve(262144);
    roots.push_back(nil);
  }

  Node *my_new(const T &data) {
    pool[pid].data = data;
    pool[pid].l = pool[pid].r = nil;
    return &(pool[pid++]);
  }

  Node *merge(Node *l, Node *r) {
    pool[pid].data = Monoid::op(l->data, r->data);
    pool[pid].l = l;
    pool[pid].r = r;
    return &(pool[pid++]);
  }

  Node *build(const std::vector<T> &v) {
    N = (i64)v.size();
    return build(0, (i64)v.size(), v);
  }

  Node *build(i64 l, i64 r, const std::vector<T> &v) {
    if (l + 1 == r) return my_new(v[l]);
    i64 m = (l + r) >> 1;
    return merge(build(l, m, v), build(m, r, v));
  }

  Node *set(Node *n, i64 k, const T &x) {
    Node *root = set_(k, x, n, 0, N);
    roots.push_back(root);
    return root;
  }

  T fold(Node *n, i64 a, i64 b) { return fold_(a, b, n, 0, N); }
  T get(Node *n, i64 k) { return fold_(k, k + 1, n, 0, N); }

  Node *new_tree() { return nil; }

 private:
  Node *set_(i64 a, const T &x, Node *n, i64 l, i64 r) {
    if (l + 1 == r) return my_new(x);
    i64 m = (l + r) >> 1;
    if (a < m) return merge(set_(a, x, n->l, l, m), n->r);
    return merge(n->l, set_(a, x, n->r, m, r));
  }

  T fold_(i64 a, i64 b, Node *n, i64 l, i64 r) {
    if (n == nil) return Monoid::id();
    if (r <= a or b <= l) return Monoid::id();
    if (a <= l and r <= b) return n->data;
    i64 m = (l + r) >> 1;
    return Monoid::op(fold_(a, b, n->l, l, m), fold_(a, b, n->r, m, r));
  }
};
