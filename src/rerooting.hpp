
template <typename Monoid>
struct ReRooting {
  using T = typename Monoid::T;

  int n;                  // number of nodes
  vector<vector<int>> g;  // graph (tree)
  vector<T> sub;          // values for each subtree rooted at i.
  vector<T> full;         // values for each entire tree rooted at i
  int base_root;          // base root node where we start DFS

  explicit ReRooting(vector<vector<int>> g, int r = 0)
      : n((int)g.size()), g(move(g)), sub(n), full(n), base_root(r) {}

  const vector<T> &run() {
    pull_up(base_root, -1);
    push_down(base_root, -1, Monoid::id());
    return full;
  }

 private:
  T pull_up(int v, int par) {
    T res = Monoid::id();
    for (int u : g[v]) {
      if (u == par) continue;
      res = Monoid::op(res, pull_up(u, v));
    }
    sub[v] = Monoid::build(res);
    return sub[v];
  }

  void push_down(int v, int par, T upper_sub) {
    int m = g[v].size();
    vector<T> cuml(m + 1, Monoid::id()), cumr(m + 1, Monoid::id());

    for (int i = 0; i < m; ++i) {
      int u = g[v][i];
      if (u == par) {
        cuml[i + 1] = Monoid::op(cuml[i], upper_sub);
      } else {
        cuml[i + 1] = Monoid::op(cuml[i], sub[u]);
      }
    }

    for (int i = m - 1; i >= 0; --i) {
      int u = g[v][i];
      if (u == par) {
        cumr[i] = Monoid::op(upper_sub, cumr[i + 1]);
      } else {
        cumr[i] = Monoid::op(sub[u], cumr[i + 1]);
      }
    }

    full[v] = Monoid::build(cuml[m]);

    for (int i = 0; i < m; ++i) {
      int u = g[v][i];
      if (u == par) continue;
      T next_upper_sub = Monoid::build(Monoid::op(cuml[i], cumr[i + 1]));
      push_down(u, v, move(next_upper_sub));
    }
  }
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

  static T build(T val) { return {val.size + 1, val.value}; }
};
