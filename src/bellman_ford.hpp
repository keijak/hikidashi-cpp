#include <unordered_set>
#include <vector>

using i64 = long long;

const i64 INF = 1e18;

// Finds the shortest path from the start node and detects negative cycle.
// Returns the distance from the start node to each node.
// INF indicates unreachable. -INF indicates having a negative cycle in a
// path from the start node.
std::vector<i64> bellman_ford(
    const std::vector<std::vector<std::pair<int, i64>>> &adj, const int start) {
  const int n = int(adj.size());
  const i64 INF = 1e18;
  std::vector<i64> min_dist(n, INF);  // distance from the start node.
  min_dist[start] = 0;

  for (int k = 0; k < n - 1; ++k) {
    for (int i = 0; i < n; ++i) {
      const auto di = min_dist[i];
      if (di == INF) continue;  // We haven't reached i yet.
      for (auto [j, dist] : adj[i]) {
        if (min_dist[j] > di + dist) {
          min_dist[j] = di + dist;
        }
      }
    }
  }

  // Negative cycle detection.
  // When there's no negative cycle, at least one node gets the shortest
  // distance determined in each iteration. If we have gone through N+1
  // iterations and still have an update, there must be a negative cycle.
  for (int k = 0; k < n; ++k) {
    for (int i = 0; i < n; ++i) {
      if (min_dist[i] == INF) continue;
      const bool negative_cycle = (min_dist[i] == -INF);
      for (auto [j, dist] : adj[i]) {
        if (negative_cycle or min_dist[j] > min_dist[i] + dist) {
          min_dist[j] = -INF;
        }
      }
    }
  }

  return min_dist;
}
