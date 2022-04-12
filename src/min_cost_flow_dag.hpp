// base: https://kopricky.github.io/code/NetworkFlow/min_cost_flow_DAG.html
//
// The input graph may contain negative edge cost but must be a DAG.
// Calculates potential at each node using DP instead of Bellman-Ford,
// which is less general but faster.
//
// Usage: Single flow call.
//
//   MinCostFlowDAG<int,int> mcf(n);
//   for (...) mcf.add_edge(...);
//   auto [max_flow, min_cost] = mcf.flow(kSource, kSink, flow);
//   const auto& cost_slope = mcf.slope();
//
// Usage:  Multiple flow calls.
//
//   MinCostFlowDAG<int,int> mcf(n);
//   for (...) mcf.add_edge(...);
//   for (int f = 1; f <= X; ++f) {
//     auto mcf_runner = mcf;
//     auto [max_flow, min_cost] = mcf.flow(kSource, kSink, flow);
//   }

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
  std::vector<Cost> H, dist;
  std::vector<int> indeg, prevv, preve;
  std::optional<int> source_vertex_;
  std::vector<std::pair<Cap, Cost>> last_slope_;

  explicit MinCostFlowDAG(int node_count)
      : V(node_count),
        G(V),
        H(V, INF),    // potential table
        dist(V, 0),   // distance from source_vertex_
        indeg(V, 0),  // indegree of from each vertex
        prevv(V),
        preve(V) {}

  void add_edge(const int from, const int to, const Cap cap, const Cost cost) {
    if (cap == 0) return;
    G[from].push_back(Edge{to, (int)G[to].size(), cap, cost});
    G[to].push_back(Edge{from, (int)G[from].size() - 1, 0, -cost});
    ++indeg[to];
  }

  // Calculates (max_flow, min_cost) from `v_source` to `v_sink` where
  // `max_flow <= flow_limit`.
  // The piece-wise linear curve of (flow, cost) history will be stored
  // in `last_slope_`.
  std::pair<Cap, Cost> flow(const int v_source, const int v_sink,
                            Cap flow_limit = std::numeric_limits<Cap>::max() /
                                             4) {
    maybe_build(v_source);  // Maybe initialize the potential table.
    const Cap initial_flow_limit = flow_limit;
    Cost cur_cost = 0, min_cost = 0;
    Cap max_flow = 0;
    last_slope_.clear();  // Discard the previous record.
    last_slope_.emplace_back(0, 0);
    while (flow_limit > 0) {
      dijkstra(v_source);
      if (dist[v_sink] >= INF) break;
      update(v_source, v_sink, flow_limit, cur_cost);
      const Cap &cur_flow = initial_flow_limit - flow_limit;
      last_slope_.emplace_back(cur_flow, cur_cost);
      assert(cur_flow >= max_flow);
      max_flow = cur_flow;
      if (min_cost > cur_cost) min_cost = cur_cost;
    }
    return {max_flow, min_cost};
  }

  // Cost slope for the last flow() call.
  const std::vector<std::pair<Cap, Cost>> &slope() const { return last_slope_; }

 private:
  struct DijkstraState {
    Cost dist;
    int node;
  };
  friend bool operator>(const DijkstraState &x, const DijkstraState &y) {
    return x.dist > y.dist;
  }

  // Calculate the potential for the given source node.
  void maybe_build(const int v_source) {
    if (source_vertex_.has_value()) {
      assert(source_vertex_.value() == v_source);
      return;
    }
    source_vertex_ = v_source;
    auto ord = topological_sort();
    assert(ord.has_value());
    calc_potential(*ord, v_source);
  }

  std::optional<std::vector<int>> topological_sort() {
    std::vector<int> ord;
    std::queue<int> que;
    for (int i = 0; i < V; ++i) {
      if (indeg[i] == 0) que.push(i);
    }
    while (!que.empty()) {
      const int p = que.front();
      que.pop();
      ord.push_back(p);
      for (auto &e : G[p]) {
        if (e.cap > 0 && --indeg[e.to] == 0) que.push(e.to);
      }
    }
    if (*max_element(indeg.begin(), indeg.end()) != 0) {
      return std::nullopt;
    }
    return ord;
  }

  void calc_potential(const std::vector<int> &ord, const int s) {
    H[s] = 0;
    for (const int v : ord) {
      if (H[v] == INF) continue;
      for (const Edge &e : G[v]) {
        if (e.cap > 0) H[e.to] = min(H[e.to], H[v] + e.cost);
      }
    }
  }

  void dijkstra(const int s) {
    using Heap = std::priority_queue<DijkstraState, std::vector<DijkstraState>,
                                     std::greater<>>;
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
        if (e.cap > 0 and dist[e.to] > dist[v] + e.cost + H[v] - H[e.to]) {
          dist[e.to] = dist[v] + e.cost + H[v] - H[e.to];
          prevv[e.to] = v;
          preve[e.to] = i;
          heap.push(DijkstraState{dist[e.to], e.to});
        }
      }
    }
  }

  void update(const int s, const int t, Cap &f, Cost &res) {
    for (int i = 0; i < V; i++) {
      if (dist[i] != INF) H[i] += dist[i];
    }
    Cap d = f;
    for (int v = t; v != s; v = prevv[v]) {
      d = min(d, G[prevv[v]][preve[v]].cap);
    }
    f -= d;
    res += H[t] * d;
    for (int v = t; v != s; v = prevv[v]) {
      Edge &e = G[prevv[v]][preve[v]];
      e.cap -= d;
      G[v][e.rev].cap += d;
    }
  }
};
