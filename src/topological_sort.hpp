#include <bits/stdc++.h>
using namespace std;

// Returns the topogically sorted list of nodes.
// Nodes with zero indegree come first.
// Returns nullopt if the input graph is not a DAG.
// Args:
//   g: directed adjacency list.
//     g[a] = {b, c, d};  // => {a->b, a->c, a->d}
optional<vector<int>> topological_sort(const vector<vector<int>> &g) {
  const int n = g.size();
  vector<int> indeg(n);
  for (int v = 0; v < n; ++v) {
    for (auto u : g[v]) ++indeg[u];
  }
  deque<int> q;
  for (int v = 0; v < n; ++v) {
    if (indeg[v] == 0) {
      q.emplace_back(v);
    }
  }
  vector<int> res;
  while (not q.empty()) {
    auto v = q.front();
    q.pop_front();
    res.push_back(v);
    for (auto u : g[v]) {
      if (--indeg[u] == 0) {
        q.push_back(u);
      }
    }
  }
  if (int(res.size()) != n) {
    return nullopt;  // not a DAG
  }
  return res;
}
