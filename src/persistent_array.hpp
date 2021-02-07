
template <typename T, int K = 20>
struct PersistentArray {
  struct Node;
  // No memory release by default.
  using NodePtr = Node *;  // ALT: std::shared_ptr<Node>

  struct Node {
    std::optional<T> val;
    std::array<NodePtr, K> ch;
    Node() : val{}, ch{} {}
  };

  explicit PersistentArray(NodePtr root) : root_{std::move(root)} {}
  PersistentArray() : root_{} {}
  PersistentArray(const PersistentArray &) = default;
  PersistentArray(PersistentArray &&) = default;
  PersistentArray &operator=(const PersistentArray &) = default;
  PersistentArray &operator=(PersistentArray &&) = default;

  std::optional<T> operator[](int idx) const { return get_(idx, root_); }

  PersistentArray<T, K> set(int idx, T val) const {
    return PersistentArray<T, K>(set_(idx, val, root_));
  }

 private:
  static std::optional<T> get_(int idx, const NodePtr &node) {
    if (node == nullptr) return std::nullopt;
    if (idx == 0) return node->val;
    const NodePtr &child = node->ch[idx % K];
    if (child == nullptr) return std::nullopt;
    return get_(idx / K, child);
  }

  static NodePtr set_(int idx, T val, const NodePtr &node) {
    NodePtr res{new Node()};
    if (node != nullptr) {
      res->val = node->val;
      std::copy(node->ch.begin(), node->ch.end(), res->ch.begin());
    }
    if (idx == 0) {
      res->val = std::move(val);
    } else {
      res->ch[idx % K] = set_(idx / K, std::move(val), res->ch[idx % K]);
    }
    return res;
  }

  NodePtr root_;
};
