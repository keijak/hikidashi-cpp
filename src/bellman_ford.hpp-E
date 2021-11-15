#include <limits>
#include <vector>

using Int = long long;

const Int INF = std::numeric_limits<Int>::max() / 2;

struct Edge {
  Int cost;
  int to;
};

// Finds the shortest path from the start node and detects negative cycle if
// exists. Returns the minimum cost from the start node to each node. INF
// indicates unreachable. -INF indicates having a negative cycle in a path from
// the start node.
auto bellman_ford(const std::vector<std::vector<Edge>>& adj, const int start) {
  const int n = int(adj.size());
  std::vector<Int> mincost(n, INF);
  mincost[start] = 0;

  for (int k = 0; k < n - 1; ++k) {
    for (int i = 0; i < n; ++i) {
      const Int di = mincost[i];
      if (di == INF) continue;  // Haven't reached i yet.
      for (const Edge& e : adj[i]) {
        if (mincost[e.to] > di + e.cost) {
          mincost[e.to] = di + e.cost;
        }
      }
    }
  }

  // Negative cycle detection.
  // If there's no negative cycle, at least one node gets the shortest
  // distance determined in each iteration above. If we have gone through N-1
  // iterations and still have an update, there must be a negative cycle.
  for (int k = 0; k < n; ++k) {
    bool updated = false;
    for (int i = 0; i < n; ++i) {
      const Int di = mincost[i];
      if (di == INF) continue;
      const bool in_negative_cycle = (di == -INF);
      for (const Edge& e : adj[i]) {
        if (mincost[e.to] == -INF) continue;
        if (in_negative_cycle or (mincost[e.to] > di + e.cost)) {
          mincost[e.to] = -INF;
          updated = true;
        }
      }
    }
    if (not updated) break;
  }

  return mincost;
}
