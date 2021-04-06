#include <vector>
using namespace std;
using i64 = long long;

// tour: preorder node ids
// The interval [pre_index[v], post_index[v]) represents a subtree whose
// root is v.
struct EulerTour {
  using G = vector<vector<int>>;

  const int n;  // number of nodes
  G g;

  // Euler Tour on nodes.
  vector<pair<int, int>> tour;  // (depth, node id)
  vector<int> pre_index;   // index in the tour on entering the subtree of v
  vector<int> post_index;  // index in the tour on exiting the subtree of v
  vector<int> depth;

  explicit EulerTour(G g, int root = 0)
      : n((int)g.size()),
        g(move(g)),
        pre_index(n, -1),
        post_index(n, -1),
        depth(n, -1) {
    tour.reserve(n * 2);
    depth[root] = 0;
    dfs(root, -1);
  }

 private:
  void dfs(int v, int p) {
    pre_index[v] = int(tour.size());
    if (p >= 0) depth[v] = depth[p] + 1;
    tour.emplace_back(depth[v], v);
    for (auto u : g[v]) {
      if (u == p) continue;
      dfs(u, v);
      tour.emplace_back(depth[v], v);
    }
    post_index[v] = int(tour.size());
  }
};

struct MinPair {
  using T = pair<int, int>;
  static T op(const T &x, const T &y) { return std::min(x, y); }
  static constexpr T id() { return {std::numeric_limits<int>::max(), 0}; }
};

// Returns the LCA of node u and node v.
tuple<int, int> path_lca(const EulerTour::G &g, int u, int v) {
  EulerTour et(g);
  SparseTable<MinPair> st(et.tour);

  // Usage
  int ui = et.pre_index[u], vi = et.pre_index[v];
  if (ui > vi) swap(ui, vi);
  auto [lca_depth, lca_node] = st.fold(ui, vi + 1);
  int path_distance = (et.depth[u] - lca_depth) + (et.depth[v] - lca_depth);
  return {path_distance, lca_node};
}

struct MinInt {
  using T = int;
  static T op(const T &x, const T &y) { return std::min(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::max(); }
};

struct MaxInt {
  using T = int;
  static T op(const T &x, const T &y) { return std::max(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::lowest(); }
};

// Returns the LCA of all nodes in [l, r).
tuple<int, int> range_lca(const EulerTour::G &g, int l, int r) {
  EulerTour et(g);
  SparseTable<MinPair> tour_st(et.tour);
  SparseTable<MinInt> min_st(et.pre_index);
  SparseTable<MaxInt> max_st(et.pre_index);

  int mini = min_st.fold(l, r);
  int maxi = max_st.fold(l, r);
  auto [lca_depth, lca_node] = tour_st.fold(mini, maxi + 1);
  return {lca_node, lca_depth};
}

// tour: preorder edge costs.
// The interval [pre_index[v], post_index[v]) represents a subtree whose
// root is v.
struct EulerTourOnEdge {
  struct Edge {
    i64 cost;
    int to;
  };
  using G = vector<vector<Edge>>;

  const int n;  // number of nodes
  G g;

  // Euler Tour on edges.
  vector<pair<i64, bool>> tour;  // (edge cost, entering?)
  vector<int> pre_index;   // index in the tour on entering the subtree of v
  vector<int> post_index;  // index in the tour on exiting the subtree of v
  vector<int> depth;

  explicit EulerTourOnEdge(G g, int root = 0)
      : n((int)g.size()),
        g(move(g)),
        pre_index(n, -1),
        post_index(n, -1),
        depth(n, -1) {
    tour.reserve(n * 2);
    depth[root] = 0;
    dfs(root, -1);
  }

 private:
  void dfs(int v, int p) {
    pre_index[v] = int(tour.size());
    if (p >= 0) depth[v] = depth[p] + 1;
    for (const Edge &e : g[v]) {
      if (e.to == p) continue;
      tour.push_back({e.cost, true});
      dfs(e.to, v);
      tour.push_back({e.cost, false});
    }
    post_index[v] = int(tour.size());
  }
};
