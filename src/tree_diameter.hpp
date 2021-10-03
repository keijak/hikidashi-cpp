#include <algorithm>
#include <cassert>
#include <vector>

struct Diameter {
  int distance;
  std::vector<int> path;
};

// Tree diameter.
// Returns the maximum diameter and two endpoint nodes of the diameter.
Diameter tree_diameter(const std::vector<std::vector<int>> &g) {
  std::vector<int> parent(g.size());

  auto farthest = [&](auto &rec, int v, int p, int d) -> std::pair<int, int> {
    parent[v] = p;
    int rd = d, rv = v;
    for (auto u : g[v]) {
      if (u == p) continue;
      const auto &[child_d, child_v] = rec(rec, u, v, d + 1);
      if (child_d > rd) {
        rd = child_d;
        rv = child_v;
      }
    }
    return {rd, rv};
  };

  const auto &[_, vr] = farthest(farthest, 0, -1, 0);
  const auto &[distance, vl] = farthest(farthest, vr, -1, 0);

  // Restore the path from vl to vr.
  std::vector<int> diameter_path;
  for (int v = vl; v != -1; v = parent[v]) {
    diameter_path.push_back(v);
  }
  assert(diameter_path.back() == vr);
  assert(int(diameter_path.size()) == distance + 1);

  return Diameter{distance, std::move(diameter_path)};
}
