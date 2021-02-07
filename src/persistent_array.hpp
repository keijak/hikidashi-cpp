
template <typename T, int K = 20>
class PersistentArray {
  struct Node;
  // No memory release by default.
  using NodePtr = Node *;  // std::shared_ptr<Node>;

  struct Node {
    std::optional<T> val;
    std::unique_ptr<std::array<NodePtr, K>> children;
  };

  explicit PersistentArray(NodePtr root) : root_{std::move(root)} {}
  PersistentArray() : root_{} {}
  PersistentArray(const PersistentArray &) = default;
  PersistentArray(PersistentArray &&) = default;
  PersistentArray &operator=(const PersistentArray &) = default;
  PersistentArray &operator=(PersistentArray &&) = default;

  std::optional<T> operator[](int idx) const { return do_get(idx, root_); }

  PersistentArray<T, K> set(int idx, T val) const {
    return PersistentArray<T, K>(do_set(idx, val, root_));
  }

 private:
  static std::optional<T> do_get(int idx, const NodePtr &node) {
    if (node == nullptr) return std::nullopt;
    if (idx == 0) return node->val;
    if (node->children == nullptr) return std::nullopt;
    const NodePtr &child = (*node->children)[idx % K];
    if (child == nullptr) return std::nullopt;
    return do_get(idx / K, child);
  }

  static NodePtr do_set(int idx, T val, const NodePtr &node) {
    NodePtr res{new Node()};
    if (node != nullptr) {
      res->val = node->val;
      if (node->children != nullptr) {
        res->children.reset(new std::array<NodePtr, K>(*node->children));
      }
    }
    if (idx == 0) {
      res->val = std::move(val);
    } else {
      if (res->children == nullptr) {
        res->children.reset(new std::array<NodePtr, K>());
      }
      (*res->children)[idx % K] =
          do_set(idx / K, std::move(val), (*res->children)[idx % K]);
    }
    return res;
  }

 private:
  NodePtr root_;
};
