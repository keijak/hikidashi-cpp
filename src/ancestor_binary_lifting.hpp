// Doubling-based LCA.
//
// Note: Other LCA algorithms:
// - euler_tour_technique.hpp
// - heavy_light_decomposition.hpp

#include <cassert>
#include <functional>
#include <optional>
#include <type_traits>
#include <utility>
#include <vector>
using namespace std;

// Lowest Common Ancestor by Doubling.
// - query: O(log N)
// - build: O(N log N)
// - space: O(N log N)
struct AncestorBinaryLifting {
  using G = vector<vector<int>>;
  static const int kMaxBits = 30;  // max upper lookup (2^kMaxBits)

  const int n;  // number of nodes
  G adj;
  vector<vector<optional<int>>> upper;  // 2^k upper node
  vector<int> depth;

  explicit AncestorBinaryLifting(G g, int root = 0)
      : n(g.size()),
        adj(move(g)),
        upper(kMaxBits, vector<optional<int>>(n)),
        depth(n) {
    depth[root] = 0;

    // Build `depth` and `upper[0]`.
    dfs(0, -1);

    // Build `upper[k]` for k > 0.
    for (int k = 0; k + 1 < kMaxBits; ++k) {
      for (int v = 0; v < n; ++v) {
        if (upper[k][v].has_value()) {
          upper[k + 1][v] = upper[k][upper[k][v].value()];
        }
      }
    }
  }

  optional<int> parent(int v) const { return upper[0][v]; }

  // Returns i-th ancestor of v.
  // - parent is the 1st ancestor.
  // - `upper[k][v]`is the (2^k)-th ancestor.
  optional<int> ancestor(int v, int i) const {
    assert(i < (1 << kMaxBits));
    int x = v;
    for (int k = kMaxBits - 1; k >= 0; --k) {
      if (i & (1 << k)) {
        if (not upper[k][x].has_value()) return nullopt;
        x = upper[k][x].value();
      }
    }
    return x;
  }

  // Returns the node ID of the lowest common ancestor.
  int lca(int u, int v) const {
    if (depth[u] > depth[v]) swap(u, v);
    int ddiff = depth[v] - depth[u];

    // Move up `v` so both nodes have the same depth.
    for (int k = kMaxBits - 1; k >= 0; --k) {
      if (ddiff & (1 << k)) {
        v = upper[k][v].value();
      }
    }
    if (u == v) return u;

    // Move up both nodes but still keep them below the LCA.
    for (int k = kMaxBits - 1; k >= 0; --k) {
      if (upper[k][u] != upper[k][v]) {
        u = upper[k][u].value();
        v = upper[k][v].value();
      }
    }

    // Now both nodes are direct children of the LCA.
    return parent(u).value();
  }

  // Returns the distance (number of edges) between two nodes.
  int distance(int u, int v) const {
    int p = lca(u, v);
    return (depth[u] - depth[p]) + (depth[v] - depth[p]);
  }

  // Returns the minimum number of steps to reach an ancestor
  // that satisfies `pred(a)`.
  template <class F>
  int min_steps(int start, F pred) const {
    static_assert(std::is_invocable_r_v<bool, F, int>);
    int max_false = 0;
    int v = start;
    for (int d = kMaxBits - 1; d >= 0; --d) {
      auto u = upper[d][v];
      if (not u.has_value()) continue;
      if (pred(u.value())) continue;
      max_false += 1 << d;
      v = u.value();
    }
    return max_false + 1;
  }

 private:
  void dfs(int v, int p) {
    upper[0][v] = p;  // parent
    for (auto u : adj[v]) {
      if (u == p) continue;
      depth[u] = depth[v] + 1;
      dfs(u, v);
    }
  }
};
