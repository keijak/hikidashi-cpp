#include <vector>

// Heavy-Light Decomposition and Euler Tour
struct HLD {
  int n;                              // number of nodes in the tree
  std::vector<std::vector<int>> adj;  // The tree's adjacency list.
  std::vector<int> subsize;           // subtree size
  std::vector<int> parent;            // parent node id (or -1)
  // "index" is preorder index in DFS traversal (Euler Tour).
  std::vector<int> node_to_index;  // node id to index
  std::vector<int> index_to_node;  // index to node id
  std::vector<int> comp_root;      // root of its heavy path component
  int root;                        // root of the tree

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

  // f: [l, r) -> void
  // The intervals contain the indices of both u and v.
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

  // f: [l, r) -> void
  // The intervals contain the indices of nodes that represent each edge's
  // deeper end (closer to leaves).
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
      auto it = std::find(edges.begin(), edges.end(), parent[v]);
      if (it != edges.end()) edges.erase(it);
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
