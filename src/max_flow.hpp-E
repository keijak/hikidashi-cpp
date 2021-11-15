#include <bits/stdc++.h>

template <typename Cap>
struct Dinic {
 private:
  static const Cap INF = std::numeric_limits<Cap>::max();

  struct Edge {
    int to;
    Cap cap;
    int rev;
    bool isrev;
    int idx;
  };
  std::vector<std::vector<Edge>> g_;

  std::vector<int> min_cost_, iter_;

 public:
  explicit Dinic(int n) : g_(n) {}

  int size() const { return (int)g_.size(); }

  void resize(int x) { g_.resize(x); }

  void add_edge(int from, int to, Cap cap, int idx = -1) {
    g_[from].eb(Edge{to, cap, g_[to].size(), false, idx});
    g_[to].eb(Edge{from, 0, g_[from].size() - 1, true, idx});
  }

  Cap max_flow(int s, int t) {
    Cap flow = 0;
    while (bfs(s, t)) {
      iter_.assign(g_.size(), 0);
      for (;;) {
        auto f = dfs(s, t, INF);
        if (f <= 0) break;
        flow += f;
      }
    }
    return flow;
  }

 private:
  bool bfs(int s, int t) {
    min_cost_.assign(SZ(g_), -1);
    std::queue<int> que;
    min_cost_[s] = 0;
    que.push(s);
    while (!que.empty() and min_cost_[t] == -1) {
      int p = que.front();
      que.pop();
      for (auto &e : g_[p]) {
        if (e.cap > 0 and min_cost_[e.to] == -1) {
          min_cost_[e.to] = min_cost_[p] + 1;
          que.push(e.to);
        }
      }
    }
    return min_cost_[t] != -1;
  }

  Cap dfs(int idx, const int t, Cap flow) {
    if (idx == t) return flow;
    for (int &i = iter_[idx]; i < SZ(g_[idx]); i++) {
      Edge &e = g_[idx][i];
      if (e.cap > 0 and min_cost_[idx] < min_cost_[e.to]) {
        Cap d = dfs(e.to, t, min(flow, e.cap));
        if (d > 0) {
          e.cap -= d;
          g_[e.to][e.rev].cap += d;
          return d;
        }
      }
    }
    return 0;
  }
};
