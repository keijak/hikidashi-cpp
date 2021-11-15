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

// Lowest Common Ancestor by binary lifting.
// - query: O(log N)
// - build: O(N log N)
// - space: O(N log N)
struct AncestorBinaryLifting {
  using G = vector<vector<int>>;

  const int n;         // number of nodes
  const int max_bits;  // max upper lookup (2^max_bits)
  const int root;      // root node
  const G& adj;
  vector<vector<int>> upper;  // 2^k upper node
  vector<int> depth;

  explicit AncestorBinaryLifting(const G& g, int root = 0)
      : n(g.size()),
        max_bits(power2_ceil(n + 1)),
        root(root),
        adj(g),
        upper(max_bits, vector<int>(n)),
        depth(n, 0) {
    // Build `depth` and `upper[0]`.
    // The root node's parent is itself.
    dfs(root, root);

    // Build `upper[k]` for k > 0.
    for (int k = 0; k + 1 < max_bits; ++k) {
      for (int v = 0; v < n; ++v) {
        upper[k + 1][v] = upper[k][upper[k][v]];
      }
    }
  }

  // Returns the parent of v.
  // The root's parent is the root itself.
  int parent(int v) const { return upper[0][v]; }

  // Returns i-th ancestor of v.
  // - 0th ancestor is v itself.
  // - 1st ancestor is the parent.
  // - (2^k)-th ancestor is `upper[k][v]`.
  int ancestor(int v, int i) const {
    int x = v;
    for (int k = max_bits - 1; k >= 0; --k) {
      if (i & (1 << k)) {
        x = upper[k][x];
      }
    }
    return x;
  }

  // Returns the node ID of the lowest common ancestor.
  int lca(int u, int v) const {
    if (depth[u] > depth[v]) swap(u, v);

    // Move up `v` so both nodes have the same depth.
    int ddiff = depth[v] - depth[u];
    v = ancestor(v, ddiff);
    if (u == v) return u;

    // Move up both nodes but still keep them below the LCA.
    for (int k = max_bits - 1; k >= 0; --k) {
      if (upper[k][u] != upper[k][v]) {
        u = upper[k][u];
        v = upper[k][v];
      }
    }
    // Now both nodes are direct children of the LCA.
    const int pu = parent(u), pv = parent(v);
    assert(pu == pv);
    return pu;
  }

  // Returns the distance (number of edges) between two nodes.
  int distance(int u, int v) const {
    int p = lca(u, v);
    return (depth[u] - depth[p]) + (depth[v] - depth[p]);
  }

  // Binary search.
  // Returns the minimum number of steps to reach an ancestor
  // that satisfies `pred(a)`.
  template <class F>
  optional<int> min_steps(int start, F pred) const {
    static_assert(std::is_invocable_r_v<bool, F, int>);
    if (pred(start)) return 0;
    if (not pred(root)) return nullopt;
    int max_false = 0;
    int v = start;
    for (int d = max_bits - 1; d >= 0; --d) {
      auto u = upper[d][v];
      if (pred(u)) continue;
      max_false += 1 << d;
      v = u;
    }
    int min_true = max_false + 1;
    return min_true;
  }

 private:
  static int power2_ceil(int n) {
    int b = 1;
    while ((1u << b) < (unsigned)n) ++b;
    return b;
  }

  void dfs(int v, int p) {
    upper[0][v] = p;  // parent
    for (auto u : adj[v]) {
      if (u == p) continue;
      depth[u] = depth[v] + 1;
      dfs(u, v);
    }
  }
};
