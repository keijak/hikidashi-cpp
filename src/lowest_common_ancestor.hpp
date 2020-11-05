// Lowest Common Ancestor of a tree.
class LCA {
 public:
  const int n;  // number of nodes
  const vector<vector<int>> &adj;
  vector<vector<int>> upper;  // 2^k upper node
  vector<int> depth;

  explicit LCA(const Graph &g)
      : n(g.n), adj(g.adj), upper(K, vector<int>(g.n, -1)), depth(g.n, -1) {
    dfs(0, -1, 0);
    for (int k = 0; k + 1 < K; k++) {
      REP(v, n) {
        if (upper[k][v] < 0) {
          upper[k + 1][v] = -1;
        } else {
          upper[k + 1][v] = upper[k][upper[k][v]];
        }
      }
    }
  }

  // Returns the node ID of the lowest common ancestor.
  int ancestor(int u, int v) const {
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
    int p = ancestor(u, v);
    return (depth[u] - depth[p]) + (depth[v] - depth[p]);
  }

 private:
  void dfs(int v, int p, int d) {
    upper[0][v] = p;
    depth[v] = d;
    for (auto x : adj[v]) {
      if (x == p) continue;
      dfs(x, v, d + 1);
    }
  }

  static const int K = 30;  // max upper lookup (2^K)
};