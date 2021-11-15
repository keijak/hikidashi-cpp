// Simple Graph
struct Graph {
  int n;
  V<V<int>> adj;
  explicit Graph(int n) : n(n), adj(n) {}
  void input_undirected(int m) { input(m, false); }
  void input_directed(int m) { input(m, true); }

 private:
  void input(int m, bool is_directed) {
    for (int i = 0; i < m; ++i) {
      int u, v;  // read as 1-indexed.
      cin >> u >> v;
      u--, v--;  // to 0-indexed.
      adj[u].push_back(v);
      if (not is_directed) adj[v].push_back(u);
    }
  }
};

struct Edge {
  int to;
  long long weight;
  explicit Edge(int to, long long weight) : to(to), weight(weight) {}
  bool operator<(const Edge& e) const {
    if (weight != e.weight) return weight < e.weight;
    return to < e.to;
  }
};

// Weighted Graph
struct WGraph {
  int n;
  V<V<Edge>> adj;
  explicit WGraph(int n) : n(n), adj(n) {}
  void input_undirected(int m) { input(m, false); }
  void input_directed(int m) { input(m, true); }

 private:
  void input(int m, bool is_directed) {
    for (int i = 0; i < m; ++i) {
      int u, v;  // read as 1-indexed.
      long long w;
      cin >> u >> v >> w;
      u--, v--;  // to 0-indexed.
      adj[u].emplace_back(v, w);
      if (not is_directed) adj[v].emplace_back(u, w);
    }
  }
};
