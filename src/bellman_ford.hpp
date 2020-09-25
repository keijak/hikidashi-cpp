#include <utility>
#include <vector>

using i64 = long long;
const i64 INF = 1LL << 40;

// Find shortest path from a start node.
// Allows edges with negative cost.
pair<vector<i64>, bool> bellman_ford(
    const int start, const vector<vector<pair<int, i64>>>& adj) {
  const int N = static_cast<int>(adj.size());
  vector<i64> dist(N, INF);  // distance from the start node.
  dist[start] = 0;

  for (int k = 0;; ++k) {
    bool update = false;
    REP(i, N) {
      if (dist[i] == INF) {
        continue;  // We haven't reached i yet.
      }
      for (const pair<int, i64>& edge : adj[i]) {
        int j = edge.first;  // from i to j
        i64 cost = edge.second;
        if (dist[j] > dist[i] + cost) {
          dist[j] = dist[i] + cost;
          update = true;
        }
      }
    }
    if (!update) {
      return {std::move(dist), false};
    }
    // Negative cycle detection.
    // When there's no negative cycle, at least one node gets the shortest
    // distance determined for each iteration. If we have gone through N+1
    // iterations and still have an update, there must be a negative cycle.
    if (k == N) {
      return {std::move(dist), true};
    }
  }
  // Never reach here.
}
