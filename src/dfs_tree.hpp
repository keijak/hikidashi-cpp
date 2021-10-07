#include <bits/stdc++.h>
using namespace std;

struct DFSTree {
  int n_;
  int root_;
  vector<vector<int>> g_;  // a spanning tree of the original graph.
  vector<int> parent_;
  vector<int> depth_;

  // edge(u,v) where v is an ancestor of u in the DFS tree.
  set<pair<int, int>> back_edges_;

  explicit DFSTree(vector<vector<int>> g, int root = 0)
      : n_((int)g.size()), root_(root), g_(g), parent_(n_, -1), depth_(n_, -1) {
    dfs(root_, -1, 0);
    for (int v = 0; v < n_; ++v) {
      std::sort(g[v].begin(), g[v].end());
    }
  }

  bool is_connected(int u, int v) const {
    if (depth_[u] < depth_[v]) std::swap(u, v);
    return (parent_[u] == v) or back_edges_.count({u, v});
  }

 private:
  void dfs(int v, int p, int d) {
    parent_[v] = p;
    depth_[v] = d;
    set<int> non_neighbors;
    for (auto u : g_[v]) {
      if (u == p) continue;
      if (depth_[u] == -1) {
        dfs(u, v, d + 1);
      }
      non_neighbors.insert(u);
      if (depth_[u] < d) {
        // backward
        back_edges_.emplace(v, u);
      } else {
        // forward
      }
    }
    g_[v].erase(std::remove_if(g_[v].begin(), g_[v].end(),
                               [&](int u) { return non_neighbors.count(u); }),
                g_[v].end());
  }
};
