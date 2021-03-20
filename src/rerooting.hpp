struct Edge {
  int to;
  i64 len;
};

template <typename Task>
class ReRooting {
  using T = typename Task::T;

 public:
  int n;                   // number of nodes
  vector<vector<Edge>> g;  // graph (tree)
  vector<T> sub;           // values for each subtree rooted at i
  vector<T> full;          // values for each entire tree rooted at i
  int base_root;           // base root node where we start DFS

  explicit ReRooting(vector<vector<Edge>> g, int r = 0)
      : n((int)g.size()), g(move(g)), sub(n), full(n), base_root(r) {}

  const vector<T> &run() {
    pull_up(base_root, -1);
    push_down(base_root, -1, Task::id());
    return full;
  }

 private:
  T pull_up(int v, int par) {
    T res = Task::id();
    for (auto &e : g[v]) {
      if (e.to == par) continue;
      i64 sub = pull_up(e.to, v);
      res = Task::op(res, Task::add_edge(sub, e));
    }
    return (sub[v] = Task::add_node(res, v));
  }

  void push_down(int v, int par, T upper_sub) {
    int m = g[v].size();
    vector<T> cuml(m + 1, Task::id()), cumr(m + 1, Task::id());

    for (int i = 0; i < m; ++i) {
      auto &e = g[v][i];
      if (e.to == par) {
        cuml[i + 1] = Task::op(cuml[i], Task::add_edge(upper_sub, e));
      } else {
        cuml[i + 1] = Task::op(cuml[i], Task::add_edge(sub[e.to], e));
      }
    }

    for (int i = m - 1; i >= 0; --i) {
      auto &e = g[v][i];
      if (e.to == par) {
        cumr[i] = Task::op(Task::add_edge(upper_sub, e), cumr[i + 1]);
      } else {
        cumr[i] = Task::op(Task::add_edge(sub[e.to], e), cumr[i + 1]);
      }
    }

    full[v] = Task::add_node(cuml[m], v);

    for (int i = 0; i < m; ++i) {
      auto &e = g[v][i];
      if (e.to == par) continue;
      T next_upper_sub = Task::add_node(Task::op(cuml[i], cumr[i + 1]), v);
      push_down(e.to, v, next_upper_sub);
    }
  }
};

// Examples

struct EDPC_V {
  using T = Mint;

  static T op(const T &x, const T &y) { return x * y; }

  static T id() { return 1; }

  static T add_node(T val, int node) { return val + 1; }
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
  using T = i64;

  static T id() { return 0LL; }

  static T op(const T &x, const T &y) { return std::max(x, y); }

  static T add_node(T val, int node) { return val + w[node]; }

  static T add_edge(T val, const Edge &edge) {
    return std::max(val - edge.len, 0LL);
  }
};
