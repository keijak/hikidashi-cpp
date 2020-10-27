#include <unordered_set>
#include <vector>

using i64 = long long;

// Find the shortest path from the start node and detects negative cycle.
// The first return value indicates the distance from the start node.
// The second return value indicates which nodes have a negative cycle in a
// path from the start node.
std::pair<std::vector<i64>, std::vector<bool>> bellman_ford(
    const std::vector<std::vector<std::pair<int, i64>>> &adj, const int start) {
  const int n = int(adj.size());
  const i64 INF = 1e18;
  std::vector<i64> min_dist(n, INF);  // distance from the start node.
  min_dist[start] = 0;
  std::vector<bool> dist_inf(n, false);  // nodes in negative infinity distance.

  for (int k = 0;; ++k) {
    std::unordered_set<int> updated;
    for (int i = 0; i < n; ++i) {
      const auto di = min_dist[i];
      if (di == INF) continue;  // We haven't reached i yet.
      for (auto [j, dist] : adj[i]) {
        if (dist_inf[j]) continue;
        if (min_dist[j] > di + dist) {
          min_dist[j] = di + dist;
          updated.insert(j);
        }
      }
    }

    if (updated.empty()) {
      return {std::move(min_dist), std::move(dist_inf)};
    }

    // Negative cycle detection
    if (k >= n) {
      // When there's no negative cycle, at least one node gets the shortest
      // distance determined in each iteration. If we have gone through N+1
      // iterations and still have an update, there must be a negative cycle.
      for (auto u : updated) {
        dist_inf[u] = true;
      }
      updated.clear();
    }
  }
  assert(false);  // Never reach here.
}
