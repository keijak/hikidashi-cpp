// Lowest Common Ancestor of a tree.
class LCA {
 public:
  const int n;  // number of nodes
  const vector<vector<int>> &adj;
  vector<vector<int>> parent;
  vector<int> depth;

  explicit LCA(const Graph &g)
      : n(g.n), adj(g.adj), parent(K, vector<int>(g.n, -1)), depth(g.n, -1) {
    dfs(0, -1, 0);
    for (int k = 0; k + 1 < K; k++) {
      REP(v, n) {
        if (parent[k][v] < 0) {
          parent[k + 1][v] = -1;
        } else {
          parent[k + 1][v] = parent[k][parent[k][v]];
        }
      }
    }
  }

  // Returns the node ID of the lowest common ancestor.
  int query(int u, int v) const {
    if (depth[u] > depth[v]) swap(u, v);
    for (int k = 0; k < K; k++) {
      if ((depth[v] - depth[u]) & (1 << k)) {
        v = parent[k][v];
      }
    }
    if (u == v) return u;
    for (int k = K - 1; k >= 0; k--) {
      if (parent[k][u] != parent[k][v]) {
        u = parent[k][u];
        v = parent[k][v];
      }
    }
    return parent[0][u];
  }

  // Returns the distance (number of edges) between two nodes.
  int distance(int u, int v) const {
    int p = query(u, v);
    return (depth[u] - depth[p]) + (depth[v] - depth[p]);
  }

 private:
  void dfs(int v, int p, int d) {
    parent[0][v] = p;
    depth[v] = d;
    for (auto x : adj[v]) {
      if (x == p) continue;
      dfs(x, v, d + 1);
    }
  }

  static const int K = 30;  // max parent lookup (2^K)
};