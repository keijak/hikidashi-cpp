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

const i64 INF = 1e18;

std::vector<i64> dijkstra(const std::vector<std::vector<Edge>> &g, int source) {
  const int n = g.size();
  std::vector<i64> mincost(n, INF);
  mincost[source] = 0LL;
  MinHeap<State> que;
  que.push({0LL, source});
  while (not que.empty()) {
    State cur = std::move(que.top());
    que.pop();
    if (cur.cost != mincost[cur.node]) {
      continue;
    }
    for (const auto &e : g[cur.node]) {
      i64 new_cost = cur.cost + e.cost;
      if (mincost[e.to] > new_cost) {
        mincost[e.to] = new_cost;
        que.push({new_cost, e.to});
      }
    }
  }
  return mincost;
}

std::vector<i64> grid_bfs(const std::vector<std::string> &g, int source_r,
                          int source_c) {
  const int H = g.size();
  const int W = g[0].size();
  array<int, 4> dx = {0, 0, 1, -1}, dy = {1, -1, 0, 0};
  auto mincost = std::vector(H, std::vector(W, INF));
  mincost[source_r][source_c] = 0LL;
  std::deque<State> que;
  que.push_back({0LL, source_r, source_c});
  while (not que.empty()) {
    State cur = std::move(que.front());
    que.pop_front();
    if (cur.cost != mincost[cur.node]) {
      continue;
    }
    for (int i = 0; i < 4; ++i) {
      int nr = cur.r + dy[i];
      int nc = cur.c + dx[i];
      if (nr < 0 or nr >= H or nc < 0 or nc >= W) continue;
      if (g[nr][nc] == '#') continue;
      i64 new_cost = cur.cost + 1;
      if (mincost[nr][nc] > new_cost) {
        mincost[nr][nc] = new_cost;
        que.push_back({new_cost, nr, nc});
      }
    }
  }
  return mincost;
}
