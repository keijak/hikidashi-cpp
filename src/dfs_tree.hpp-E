#include <bits/stdc++.h>
using namespace std;

template <bool reshape_graph = false>
struct DFSTree {
  int n_;
  int root_;
  std::vector<int> parent_;
  std::vector<int> depth_;

  // if reshape_graph=true, `g_` is mutated into a tree.
  std::vector<std::vector<int>>& g_;

  // edge(u,v) where v is an ancestor of u in the DFS tree.
  std::set<std::pair<int, int>> back_edges_;

  explicit DFSTree(std::vector<std::vector<int>>& g, int root = 0)
      : n_(int(g.size())), root_(root), parent_(n_, -1), depth_(n_, -1), g_(g) {
    dfs(root_, -1, 0);
  }

  bool is_connected(int u, int v) const {
    if (depth_[u] < depth_[v]) std::swap(u, v);
    return (parent_[u] == v) or back_edges_.count({u, v});
  }

 private:
  void dfs(int v, int p, int d) {
    parent_[v] = p;
    depth_[v] = d;
    std::set<int> non_neighbors;
    for (auto u : g_[v]) {
      if (u == p) {  // parent
        continue;
      }
      if (depth_[u] == -1) {  // child
        dfs(u, v, d + 1);
        continue;
      }
      if constexpr (reshape_graph) {
        non_neighbors.insert(u);
      }
      if (depth_[u] < d) {  // backward
        back_edges_.emplace(v, u);
      } else {
        // forward
      }
    }
    if constexpr (reshape_graph) {
      auto& adj = g_[v];
      adj.erase(std::remove_if(adj.begin(), adj.end(),
                               [&](int u) { return non_neighbors.count(u); }),
                adj.end());
    }
  }
};
