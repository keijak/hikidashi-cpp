#include <bits/stdc++.h>

template <int alphabet_size = 26, int alphabet_base = 'a', int ROOT_ID = 0>
struct Trie {
  struct Node {
    int ch;                                // character
    std::vector<std::optional<int>> next;  // child node indices
    std::vector<int> entries;  // entry ids stored at the terminal node
    int prefix_count;          // how many entries are stored below this node.
    Node(int c) : ch(c), next(alphabet_size), prefix_count(0) {}
  };
  std::vector<Node> nodes;

  Trie() : nodes(1, Node{ROOT_ID}) {}

  const Node *insert(std::string_view entry, int entry_id) {
    int node_id = ROOT_ID;
    for (char ch : entry) {
      ++(nodes[node_id].prefix_count);
      int c = int(ch) - alphabet_base;
      auto &next_id = nodes[node_id].next[c];
      if (not next_id.has_value()) {
        next_id = (int)nodes.size();
        nodes.emplace_back(c);
      }
      node_id = next_id.value();
    }
    Node *node = &nodes[node_id];
    ++(node->prefix_count);
    node->entries.push_back(entry_id);
    return node;
  }
  const Node *insert(std::string_view entry) {
    return insert(entry, nodes[ROOT_ID].prefix_count);
  }

  const Node *search(std::string_view entry) const {
    int node_id = ROOT_ID;
    for (char ch : entry) {
      int c = int(ch) - alphabet_base;
      const auto &next_id = nodes[node_id].next[c];
      if (not next_id.has_value()) return nullptr;
      node_id = next_id.value();
    }
    return &nodes[node_id];
  }

  bool contains(std::string_view entry) const {
    auto res = search(entry);
    if (res == nullptr) return false;
    return not res->entries.empty();
  }

  bool contains_prefix(std::string_view entry) const {
    return search(entry) != nullptr;
  }
};
