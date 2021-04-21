// Doubling-based LCA.
//
// Note: Other LCA algorithms:
// - euler_tour_technique.hpp
// - heavy_light_decomposition.hpp

#include <utility>
#include <vector>
using namespace std;

// Lowest Common Ancestor by Doubling.
// - query: O(log N)
// - build: O(N log N)
// - space: O(N log N)
struct DoublingAncestor {
  using G = vector<vector<int>>;
  static const int K = 30;  // max upper lookup (2^K)

  const int n;  // number of nodes
  G adj;
  vector<vector<int>> upper;  // 2^k upper node
  vector<int> depth;

  explicit DoublingAncestor(G g, int root = 0)
      : n(g.size()), adj(move(g)), upper(K, vector<int>(n, -1)), depth(n) {
    depth[root] = 0;

    // Build `depth` and `upper[0]`.
    dfs(0, -1);

    // Build `upper[k]` for k > 0.
    for (int k = 0; k + 1 < K; ++k) {
      for (int v = 0; v < n; ++v) {
        if (upper[k][v] < 0) {
          upper[k + 1][v] = -1;
        } else {
          upper[k + 1][v] = upper[k][upper[k][v]];
        }
      }
    }
  }

  // Returns the node ID of the lowest common ancestor.
  int lca(int u, int v) const {
    if (depth[u] > depth[v]) swap(u, v);
    int ddiff = depth[v] - depth[u];

    // Move up `v` so both nodes have the same depth.
    for (int k = K - 1; k >= 0; --k) {
      if (ddiff & (1 << k)) {
        v = upper[k][v];
      }
    }
    if (u == v) return u;

    // Move up both nodes but still keep them below the LCA.
    for (int k = K - 1; k >= 0; --k) {
      if (upper[k][u] != upper[k][v]) {
        u = upper[k][u];
        v = upper[k][v];
      }
    }

    // Now both nodes are direct children of the LCA.
    return parent(u);
  }

  int parent(int v) const { return upper[0][v]; }

  // k-th ancestor of v.
  // - The parent is the 1st ancestor.
  // - The parent's parent is the 2nd ancestor.
  int ancestor(int v, int k) const {
    for (int i = K - 1; i >= 0; --i) {
      if (k & (1 << i)) {
        v = upper[i][v];
      }
    }
    return v;
  }

  // Returns the distance (number of edges) between two nodes.
  int distance(int u, int v) const {
    int p = lca(u, v);
    return (depth[u] - depth[p]) + (depth[v] - depth[p]);
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
