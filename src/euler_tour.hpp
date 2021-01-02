#include <vector>
using namespace std;

// tour: preorder node ids
// The interval [pre_index[v], post_index[v]) represents a subtree whose
// root is v.
struct EulerTour {
  using G = vector<vector<int>>;

  const int n;  // number of nodes
  G adj;

  // Euler Tour on nodes.
  vector<int> tour;        // nodes
  vector<int> pre_index;   // index in the tour on entering the subtree of v
  vector<int> post_index;  // index in the tour on exiting the subtree of v

  explicit EulerTour(G g, int root = 0)
      : n((int)g.size()), adj(move(g)), pre_index(n, -1), post_index(n, -1) {
    tour.reserve(n);
    dfs(root, -1);
  }

 private:
  void dfs(int v, int p) {
    pre_index[v] = int(tour.size());
    tour.push_back(v);
    for (auto u : adj[v]) {
      if (u == p) continue;
      dfs(u, v);
    }
    post_index[v] = int(tour.size());
  }
};
