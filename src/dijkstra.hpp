#include <bits/stdc++.h>
using namespace std;
using Int = long long;
template <typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template <typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;

template <class T>
using MinHeap = std::priority_queue<T, vector<T>, greater<T>>;

struct Edge {
  int to;
  Int cost;
};
using Graph = std::vector<std::vector<Edge>>;

struct State {
  Int cost;
  int node;
};
bool operator>(const State &x, const State &y) { return x.cost > y.cost; }

// Returns min distance from the start node to each node (if exists).
auto search_shortest_path(const Graph &g, int start, int goal) {
  const int n = g.size();
  auto mincost = vector(n, kBigVal<Int>);
  MinHeap<State> que;
  auto push = [&](Int cost, int node) -> bool {
    if (chmin(mincost[node], cost)) {
      que.push(State{cost, node});
      return true;
    }
    return false;
  };
  assert(push(0LL, start));

  while (not que.empty()) {
    State cur = que.top();
    que.pop();
    if (cur.cost != mincost[cur.node]) continue;
    if (cur.node == goal) break;
    for (const auto &e : g[cur.node]) {
      auto new_cost = cur.cost + e.cost;
      push(new_cost, e.to);
    }
  }
  return mincost[goal];
}

struct GridState {
  Int cost;
  int r;
  int c;
};
bool operator>(const GridState &x, const GridState &y) {
  return x.cost > y.cost;
}

auto search_shortest_path_on_grid(const vector<string> &g,
                                  const pair<int, int> &start,
                                  const pair<int, int> &goal) {
  static const int dx[4] = {1, 0, -1, 0};
  static const int dy[4] = {0, 1, 0, -1};
  const int H = g.size();
  const int W = g[0].size();
  auto inside = [&](int i, int j) -> bool {
    return 0 <= i and i < H and 0 <= j and j < W;
  };
  auto mincost = vector(H, vector(W, kBigVal<Int>));
  MinHeap<GridState> que;
  auto push = [&](Int cost, int r, int c) -> bool {
    if (not inside(r, c)) return false;
    if (g[r][c] == '#') return false;
    if (not chmin(mincost[r][c], cost)) return false;
    que.push(GridState{cost, r, c});
    return true;
  };
  assert(push(0LL, start.first, start.second));

  while (not que.empty()) {
    GridState cur = que.top();
    que.pop();
    if (cur.cost != mincost[cur.r][cur.c]) continue;
    if (pair(cur.r, cur.c) == goal) break;
    assert(g[cur.r][cur.c] == '.');
    for (int i = 0; i < 4; ++i) {
      const int nr = cur.r + dx[i];
      const int nc = cur.c + dy[i];
      // Note: If the cost delta is always 0 or 1, BFS should be enough.
      const auto new_cost = cur.cost + 1;
      push(new_cost, nr, nc);
    }
  }
  return mincost[goal.first][goal.second];
}
