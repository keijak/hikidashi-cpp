struct Edge {
  int to;
  i64 len;
};

template <typename Task>
class ReRooting {
 private:
  using NV = typename Task::NodeValue;
  using EV = typename Task::EdgeValue;

  Task task;
  int n;                   // number of nodes
  vector<vector<Edge>> g;  // graph (tree)
  vector<NV> sub;          // values for each subtree rooted at i
  vector<NV> full;         // values for each entire tree rooted at i
  int base_root;           // base root node where we start DFS

 public:
  explicit ReRooting(Task task, vector<vector<Edge>> g, int r = 0)
      : task(move(task)),
        n((int)g.size()),
        g(move(g)),
        sub(n),
        full(n),
        base_root(r) {}

  const vector<NV> &run() {
    pull_up(base_root, -1);
    push_down(base_root, -1, task.id());
    return full;
  }

 private:
  NV pull_up(int v, int par) {
    EV res = task.id();
    for (auto &e : g[v]) {
      int u = e.to;
      if (u == par) continue;
      i64 sub = pull_up(u, v);
      res = task.merge(res, task.add_edge(sub, e));
    }
    return (sub[v] = task.add_node(res, v));
  }

  void push_down(int v, int par, NV upper_sub) {
    int m = g[v].size();
    vector<EV> cuml(m + 1, task.id()), cumr(m + 1, task.id());

    for (int i = 0; i < m; ++i) {
      auto &e = g[v][i];
      int u = e.to;
      if (u == par) {
        cuml[i + 1] = task.merge(cuml[i], task.add_edge(upper_sub, e));
      } else {
        cuml[i + 1] = task.merge(cuml[i], task.add_edge(sub[u], e));
      }
    }

    for (int i = m - 1; i >= 0; --i) {
      auto &e = g[v][i];
      int u = e.to;
      if (u == par) {
        cumr[i] = task.merge(task.add_edge(upper_sub, e), cumr[i + 1]);
      } else {
        cumr[i] = task.merge(task.add_edge(sub[u], e), cumr[i + 1]);
      }
    }

    full[v] = task.add_node(cuml[m], v);

    for (int i = 0; i < m; ++i) {
      auto &e = g[v][i];
      int u = e.to;
      if (u == par) continue;
      NV next_upper_sub = task.add_node(task.merge(cuml[i], cumr[i + 1]), v);
      push_down(u, v, next_upper_sub);
    }
  }
};

// Examples

struct EDPC_V {
  using T = Mint;

  static T id() { return 1; }
  static T add_node(T val, int node) { return val + 1; }
  static T op(const T &x, const T &y) { return x * y; }
};

struct ABC160 {
  struct T {
    int size;
    Mint value;
  };

  static T op(const T &x, const T &y) {
    int sz = x.size + y.size;
    Mint val = x.value * y.value * fs.C(sz, y.size);
    return {sz, val};
  }

  static T id() { return {0, 1}; }

  static T add_node(T val, int node) { return {val.size + 1, val.value}; }
};

// https://codeforces.com/contest/1084/problem/D
struct FairNut {
  using NodeValue = i64;
  using EdgeValue = i64;

  vector<i64> w;

  explicit FairNut(vector<i64> w) : w(move(w)) {}

  EdgeValue id() const { return 0LL; }

  NodeValue add_node(EdgeValue val, int node) const { return val + w[node]; }

  EdgeValue add_edge(NodeValue val, const Edge &edge) const {
    return std::max(val - edge.len, 0LL);
  }

  EdgeValue merge(const EdgeValue &x, const EdgeValue &y) const {
    return std::max(x, y);
  }
};
