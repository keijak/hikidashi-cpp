#include <bits/stdc++.h>
using namespace std;

struct BipartiteChecker {
  struct Edge {
    int to;
  };

 private:
  int n_;
  vector<vector<Edge>> g_;
  vector<int> depth_;
  vector<int> prev_;
  optional<deque<int>> odd_cycle_;

 public:
  explicit BipartiteChecker(vector<vector<Edge>> g)
      : n_(g.size()), g_(move(g)), depth_(n_, -1), prev_(n_, -1) {
    run();
  }

  // Returns true iff the given graph is bipartite.
  bool is_bipartite() const { return not odd_cycle_.has_value(); }

  // Returns one of the odd cycles.
  // Only valid when `is_bipartite()` is false.
  const deque<int> &odd_cycle() const { return odd_cycle_.value(); }

  // Coloring of a node (0 or 1).
  // Only valid when `is_bipartite()` is true.
  int color(int v) const { return depth_[v] % 2; }

 private:
  void run() {
    for (int i = 0; i < n_; ++i) {
      if (depth_[i] != -1) continue;
      bfs(i);
      if (odd_cycle_.has_value()) break;
    }
  }

  void bfs(int start) {
    queue<int> q;
    depth_[start] = 0;
    q.push(start);
    while (not q.empty()) {
      int v = q.front();
      q.pop();
      const int d = depth_[v] + 1;
      for (const auto &e : g_[v]) {
        if (depth_[e.to] != -1) {
          if (depth_[e.to] % 2 == d % 2) continue;
          // Detected an odd cycle!
          deque<int> cycle;
          for (; v != -1 and depth_[v] > depth_[e.to]; v = prev_[v]) {
            cycle.push_front(v);
          }
          int u = e.to;
          while (v != u) {
            assert(v != -1 and u != -1);
            cycle.push_front(v);
            cycle.push_back(u);
            v = prev_[v];
            u = prev_[u];
          }
          cycle.push_front(v);
          odd_cycle_ = std::move(cycle);
          return;
        }
        depth_[e.to] = d;
        prev_[e.to] = v;
        q.push(e.to);
      }
    }
  }
};
