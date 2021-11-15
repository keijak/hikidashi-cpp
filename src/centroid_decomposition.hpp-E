#include <bits/stdc++.h>

struct CentroidTree {
  int n_;
  std::vector<int> cpar_;  // parent of each node in the centroid tree

  explicit CentroidTree(std::vector<std::vector<int>> g, int root = 0)
      : n_(g.size()), cpar_(n_, -1) {
    std::vector<std::set<int>> gd(n_);
    std::vector<int> subsize(n_, 0);
    for (int v = 0; v < n_; ++v) {
      gd[v].insert(g[v].begin(), g[v].end());
    }
    build(root, -1, gd, subsize);
  }

 private:
  void build(int v, int p, std::vector<std::set<int>> &gd,
             std::vector<int> &subsize) {
    const int n = dfs_subsize(v, p, gd, subsize);  // fill subsize
    const int c = centroid(v, p, n, gd, subsize);  // find centroid
    cpar_[c] = p;

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

  int centroid(int v, int p, int n, const std::vector<std::set<int>> &gd,
               const std::vector<int> &subsize) {
    for (auto u : gd[v]) {
      if (u == p) continue;
      if (subsize[u] > n / 2) {
        return centroid(u, v, n, gd, subsize);
      }
    }
    return v;
  }
};
