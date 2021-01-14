#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
template <typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}

const i64 INF = 1e16;

template <class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

struct Edge {
  i64 cost;
  int to;
};

struct State {
  i64 cost;
  int node;
};
bool operator>(const State &x, const State &y) { return x.cost > y.cost; }

// Returns min distance from the source node to each node (if exists).
vector<i64> dijkstra(const vector<vector<Edge>> &g, int source) {
  const int n = g.size();
  vector<i64> mincost(n, INF);
  mincost[source] = 0LL;
  MinHeap<State> que;
  que.push({0, source});
  while (not que.empty()) {
    State cur = move(que.top());
    que.pop();
    if (cur.cost > mincost[cur.node]) continue;
    for (const auto &e : g[cur.node]) {
      auto new_cost = cur.cost + e.cost;
      if (chmin(mincost[e.to], new_cost)) {
        que.push({new_cost, e.to});
      }
    }
  }
  return mincost;
}

vector<vector<i64>> grid_search(const vector<string> &g, int source_r,
                                int source_c) {
  const int H = g.size();
  const int W = g[0].size();
  auto pack = [&](int r, int c) -> int { return r * W + c; };
  auto unpack = [&](int node) -> pair<int, int> {
    return {node / W, node % W};
  };
  array<int, 4> dx = {0, 0, 1, -1}, dy = {1, -1, 0, 0};
  vector mincost(H, vector(W, INF));
  mincost[source_r][source_c] = 0LL;
  int source_node = pack(source_r, source_c);
  queue<State> que;
  que.push({0LL, source_node});
  while (not que.empty()) {
    State cur = move(que.front());
    que.pop();
    auto [cur_r, cur_c] = unpack(cur.node);
    if (cur.cost > mincost[cur_r][cur_c]) continue;
    for (int i = 0; i < 4; ++i) {
      int nr = cur_r + dx[i];
      int nc = cur_c + dy[i];
      if (nr < 0 or nr >= H or nc < 0 or nc >= W) continue;
      if (g[nr][nc] == '#') continue;
      int new_node = pack(nr, nc);
      i64 new_cost = cur.cost + 1;
      if (chmin(mincost[nr][nc], new_cost)) {
        que.push({new_cost, new_node});
      }
    }
  }
  return mincost;
}
