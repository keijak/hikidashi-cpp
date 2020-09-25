// Tree diameter.
// Returns the maximum diameter and two end point nodes with the diameter.
pair<int, pair<int, int>> tree_diameter(const V<V<int>>& adj) {
  auto dfs = [&](auto self, int v, int p, int d) -> pair<int, int> {
    int res_d = d, res_v = v;
    for (auto u : adj[v]) {
      if (u != p) {
        auto [child_d, child_v] = self(self, u, v, d + 1);
        if (child_d > res_d) {
          res_d = child_d;
          res_v = child_v;
        }
      }
    }
    return {res_d, res_v};
  };

  auto [d1, u] = dfs(dfs, 0, -1, 0);
  auto [d2, v] = dfs(dfs, u, -1, 0);
  return {d2, make_pair(u, v)};
}
