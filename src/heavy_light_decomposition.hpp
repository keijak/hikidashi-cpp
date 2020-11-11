#include <vector>

// Heavy-Light Decomposition and Euler Tour
struct HLD {
  int n;
  std::vector<std::vector<int>> adj;
  std::vector<int> subsize, parent, node_to_index, index_to_node, comp_root;
  int root;
  int counter;

  explicit HLD(std::vector<std::vector<int>> g, int root = 0)
      : n(int(g.size())),
        adj(g),
        subsize(n, 1),
        parent(n, -1),
        node_to_index(n, -1),
        index_to_node(n, -1),
        comp_root(n, -1),
        root(root) {
    dfs_subsize(root);
    int counter = 0;
    comp_root[root] = root;
    dfs_hld(root, counter);
  }

  int lca(int u, int v) {
    for (;;) {
      if (node_to_index[u] > node_to_index[v]) std::swap(u, v);
      if (comp_root[u] == comp_root[v]) return u;
      v = parent[comp_root[v]];
    }
  }

  template <typename F>
  void for_each(int u, int v, const F &f) {
    for (;;) {
      if (node_to_index[u] > node_to_index[v]) std::swap(u, v);
      f(std::max(node_to_index[comp_root[v]], node_to_index[u]),
        node_to_index[v] + 1);
      if (comp_root[u] == comp_root[v]) break;
      v = parent[comp_root[v]];
    }
  }

  template <typename F>
  void for_each_edge(int u, int v, const F &f) {
    for (;;) {
      if (node_to_index[u] > node_to_index[v]) std::swap(u, v);
      if (comp_root[u] == comp_root[v]) {
        if (u != v) f(node_to_index[u] + 1, node_to_index[v] + 1);
        break;
      }
      f(node_to_index[comp_root[v]], node_to_index[v] + 1);
      v = parent[comp_root[v]];
    }
  }

 private:
  // Fills `subsize` and `parent`.
  void dfs_subsize(int v) {
    auto &edges = adj[v];
    if (parent[v] >= 0) {
      // Drop the parent from `adj`. It's separately stored in `parent`.
      edges.erase(std::find(edges.begin(), edges.end(), parent[v]));
    }
    for (int &u : edges) {
      parent[u] = v;
      dfs_subsize(u);
      subsize[v] += subsize[u];
      if (subsize[u] > subsize[edges[0]]) {
        std::swap(u, edges[0]);
      }
    }
  }

  // Fills `node_to_index`, `index_to_node`, and `comp_root`.
  void dfs_hld(int v, int &counter) {
    node_to_index[v] = counter++;
    index_to_node[node_to_index[v]] = v;
    for (int u : adj[v]) {
      assert(u != parent[v]);
      comp_root[u] = (u == adj[v][0] ? comp_root[v] : u);
      dfs_hld(u, counter);
    }
  }
};
