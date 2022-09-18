#include <bits/stdc++.h>

template <int ROOT_ID = 0>
struct AhoCorasick {
  struct Node {
    char ch;
    std::map<char, int> next;
    std::vector<int> entries;
    int prefix_count;
    int suffix_count;
    int failure_id;

    explicit Node(char c)
        : ch(c), prefix_count(0), suffix_count(0), failure_id(ROOT_ID) {}
  };
  std::vector<Node> nodes;

  AhoCorasick() : nodes(1, Node{ROOT_ID}) {}

  const Node *insert(std::string_view entry, int entry_id) {
    int node_id = ROOT_ID;
    for (char ch : entry) {
      ++(nodes[node_id].prefix_count);
      auto it = nodes[node_id].next.find(ch);
      if (it == nodes[node_id].next.end()) {
        int next_id = (int)nodes.size();
        nodes[node_id].next[ch] = next_id;
        nodes.emplace_back(ch);
        node_id = next_id;
      } else {
        node_id = it->second;
      }
    }
    Node *node = &nodes[node_id];
    node->prefix_count += 1;
    node->suffix_count += 1;
    node->entries.push_back(entry_id);
    return node;
  }
  const Node *insert(std::string_view entry) {
    return insert(entry, nodes[ROOT_ID].prefix_count);
  }

  void build_failure_links() {
    std::queue<pair<int, int>> q;
    for (auto [ch, next_id] : nodes[ROOT_ID].next) {
      q.emplace(next_id, ROOT_ID);
    }
    while (not q.empty()) {
      int node_id, parent_id;
      std::tie(node_id, parent_id) = q.front();
      q.pop();
      if (parent_id != ROOT_ID) {
        const char ch = nodes[node_id].ch;
        parent_id = nodes[parent_id].failure_id;
        while (true) {
          auto it = nodes[parent_id].next.find(ch);
          if (it != nodes[parent_id].next.end()) {
            nodes[node_id].failure_id = it->second;
            nodes[node_id].suffix_count += nodes[it->second].suffix_count;
            break;
          }
          if (parent_id == ROOT_ID) break;
          parent_id = nodes[parent_id].failure_id;
        }
      }
      for (const auto [ch, next_id] : nodes[node_id].next) {
        q.emplace(next_id, node_id);
      }
    }
  }

  // Returns all entry_ids found in the text and their end positions.
  std::vector<std::pair<int, int>> scan(std::string_view text) {
    std::vector<std::pair<int, int>> found;
    int node_id = ROOT_ID;
    for (int i = 0; i < (int)text.size(); ++i) {
      char ch = text[i];
      for (;;) {
        auto &node = nodes[node_id];
        auto it = node.next.find(ch);
        if (it != node.next.end()) {
          node_id = it->second;
          break;
        }
        if (node_id == 0) break;
        node_id = node.failure_id;
      }
      for (int it = node_id; it != ROOT_ID; it = nodes[it].failure_id) {
        auto &node = nodes[it];
        if (node.suffix_count == 0) break;
        for (int entry_id : node.entries) {
          found.emplace_back(entry_id, i + 1);
        }
      }
    }
    return found;
  }
};
