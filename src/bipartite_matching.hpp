#include <bits/stdc++.h>

// Bipartite Matching between two sets of vertices
// V1 (size n) and V2 (size m). O(sqrt(n+m) * |E|).
struct HopcroftKarp {
  std::vector<std::vector<int> > graph;
  std::vector<int> dist, match;
  std::vector<bool> used, vv;

  // (n = |V1|, m = |V2|): sizes of the two sets of vertices.
  HopcroftKarp(int n, int m) : graph(n), match(m, -1), used(n) {}

  // Adds an edge between u (in V1) and v (in V2).
  void add_edge(int u, int v) {
    assert(0 <= u and u < (int)graph.size());  // [0, n)
    assert(0 <= v and v < (int)match.size());  // [0, m)
    graph[u].push_back(v);
  }

  // Returns the number of max bipartite matching.
  // The matching result is stored in `match` (match[v] == u).
  int bipartite_matching() {
    int ret = 0;
    while (true) {
      bfs();
      vv.assign(graph.size(), false);
      int flow = 0;
      for (int i = 0; i < (int)graph.size(); i++) {
        if (!used[i] && dfs(i)) ++flow;
      }
      if (flow == 0) return ret;
      ret += flow;
    }
  }

  friend std::ostream &operator<<(std::ostream &os, const HopcroftKarp &bm) {
    os << "Matching{";
    for (int i = 0; i < (int)bm.match.size(); i++) {
      if (bm.match[i] != -1) {
        os << "(" << bm.match[i] << ", " << i << "),\n";
      }
    }
    return os << "}\n";
  }

 private:
  void bfs() {
    dist.assign(graph.size(), -1);
    std::queue<int> que;
    for (int i = 0; i < (int)graph.size(); i++) {
      if (!used[i]) {
        que.emplace(i);
        dist[i] = 0;
      }
    }

    while (!que.empty()) {
      int a = que.front();
      que.pop();
      for (auto &b : graph[a]) {
        int c = match[b];
        if (c >= 0 && dist[c] == -1) {
          dist[c] = dist[a] + 1;
          que.emplace(c);
        }
      }
    }
  }

  bool dfs(int a) {
    vv[a] = true;
    for (auto &b : graph[a]) {
      int c = match[b];
      if (c < 0 || (!vv[c] && dist[c] == dist[a] + 1 && dfs(c))) {
        match[b] = a;
        used[a] = true;
        return true;
      }
    }
    return false;
  }
};
