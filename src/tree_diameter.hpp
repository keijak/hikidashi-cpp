#include <vector>

// Tree diameter.
// Returns the maximum diameter and two end point nodes with the diameter.
std::pair<int, std::pair<int, int>> tree_diameter(
    const std::vector<std::vector<int>>& g) {
  auto farthest = [&](auto& rec, int v, int p, int d) -> std::pair<int, int> {
    int res_d = d, res_v = v;
    for (auto u : g[v]) {
      if (u == p) continue;
      const auto& [child_d, child_v] = rec(rec, u, v, d + 1);
      if (child_d > res_d) {
        res_d = child_d;
        res_v = child_v;
      }
    }
    return {res_d, res_v};
  };

  const auto& [_, u] = farthest(farthest, 0, -1, 0);
  const auto& [distance, v] = farthest(farthest, u, -1, 0);
  return std::pair{distance, std::pair{u, v}};
}
