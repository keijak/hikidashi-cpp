#include <bits/stdc++.h>

struct CentroidTree {
  int n;                    // number of nodes.
  int root;                 // root of the centroid tree.
  std::vector<int> parent;  // parent of each node in the centroid tree.
  std::vector<std::vector<int>> graph;  // adjacent nodes in the centroid tree.

  explicit CentroidTree(std::vector<std::vector<int>> g)
      : n(g.size()), root(-1), parent(n, -1), graph(n) {
    std::vector<std::set<int>> gd(n);
    std::vector<int> subsize(n, 0);
    for (int v = 0; v < n; ++v) {
      gd[v].insert(g[v].begin(), g[v].end());
    }
    const int start_node = 0;
    build(start_node, -1, gd, subsize);
    for (int v = 0; v < n; ++v) {
      const int p = parent[v];
      if (p == -1) {
        root = v;
      } else {
        graph[p].push_back(v);
        graph[v].push_back(p);
      }
    }
  }

 private:
  void build(int v, int p, std::vector<std::set<int>> &gd,
             std::vector<int> &subsize) {
    const int sz = dfs_subsize(v, p, gd, subsize);  // fill subsize
    const int c = centroid(v, p, sz, gd, subsize);  // find centroid
    parent[c] = p;

    std::vector<int> tmp(gd[c].begin(), gd[c].end());
    for (auto u : tmp) {
      gd[c].erase(u);
      gd[u].erase(c);
      build(u, c, gd, subsize);
    }
  }

  int dfs_subsize(int v, int p, const std::vector<std::set<int>> &gd,
                  std::vector<int> &subsize) {
    subsize[v] = 1;
    for (auto u : gd[v]) {
      if (u == p) continue;
      subsize[v] += dfs_subsize(u, v, gd, subsize);
    }
    return subsize[v];
  }

  int centroid(int v, int p, int sz, const std::vector<std::set<int>> &gd,
               const std::vector<int> &subsize) {
    for (auto u : gd[v]) {
      if (u == p) continue;
      if (subsize[u] > sz / 2) {
        return centroid(u, v, sz, gd, subsize);
      }
    }
    return v;
  }
};
