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
  vector<int> depth;

  explicit EulerTour(G g, int root = 0)
      : n((int)g.size()),
        adj(move(g)),
        pre_index(n, -1),
        post_index(n, -1),
        depth(n, -1) {
    tour.reserve(n * 2);
    depth[root] = 0;
    dfs(root, -1);
  }

 private:
  void dfs(int v, int p) {
    pre_index[v] = int(tour.size());
    tour.push_back(v);
    if (p >= 0) depth[v] = depth[p] + 1;
    for (auto u : adj[v]) {
      if (u == p) continue;
      dfs(u, v);
      tour.push_back(v);
    }
    post_index[v] = int(tour.size());
  }
};
