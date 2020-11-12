#include <functional>
#include <vector>

// Heavy-Light Decomposition
struct HLD {
  using NodeID = int;  // [0, n)

  int n;                                   // number of nodes in the tree
  NodeID root;                             // root of the tree
  std::vector<std::vector<NodeID>> child;  // children node ids
  std::vector<NodeID> parent;              // parent node id (or -1)
  std::vector<int> subsize;                // subtree size
  // "ord" is preorder index in DFS traversal. [0, n)
  std::vector<int> node_to_ord;     // node id to preorder index
  std::vector<NodeID> ord_to_node;  // preorder index to node id
  std::vector<NodeID> comp_root;    // root of each heavy path component

  explicit HLD(std::vector<std::vector<int>> g, NodeID root = 0)
      : n(int(g.size())),
        root(root),
        child(g),
        parent(n, -1),
        subsize(n, 1),
        node_to_ord(n, -1),
        ord_to_node(n, -1),
        comp_root(n, -1) {
    dfs_subsize(root);
    int counter = 0;
    comp_root[root] = root;
    dfs_hld(root, counter);
  }

  // Lowest Common Ancestor
  NodeID lca(NodeID u, NodeID v) {
    for (;;) {
      if (node_to_ord[u] > node_to_ord[v]) std::swap(u, v);
      if (comp_root[u] == comp_root[v]) return u;
      v = parent[comp_root[v]];
    }
  }

  // f: [l, r) -> void
  // The intervals contain the preorder indices of the nodes in the
  // u-v path, including both u and v.
  void for_each(NodeID u, NodeID v, std::function<void(int, int)> f) {
    for (;;) {
      if (node_to_ord[u] > node_to_ord[v]) std::swap(u, v);
      f(std::max(node_to_ord[comp_root[v]], node_to_ord[u]),
        node_to_ord[v] + 1);
      if (comp_root[u] == comp_root[v]) break;
      v = parent[comp_root[v]];
    }
  }

  // f: [l, r) -> void
  // The intervals contain the preorder indices of nodes corresponding to the
  // deeper end (closer to leaves) of edges in the u-v path.
  void for_each_edge(NodeID u, NodeID v, std::function<void(int, int)> f) {
    for (;;) {
      if (node_to_ord[u] > node_to_ord[v]) std::swap(u, v);
      if (comp_root[u] == comp_root[v]) {
        if (u != v) f(node_to_ord[u] + 1, node_to_ord[v] + 1);
        break;
      }
      f(node_to_ord[comp_root[v]], node_to_ord[v] + 1);
      v = parent[comp_root[v]];
    }
  }

 private:
  // Fills `parent` and `subsize` and drops parent node ids from `child`.
  void dfs_subsize(NodeID v) {
    auto &edges = child[v];
    if (parent[v] >= 0) {
      auto it = std::find(edges.begin(), edges.end(), parent[v]);
      if (it != edges.end()) edges.erase(it);
    }
    for (NodeID &u : edges) {
      parent[u] = v;
      dfs_subsize(u);
      subsize[v] += subsize[u];
      if (subsize[u] > subsize[edges[0]]) {
        std::swap(u, edges[0]);
      }
    }
  }

  // Fills `node_to_ord`, `ord_to_node`, and `comp_root`.
  void dfs_hld(NodeID v, int &counter) {
    node_to_ord[v] = counter++;
    ord_to_node[node_to_ord[v]] = v;
    for (NodeID u : child[v]) {
      assert(u != parent[v]);
      comp_root[u] = (u == child[v][0] ? comp_root[v] : u);
      dfs_hld(u, counter);
    }
  }
};
