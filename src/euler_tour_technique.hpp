#include <tuple>
#include <vector>
using namespace std;
using i64 = long long;

// tour: preorder node ids
// The interval [in[v], out[v]) represents a subtree whose
// root is v.
struct EulerTour {
  using G = vector<vector<int>>;

  const int n;  // number of nodes
  const G &g;

  // Euler Tour on nodes.
  vector<int> in;   // index in the tour on entering the subtree of v
  vector<int> out;  // index in the tour on exiting the subtree of v
  vector<int> depth;
  vector<int> node_tour;
  vector<int> depth_tour;

  explicit EulerTour(const G &g, int root = 0)
      : n((int)g.size()), g(g), in(n, -1), out(n, -1), depth(n, -1) {
    node_tour.reserve(n * 2);
    depth_tour.reserve(n * 2);
    depth[root] = 0;
    dfs(root, -1);
  }

 private:
  void dfs(int v, int p) {
    in[v] = int(node_tour.size());
    if (p != -1) depth[v] = depth[p] + 1;
    node_tour.push_back(v);
    depth_tour.push_back(depth[v]);
    for (auto u : g[v]) {
      if (u == p) continue;
      dfs(u, v);
      // node_tour.push_back(v);
      // depth_tour.push_back(depth[v]);
    }
    out[v] = int(node_tour.size());
  }
};

// Returns the LCA of node u and node v.
std::tuple<int, int, int> path_lca(const EulerTour::G &g, int u, int v) {
  EulerTour et(g);
  RMQ rmq(et.depth_tour.size(),
          [&](int i, int j) { return et.depth_tour[i] < et.depth_tour[j]; });
  {  // query
    int l = et.in[u], r = et.in[v];
    if (l > r) swap(l, r);
    int lca_ord = rmq.fold(l, r + 1);
    int lca_node = et.node_tour[lca_ord];
    int lca_depth = et.depth_tour[lca_ord];
    int path_distance = (et.depth[u] - lca_depth) + (et.depth[v] - lca_depth);
    return {lca_node, lca_depth, path_distance};
  }
}

// Returns the LCA of all nodes in [l, r).
pair<int, int> range_lca(const EulerTour::G &g, int l, int r) {
  EulerTour et(g);
  RMQ depth_rmq(et.depth_tour.size(), [&](int i, int j) {
    return et.depth_tour[i] < et.depth_tour[j];
  });
  RMQ min_in(et.in.size(), [&](int i, int j) { return et.in[i] < et.in[j]; });
  RMQ max_in(et.in.size(), [&](int i, int j) { return et.in[i] > et.in[j]; });
  {  // query
    int mini = min_in.fold(l, r);
    int maxi = max_in.fold(l, r);
    int lca_ord = depth_rmq.fold(mini, maxi + 1);
    int lca_node = et.node_tour[lca_ord];
    int lca_depth = et.depth_tour[lca_ord];
    return {lca_node, lca_depth};
  }
}

// tour: preorder edge costs.
// The interval [in[v], out[v]) represents a subtree whose
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
  vector<int> in;   // index in the tour on entering the subtree of v
  vector<int> out;  // index in the tour on exiting the subtree of v
  vector<int> depth;

  explicit EulerTourOnEdge(G g, int root = 0)
      : n((int)g.size()), g(move(g)), in(n, -1), out(n, -1), depth(n, -1) {
    tour.reserve(n * 2);
    depth[root] = 0;
    dfs(root, -1);
  }

 private:
  void dfs(int v, int p) {
    in[v] = int(tour.size());
    if (p >= 0) depth[v] = depth[p] + 1;
    for (const Edge &e : g[v]) {
      if (e.to == p) continue;
      tour.push_back({e.cost, true});
      dfs(e.to, v);
      tour.push_back({e.cost, false});
    }
    out[v] = int(tour.size());
  }
};
