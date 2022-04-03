#include <bits/stdc++.h>

#include <atcoder/scc>

// Strongly Connected Components.
// A wrapper class of atcoder::scc_graph.
struct SCC {
  template <class T>
  using Vec = std::vector<T>;

  int n;                       // number of vertices in the original graph
  const Vec<Vec<int>> &graph;  // original graph (directed)

  // There may be an edge from components[i] to components[j] if i <= j.
  int scc_n;                // number of SCCs.
  Vec<Vec<int>> scc_graph;  // scc graph (directed)
  Vec<Vec<int>> components;
  Vec<int> component_id;

  explicit SCC(const Vec<Vec<int>> &g) : n(int(g.size())), graph(g) {
    // Compute SCCs.
    atcoder::scc_graph acl_graph(n);
    for (int v = 0; v < n; ++v) {
      for (int u : graph[v]) {
        acl_graph.add_edge(v, u);
      }
    }
    components = acl_graph.scc();
    scc_n = int(components.size());
    component_id.assign(n, -1);
    for (int i = 0; i < scc_n; ++i) {
      for (int v : components[i]) {
        component_id[v] = i;
      }
    }
    // Construct the SCC grpah.
    scc_graph.resize(scc_n);
    for (int i = 0; i < scc_n; ++i) {
      for (int v : components[i]) {
        for (int u : graph[v]) {
          int j = component_id[u];
          if (i != j) scc_graph[i].push_back(j);
        }
      }
    }
    // Uniqify SCC edges.
    for (auto &neighbors : scc_graph) {
      std::sort(neighbors.begin(), neighbors.end());
      neighbors.erase(std::unique(neighbors.begin(), neighbors.end()),
                      neighbors.end());
    }
  }
};
