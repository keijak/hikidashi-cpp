#include <utility>
#include <vector>
using namespace std;

// Lowest Common Ancestor by Doubling.
// - query: O(log N)
// - build: O(N log N)
// - space: O(N log N)
struct DoublingLCA {
  using G = vector<vector<int>>;

  const int n;  // number of nodes
  G adj;
  vector<vector<int>> upper;  // 2^k upper node
  vector<int> depth;

  explicit DoublingLCA(G g, int root = 0)
      : n(g.size()), adj(move(g)), upper(K, vector<int>(n, -1)), depth(n) {
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
  using G = vector<vector<int>>;

  const int n;  // number of nodes
  G adj;

  // Euler Tour on nodes.
  vector<pair<int, int>> tour;  // (depth, node id)
  vector<int> depth;
  vector<int> index;  // index in the tour on entering the subtree of v.

  explicit EulerTour(G g, int root = 0)
      : n(g.size()), adj(move(g)), depth(n, 0), index(n, -1) {
    tour.reserve(n * 2);
    depth[root] = 0;
    dfs(root, -1);
  }

 private:
  void dfs(int v, int p) {
    index[v] = int(tour.size());
    if (p >= 0) depth[v] = depth[p] + 1;
    tour.emplace_back(depth[v], v);
    for (auto u : adj[v]) {
      if (u == p) continue;
      dfs(u, v);
      tour.emplace_back(depth[v], v);
    }
  }
};

struct Min {
  using T = pair<int, int>;
  static T op(const T &x, const T &y) { return std::min(x, y); }
  static constexpr T id() { return {std::numeric_limits<int>::max(), 0}; }
};

void euler_tour_lca_example(const EulerTour::G &g, int u, int v) {
  EulerTour et(g);
  SparseTable<Min> st(et.tour);

  // Usage
  int ui = et.index[u], vi = et.index[v];
  auto [lca_depth, lca_node] = st.fold(min(ui, vi), max(ui, vi) + 1);
  int distance = (et.depth[u] - lca_depth) + (et.depth[v] - lca_depth);
}
