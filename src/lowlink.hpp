#include <bits/stdc++.h>

// LowLink
// Computes bridges and articulation points of a graph in O(V+E).
struct LowLink {
  using G = std::vector<std::vector<int>>;

  int n;
  G adj;
  std::vector<int> ord, low;
  std::vector<int> aps;  // articulation points
  std::vector<std::pair<int, int>> bridges;

  explicit LowLink(G g) : n(g.size()), adj(move(g)), ord(n, -1), low(n) {
    init();
  }

 private:
  void init() {
    int ord_counter = 0;

    auto dfs = [&](auto self, int v, int par = -1) -> void {
      ord[v] = ord_counter++;
      low[v] = ord[v];
      bool is_articulation_point = false;
      for (auto u : adj[v]) {
        if (ord[u] < 0) {
          self(self, u, v);
          low[v] = std::min(low[v], low[u]);
          if (par >= 0 and ord[v] <= low[u]) {
            is_articulation_point = true;
          }
          if (ord[v] < low[u]) {
            bridges.emplace_back(std::min(u, v), std::max(u, v));
          }
        } else if (u != par) {  // backward edge
          low[v] = std::min(low[v], ord[u]);
        }
      }
      // Is the root node an articulatin point?
      if (par < 0 and adj[v].size() >= 2) {
        is_articulation_point = true;
      }
      if (is_articulation_point) aps.push_back(v);
    };

    for (int i = 0; i < n; ++i) {
      if (ord[i] == -1) dfs(dfs, i);
    }

    // sort(aps.begin(), aps.end());
    // sort(bridges.begin(), bridges.end());
  }
};
