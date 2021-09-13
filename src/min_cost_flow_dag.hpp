// https://kopricky.github.io/code/NetworkFlow/min_cost_flow_DAG.html
#include <bits/stdc++.h>

template <typename Cap, typename Cost>
class MinCostFlowDAG {
 public:
  static constexpr Cost INF = std::numeric_limits<Cost>::max();

  struct Edge {
    int to, rev;
    Cap cap;
    Cost cost;
  };

  const int V;
  std::vector<std::vector<Edge>> G;
  std::vector<Cost> h, dist;
  std::vector<int> deg, ord, prevv, preve;

  explicit MinCostFlowDAG(int node_count)
      : V(node_count),
        G(V),
        h(V, INF),
        dist(V, 0),
        deg(V, 0),
        prevv(V),
        preve(V) {}

  void add_edge(const int from, const int to, const Cap cap, const Cost cost) {
    if (cap == 0) return;
    G[from].push_back(Edge{to, (int)G[to].size(), cap, cost});
    G[to].push_back(Edge{from, (int)G[from].size() - 1, 0, -cost});
    ++deg[to];
  }

  std::optional<Cost> flow(const int s, const int t, Cap f) {
    assert(topological_sort());  // must be a DAG.
    calc_potential(s);
    Cost res = 0;
    while (f > 0) {
      dijkstra(s);
      if (dist[t] == INF) return std::nullopt;
      update(s, t, f, res);
    }
    return res;
  }

 private:
  struct DijkstraState {
    Cost dist;
    int node;
  };
  friend bool operator>(const DijkstraState &x, const DijkstraState &y) {
    return x.dist > y.dist;
  }

  bool topological_sort() {
    std::queue<int> que;
    for (int i = 0; i < V; ++i) {
      if (deg[i] == 0) que.push(i);
    }
    while (!que.empty()) {
      const int p = que.front();
      que.pop();
      ord.push_back(p);
      for (auto &e : G[p]) {
        if (e.cap > 0 && --deg[e.to] == 0) que.push(e.to);
      }
    }
    return (*max_element(deg.begin(), deg.end()) == 0);
  }

  void calc_potential(const int s) {
    h[s] = 0;
    for (const int v : ord) {
      if (h[v] == INF) continue;
      for (const Edge &e : G[v]) {
        if (e.cap > 0) h[e.to] = min(h[e.to], h[v] + e.cost);
      }
    }
  }

  void dijkstra(const int s) {
    using Heap = std::priority_queue<DijkstraState, std::vector<DijkstraState>,
                                     std::greater<DijkstraState>>;
    Heap heap;
    fill(dist.begin(), dist.end(), INF);
    dist[s] = 0;
    heap.push(DijkstraState{0, s});
    while (not heap.empty()) {
      const auto cur = heap.top();
      heap.pop();
      const int v = cur.node;
      if (dist[v] < cur.dist) continue;
      for (int i = 0; i < (int)G[v].size(); ++i) {
        const Edge &e = G[v][i];
        if (e.cap > 0 and dist[e.to] > dist[v] + e.cost + h[v] - h[e.to]) {
          dist[e.to] = dist[v] + e.cost + h[v] - h[e.to];
          prevv[e.to] = v;
          preve[e.to] = i;
          heap.push(DijkstraState{dist[e.to], e.to});
        }
      }
    }
  }

  void update(const int s, const int t, Cap &f, Cost &res) {
    for (int i = 0; i < V; i++) {
      if (dist[i] != INF) h[i] += dist[i];
    }
    Cap d = f;
    for (int v = t; v != s; v = prevv[v]) {
      d = min(d, G[prevv[v]][preve[v]].cap);
    }
    f -= d;
    res += h[t] * d;
    for (int v = t; v != s; v = prevv[v]) {
      Edge &e = G[prevv[v]][preve[v]];
      e.cap -= d;
      G[v][e.rev].cap += d;
    }
  }
};
