#include <bits/stdc++.h>

template <int alphabet_size = 26, int alphabet_base = 'a'>
struct Trie {
  struct Node {
    int c;                                 // character
    std::vector<std::optional<int>> next;  // child node indices
    std::vector<int> entries;  // entry ids stored at the terminal node
    int entry_count;           // how many entries are stored below this node.
    Node(int c_) : c(c_), next(alphabet_size), entry_count(0) {}
  };
  static int ROOT_ID = 0;
  std::vector<Node> nodes;

  Trie() : nodes(1, Node{ROOT_ID}) {}

  void insert(std::string_view entry, int entry_id) {
    int node_id = ROOT_ID;
    for (char ch : entry) {
      ++(nodes[node_id].entry_count);
      int c = int(ch) - alphabet_base;
      auto &next_id = nodes[node_id].next[c];
      if (not next_id.has_value()) {
        next_id = (int)nodes.size();
        nodes.emplace_back(c);
      }
      node_id = next_id.value();
    }
    ++(nodes[node_id].entry_count);
    nodes[node_id].entries.push_back(entry_id);
  }
  void insert(std::string_view entry) {
    insert(entry, nodes[ROOT_ID].entry_count);
  }

  std::std::optional<const Node *> search(std::string_view entry) const {
    int node_id = ROOT_ID;
    for (char ch : entry) {
      int c = int(ch) - alphabet_base;
      const auto &next_id = nodes[node_id].next[c];
      if (not next_id.has_value()) return nullopt;
      node_id = next_id.value();
    }
    return &nodes[node_id];
  }

  bool contains(std::string_view entry) const {
    auto res = search(entry);
    if (not res.has_value()) return false;
    return not res.value()->entries.empty();
  }

  bool contains_prefix(std::string_view entry) const {
    return search(entry).has_value();
  }
};
