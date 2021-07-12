#include <cassert>
#include <functional>
#include <map>
#include <optional>
#include <vector>
using i64 = long long;

// Heavy-Light Decomposition
struct HLDecomp {
  // Whether the given adjacency list only contains children or may also contain
  // parents. If true, skips removing parent from the graph for speedup.
  static constexpr bool kChildrenOnlyGraph = false;

  using NodeID = int;                       // [0, n)
  using G = std::vector<std::vector<int>>;  // undirected graph
  // half-open intervals of preorder indices of nodes.
  using IntervalVec = std::vector<std::pair<int, int>>;

  int n;                       // number of nodes in the tree
  NodeID root;                 // root of the tree
  G child;                     // children node ids
  std::vector<NodeID> parent;  // parent node id (or -1 on the root node)
  std::vector<int> subsize;    // subtree size
  // "ord" is preorder index in DFS traversal. [0, n)
  std::vector<int> node_to_ord;     // node id to preorder index
  std::vector<NodeID> ord_to_node;  // preorder index to node id
  std::vector<NodeID> comp_root;    // node id to its heavy path component

  explicit HLDecomp(G g, NodeID root = 0)
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
  NodeID lca(NodeID u, NodeID v) const {
    for (;;) {
      if (node_to_ord[u] > node_to_ord[v]) std::swap(u, v);
      NodeID crv = comp_root[v];
      if (comp_root[u] == crv) return u;
      assert(parent[crv] != -1);
      v = parent[crv];
    }
  }

  // Returns the set of nodes on the u-v path, including both u and v.
  //
  // The return value is half-open intervals of the preorder indices of the
  // nodes. One interval corresponds to one heavy path component.
  IntervalVec node_ranges_on_path(NodeID u, NodeID v) const {
    IntervalVec res;
    for (;;) {
      if (node_to_ord[u] > node_to_ord[v]) std::swap(u, v);
      NodeID crv = comp_root[v];
      res.emplace_back(std::max(node_to_ord[crv], node_to_ord[u]),
                       node_to_ord[v] + 1);
      if (comp_root[u] == crv) break;
      assert(parent[crv] != -1);
      v = parent[crv];
    }
    return res;
  }

  // Returns the set of edges in the u-v path.
  //
  // The return value is half-open intervals of the preorder indices of nodes
  // corresponding to the deeper end (closer to leaves) of each edge in the
  // path. Here we identify Edge(v, parent[v]) as v.
  IntervalVec edge_ranges_on_path(NodeID u, NodeID v) const {
    IntervalVec res;
    for (;;) {
      if (node_to_ord[u] > node_to_ord[v]) std::swap(u, v);
      NodeID crv = comp_root[v];
      if (comp_root[u] == crv) {
        if (u != v) res.emplace_back(node_to_ord[u] + 1, node_to_ord[v] + 1);
        break;
      }
      res.emplace_back(node_to_ord[crv], node_to_ord[v] + 1);
      assert(parent[crv] != -1);
      v = parent[crv];
    }
    return res;
  }

  // Distance (= number of edges) of the path between two nodes.
  int distance(NodeID u, NodeID v) const {
    int dist = 0;
    for (const auto &[l, r] : edge_ranges_on_path(u, v)) {
      dist += r - l;
    }
    return dist;
  }

 private:
  // Fills `parent` and `subsize`.
  void dfs_subsize(NodeID v) {
    auto &edges = child[v];
    if constexpr (not kChildrenOnlyGraph) {
      if (parent[v] != -1) {
        // Remove the parent from `child[v]`. Amortized O(N).
        auto it = std::find(edges.begin(), edges.end(), parent[v]);
        if (it != edges.end()) {
          std::swap(*it, edges.back());
          edges.pop_back();
        }
      }
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
      comp_root[u] = (u == child[v][0] ? comp_root[v] : u);
      dfs_hld(u, counter);
    }
  }
};

// Returns the cumulative sum array of edge costs (indexed by Ord).
std::vector<i64> edge_cost_cumsum(
    const HLDecomp &hld, const std::map<std::pair<int, int>, i64> &edge_cost) {
  const int n = hld.n;
  std::vector<i64> cost(n), acc(n + 1);  // indexed by Ord.
  // Iterate over the NodeID space.
  for (int v = 0; v < n; ++v) {
    // Calculate the cost from v to parent[v].
    auto p = hld.parent[v];
    if (p == -1) continue;
    auto it = edge_cost.find(std::pair{std::min(v, p), std::max(v, p)});
    if (it == edge_cost.end()) continue;
    const i64 cost_to_parent = it->second;
    const int ord = hld.node_to_ord[v];
    cost[ord] = cost_to_parent;
  }
  // Iterate over the Ord space.
  for (int i = 0; i < n; ++i) {
    acc[i + 1] = acc[i] + cost[i];
  }
  return acc;
}
