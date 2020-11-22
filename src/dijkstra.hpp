#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

template <class T>
using MinHeap = std::priority_queue<T, std::vector<T>, std::greater<T>>;

struct Edge {
  i64 cost;
  int to;
};

struct State {
  i64 cost;
  int node;
  //...
};
bool operator>(const State &x, const State &y) { return x.cost > y.cost; }

// Returns min distance from the source node to each node (if exists).
std::vector<std::optional<i64>> dijkstra(
    const std::vector<std::vector<Edge>> &g, int source) {
  const int n = g.size();
  std::vector<std::optional<i64>> mincost(n);
  mincost[source] = 0LL;
  MinHeap<State> que;
  que.push({0LL, source});
  while (not que.empty()) {
    State cur = std::move(que.top());
    que.pop();
    if (not mincost[cur.node].has_value() or
        cur.cost != mincost[cur.node].value()) {
      continue;
    }
    for (const auto &e : g[cur.node]) {
      i64 new_cost = cur.cost + e.cost;
      if (not mincost[e.to].has_value() or mincost[e.to].value() > new_cost) {
        mincost[e.to] = new_cost;
        que.push({new_cost, e.to});
      }
    }
  }
  return mincost;
}

std::vector<std::optional<i64>> grid_bfs(const std::vector<std::string> &g,
                                         int source_r, int source_c) {
  const i64 INF = 1e18;
  const int H = g.size();
  const int W = g[0].size();
  auto pack = [&](int r, int c) -> int { return r * W + c; };
  auto unpack = [&](int node) -> pair<int, int> {
    return {node / W, node % W};
  };
  array<int, 4> dx = {0, 0, 1, -1}, dy = {1, -1, 0, 0};
  auto mincost = std::vector(H * W, std::optional<i64>());
  int source_node = pack(source_r, source_c);
  mincost[source_node] = 0LL;
  std::deque<State> que;
  que.push_back({0LL, source_node});
  while (not que.empty()) {
    State cur = std::move(que.front());
    que.pop_front();
    if (not mincost[cur.node].has_value() or
        mincost[cur.node].value() != cur.cost) {
      continue;
    }
    auto [cur_r, cur_c] = unpack(cur.node);
    for (int i = 0; i < 4; ++i) {
      int nr = cur_r + dy[i];
      int nc = cur_c + dx[i];
      if (nr < 0 or nr >= H or nc < 0 or nc >= W) continue;
      if (g[nr][nc] == '#') continue;
      int new_node = pack(nr, nc);
      i64 new_cost = cur.cost + 1;
      if (mincost[new_node].value_or(INF) > new_cost) {
        mincost[new_node] = new_cost;
        que.push_back({new_cost, new_node});
      }
    }
  }
  return mincost;
}
