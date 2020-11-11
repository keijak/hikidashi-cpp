#include <vector>

// Heavy-Light Decomposition and Euler Tour
struct HLD {
  int n;
  std::vector<std::vector<int>> adj;
  std::vector<int> sub_size, et_in, et_out, component_root;
  int root;
  int counter;

  explicit HLD(std::vector<std::vector<int>> g, int root_ = 0)
      : n(int(g.size())),
        adj(g),
        sub_size(n),
        et_in(n),
        et_out(n),
        component_root(n),
        root(root_),
        counter(0) {
    dfs_subsize(root, -1);

    component_root[root] = root;
    dfs_hld(root, -1);
  }

 private:
  void dfs_subsize(int v, int p) {
    sub_size[v] = 1;
    for (auto &u : adj[v]) {
      if (u == p) continue;
      dfs_subsize(u, v);
      sub_size[v] += sub_size[u];
      if (sub_size[u] > sub_size[adj[v][0]]) {
        std::swap(u, adj[v][0]);
      }
    }
  }

  void dfs_hld(int v, int p) {
    et_in[v] = counter++;
    for (auto u : adj[v]) {
      if (u == p) continue;
      component_root[u] = (u == adj[v][0] ? component_root[v] : u);
      dfs_hld(u, v);
    }
    et_out[v] = counter;
  }
};
