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
auto search(const vector<vector<Edge>> &g, int source) {
  const int n = g.size();
  vector mincost(n, INF);
  MinHeap<State> que;
  auto push = [&](i64 cost, int node) -> bool {
    if (chmin(mincost[node], cost)) {
      que.push({cost, node});
      return true;
    }
    return false;
  };
  assert(push(0LL, source));

  while (not que.empty()) {
    State cur = move(que.top());
    que.pop();
    if (cur.cost != mincost[cur.node]) {
      continue;
    }
    for (const auto &e : g[cur.node]) {
      i64 new_cost = cur.cost + e.cost;
      push(new_cost, e.to);
    }
  }
  return mincost;
}

struct GridState {
  i64 cost;
  int r;
  int c;
};
bool operator>(const GridState &x, const GridState &y) {
  return x.cost > y.cost;
}

auto grid_search(const vector<string> &g, int source_r, int source_c) {
  const int H = g.size();
  const int W = g[0].size();
  array<int, 4> dx = {0, 0, 1, -1}, dy = {1, -1, 0, 0};
  vector mincost(H, vector(W, INF));
  queue<GridState> que;
  auto push = [&](i64 cost, int r, int c) -> bool {
    if (r < 0 or r >= H or c < 0 or c >= W) return false;
    if (g[r][c] == '#') return false;
    if (not chmin(mincost[r][c], cost)) return false;
    que.push({cost, r, c});
    return true;
  };
  assert(push(0LL, source_r, source_c));

  while (not que.empty()) {
    State cur = move(que.front());
    que.pop();
    if (cur.cost != mincost[cur.r][cur.c]) {
      continue;
    }
    for (int i = 0; i < 4; ++i) {
      int nr = cur.r + dx[i];
      int nc = cur.c + dy[i];
      i64 new_cost = cur.cost + 1;
      push(new_cost, nr, nc);
    }
  }
  return mincost;
}
