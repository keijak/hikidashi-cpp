#include <bits/stdc++.h>
using namespace std;

// Returns the topogically sorted list of nodes.
//
// Assuming edges are directed from "smaller" nodes to "larger" nodes,
// returns a list of nodes sorted in the small-to-large order.
//
// nullopt is returned if the input graph is not a DAG.
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
