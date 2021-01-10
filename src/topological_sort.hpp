// Returns the topogically sorted list of nodes.
// Nodes with zero indegree come first.
// The input graph must be a DAG. Otherwise try SCC decomposition.
vector<int> topological_sort(const vector<vector<int>> &g) {
  const int n = g.size();
  vector<int> indeg(n);
  for (int v = 0; v < n; ++v) {
    for (auto u : g[v]) ++indeg[u];
  }
  deque<int> q;
  for (int v = 0; v < n; ++v) {
    if (indeg[v] == 0) q.emplace_back(v);
  }
  vector<int> res;
  while (not q.empty()) {
    auto v = q.front();
    q.pop_front();
    res.push_back(v);
    for (auto u : g[v]) {
      if (--indeg[u] == 0) q.push_back(u);
    }
  }
  assert(int(res.size()) == n);  // Otherwise g is not a DAG.
  return res;
}
