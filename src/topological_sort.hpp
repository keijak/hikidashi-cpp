vector<int> topological_sort(const vector<vector<int>>& g, int n) {
  vector<int> indeg(n);
  REP(i, N) {
    for (auto j : g[i]) {
      indeg[j]++;
    }
  }

  deque<int> q;
  REP(i, n) {
    if (indeg[i] == 0) {
      q.emplace_back(i);
    }
  }

  vector<int> res;
  while (q.size()) {
    auto v = q.front();
    q.pop_front();
    res.push_back(v);
    for (auto u : G[v]) {
      if (--indeg[u] == 0) {
        q.push_back(u);
      }
    }
  }
  return res;
}
