#include <bits/stdc++.h>

template <typename T = unsigned, int kBitWidth = std::numeric_limits<T>::digits>
struct BinaryTrie {
  static_assert(std::is_unsigned<T>::value, "Requires unsigned type");

 public:
  struct Node;
  using NodePtr = Node *;

  struct Node {
    int leaf_count;
    std::array<NodePtr, 2> child;
    mutable T lazy_mask;
    Node() : leaf_count(0), child{nullptr, nullptr}, lazy_mask(0) {}
  };
  NodePtr root_;

  BinaryTrie() = default;
  explicit BinaryTrie(NodePtr p) : root_(p) {}

  int size() const { return size(root_); }

  bool empty() const { return size() == 0; }

  // Applies an XOR-mask to all elements.
  void xor_all(T xor_mask) {
    if (root_) root_->lazy_mask ^= xor_mask;
  }

  // Converts the multiset to a vector.
  std::vector<T> to_vec() const {
    std::vector<T> res;
    res.reserve(size());
    to_vec_internal(root_, T(0), res);
    return res;
  }

  void insert(T val) { root_ = insert_internal(root_, val); }

  // Removes one element of `val`.
  // At least one `val` must exist in the trie. Check `trie.count(val) > 0`.
  void erase_one(T val) { root_ = erase_internal(root_, val); }

  // Returns the element x in the trie that maximizes `x ^ xor_mask`.
  T max_element(T xor_mask = 0) const { return get_min(root_, ~xor_mask); }

  // Returns the element x in the trie that minimizes `x ^ xor_mask`.
  T min_element(T xor_mask = 0) const { return get_min(root_, xor_mask); }

  // Returns the minimum index i s.t. trie[i] >= val.
  int lower_bound(T val) const { return count_less(root_, val); }

  // Returns the minimum index i s.t. trie[i] > val.
  int upper_bound(T val) const { return count_less(root_, val + 1); }

  // Returns k-th (0-indexed) smallest value.
  T operator[](int k) const {
    assert(0 <= k and k < size());
    return get_internal(root_, k);
  }

  // Returns k-th (0-indexed) largest value.
  T kth_largest(int k) const {
    const int i = size() - k - 1;
    return (*this)[i];
  }

  // Counts the number of elements that are equal to `val`.
  // Note: BinaryTrie is a multiset.
  int count(T val) const {
    if (!root_) return 0;
    NodePtr t = root_;
    for (int i = kBitWidth - 1; i >= 0; i--) {
      t = t->child[(val >> T(i)) & 1];
      if (!t) return 0;
    }
    return t->leaf_count;
  }

  // Splits the trie into two: k smallest items and (n-k) largest items.
  std::pair<BinaryTrie, BinaryTrie> split(int k) {
    auto [t1, t2] = split_internal(root_, k);
    return {BinaryTrie(t1), BinaryTrie(t2)};
  }

  // Merges two tries into this. Destructs the other.
  void merge(BinaryTrie other) { root_ = merge_internal(root_, other.root_); }

 private:
  static NodePtr make_node(int leaf_count, NodePtr l = nullptr,
                           NodePtr r = nullptr) {
    static std::deque<Node> pool;
    pool.emplace_back(leaf_count, l, r);
    return &pool.back();
  }

  static void push_down(NodePtr t, int b) {
    if (t->lazy_mask == 0) return;
    if ((t->lazy_mask >> b) & 1) std::swap(t->child[0], t->child[1]);
    if (t->child[0]) t->child[0]->lazy_mask ^= t->lazy_mask;
    if (t->child[1]) t->child[1]->lazy_mask ^= t->lazy_mask;
    t->lazy_mask = 0;
  }

  static int size(NodePtr t) { return (t == nullptr) ? 0 : t->leaf_count; }

  static NodePtr insert_internal(NodePtr t, T val, int b = kBitWidth - 1) {
    if (not t) t = make_node(0);
    t->leaf_count += 1;
    if (b < 0) return t;
    push_down(t, b);
    bool f = (val >> b) & 1;
    t->child[f] = insert_internal(t->child[f], val, b - 1);
    return t;
  }

  static NodePtr erase_internal(NodePtr t, T val, int b = kBitWidth - 1) {
    assert(t);
    t->leaf_count -= 1;
    if (t->leaf_count == 0) return nullptr;
    if (b < 0) return t;
    push_down(t, b);
    bool f = (val >> b) & 1;
    t->child[f] = erase_internal(t->child[f], val, b - 1);
    return t;
  }

  static T get_min(NodePtr t, T xor_mask, int b = kBitWidth - 1) {
    assert(t);
    if (b < 0) return 0;
    push_down(t, b);
    bool f = (xor_mask >> b) & 1;
    f ^= not t->child[f];
    return get_min(t->child[f], xor_mask, b - 1) | (T(f) << b);
  }

  static T get_internal(NodePtr t, int k, int b = kBitWidth - 1) {
    if (b < 0) return 0;
    push_down(t, b);
    int m = size(t->child[0]);
    return k < m ? get_internal(t->child[0], k, b - 1)
                 : get_internal(t->child[1], k - m, b - 1) | (T(1) << b);
  }

  static int count_less(NodePtr t, T val, int b = kBitWidth - 1) {
    if (not t or b < 0) return 0;
    push_down(t, b);
    bool f = (val >> b) & 1;
    return (f ? size(t->child[0]) : 0) + count_less(t->child[f], val, b - 1);
  }

  static void to_vec_internal(NodePtr t, T val, std::vector<T> &out,
                              int b = kBitWidth - 1) {
    if (not t) return;
    if (b < 0) {
      out.push_back(val);
      return;
    }
    push_down(t, b);
    if (t->child[0]) {
      to_vec_internal(t->child[0], val, out, b - 1);
    }
    if (t->child[1]) {
      to_vec_internal(t->child[1], val | (T(1) << b), out, b - 1);
    }
  }

  static std::pair<NodePtr, NodePtr> split_internal(NodePtr t, int pos,
                                                    int b = kBitWidth - 1) {
    if (not t) return {nullptr, nullptr};
    if (b < 0) {
      if (pos <= 0)
        return {nullptr, t};
      else
        return {t, nullptr};
    }
    push_down(t, b);
    const int lsize = size(t->child[0]);
    if (lsize <= pos) {
      auto [lz, rz] = split_internal(t->child[1], pos - lsize, b - 1);
      return {make_node(size(t->child[0]) + size(lz), t->child[0], lz),
              make_node(size(rz), nullptr, rz)};
    } else {
      auto [lz, rz] = split_internal(t->child[0], pos, b - 1);
      return {make_node(size(lz), lz, nullptr),
              make_node(size(rz) + size(t->child[1]), rz, t->child[1])};
    }
  }

  static NodePtr merge_internal(NodePtr t1, NodePtr t2, int b = kBitWidth - 1) {
    if (not t2) return t1;
    if (not t1) return t2;
    push_down(t1, b);
    push_down(t2, b);
    t1->leaf_count = size(t1) + size(t2);
    t1->child[0] = merge_internal(t1->child[0], t2->child[0], b - 1);
    t1->child[1] = merge_internal(t1->child[1], t2->child[1], b - 1);
    return t1;
  }
};
using Trie = BinaryTrie<>;
