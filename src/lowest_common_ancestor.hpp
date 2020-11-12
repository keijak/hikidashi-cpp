#include <utility>
#include <vector>
using namespace std;

// Lowest Common Ancestor by Doubling.
// - query: O(log N)
// - build: O(N log N)
// - space: O(N log N)
struct DoublingLCA {
  const int n;  // number of nodes
  const vector<vector<int>> &adj;
  vector<vector<int>> upper;  // 2^k upper node
  vector<int> depth;

  explicit DoublingLCA(const Graph &g, int root = 0)
      : n(g.n), adj(g.adj), upper(K, vector<int>(g.n, -1)), depth(g.n) {
    depth[root] = 0;
    dfs(0, -1);
    for (int k = 0; k + 1 < K; k++) {
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
    for (int k = 0; k < K; k++) {
      if ((depth[v] - depth[u]) & (1 << k)) {
        v = upper[k][v];
      }
    }
    if (u == v) return u;
    for (int k = K - 1; k >= 0; k--) {
      if (upper[k][u] != upper[k][v]) {
        u = upper[k][u];
        v = upper[k][v];
      }
    }
    return upper[0][u];
  }

  int parent(int v) const { return upper[0][v]; }

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

  static const int K = 30;  // max upper lookup (2^K)
};

// Lowest Common Ancestor by EulerTour + RMQ (SparseTable).
// - query: O(1)
// - build: O(N log N)
// - space: O(N log N)
struct EulerTour {
  const int n;  // number of nodes
  const vector<vector<int>> &adj;
  vector<int> depth;
  vector<int> index;
  // Euler Tour on nodes.
  vector<pair<int, int>> tour;  // (depth, node id)

  explicit EulerTour(const Graph &g, int root = 0)
      : n(g.n), adj(g.adj), depth(g.n, 0), index(g.n, -1), tour() {
    tour.reserve(n * 2);
    dfs(root, -1);
  }

 private:
  void dfs(int v, int p) {
    index[v] = int(tour.size());
    depth[v] = (p < 0) ? 0 : (depth[p] + 1);
    tour.emplace_back(depth[v], v);
    for (auto u : adj[v]) {
      if (u == p) continue;
      dfs(u, v);
      tour.emplace_back(v);
    }
  }
};

int euler_tour_lca_example(const Graph &g, int u, int v) {
  EulerTour et(g);
  SparseTable<Min> st(et.tour);

  // Usage
  int ui = et.index[u], vi = et.index[v];
  auto [lca_depth, lca_id] = st.fold(min(ui, vi), max(ui, vi) + 1);
  return lca_id;
}
