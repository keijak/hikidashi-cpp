#include <bits/stdc++.h>
using i64 = long long;

struct Edge {
  int to;
  i64 len;
};

template <typename Task>
class Rerooting {
 private:
  using NV = typename Task::NodeValue;
  using EV = typename Task::EdgeValue;

  Task task;
  int n;                             // number of nodes
  std::vector<std::vector<Edge>> g;  // graph (tree)
  std::vector<NV> sub;               // values for each subtree rooted at i
  std::vector<NV> full;              // values for each entire tree rooted at i
  int base_root;                     // base root node where we start DFS

 public:
  explicit Rerooting(Task task, std::vector<std::vector<Edge>> g, int r = 0)
      : task(move(task)),
        n((int)g.size()),
        g(move(g)),
        sub(n),
        full(n),
        base_root(r) {}

  const std::vector<NV> &run() {
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
      auto sub = pull_up(u, v);
      res = task.merge(res, task.add_edge(std::move(sub), e));
    }
    return (sub[v] = task.add_node(res, v));
  }

  void push_down(int v, int par, NV upper_sub) {
    int m = g[v].size();
    std::vector<EV> cuml(m + 1, task.id()), cumr(m + 1, task.id());

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
      push_down(u, v, std::move(next_upper_sub));
    }
  }
};

template <typename Task>
class InplaceRerooting {
 private:
  using NV = typename Task::NodeValue;
  using EV = typename Task::EdgeValue;

  Task task;
  int n;                             // number of nodes
  std::vector<std::vector<Edge>> g;  // graph (tree)
  std::vector<NV> sub;               // values for each subtree rooted at i
  std::vector<NV> full;              // values for each entire tree rooted at i
  int base_root;                     // base root node where we start DFS

 public:
  explicit InplaceRerooting(Task task, std::vector<std::vector<Edge>> g,
                            int r = 0)
      : task(move(task)),
        n((int)g.size()),
        g(move(g)),
        sub(n),
        full(n),
        base_root(r) {}

  const std::vector<NV> &run() {
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
      auto sub = task.add_edge(pull_up(u, v), e);
      task.merge_inplace(res, std::move(sub));
    }
    return (sub[v] = task.add_node(res, v));
  }

  void push_down(int v, int par, NV upper_sub) {
    int m = g[v].size();
    EV agg = task.id();

    for (int i = 0; i < m; ++i) {
      auto &e = g[v][i];
      int u = e.to;
      if (u == par) {
        task.merge_inplace(agg, task.add_edge(upper_sub, e));
      } else {
        task.merge_inplace(agg, task.add_edge(sub[u], e));
      }
    }
    full[v] = task.add_node(agg, v);

    for (int i = 0; i < m; ++i) {
      auto &e = g[v][i];
      int u = e.to;
      if (u == par) continue;
      EV edge_value = task.add_edge(sub[u], e);
      task.subtract_inplace(agg, edge_value);
      NV next_upper_sub = task.add_node(agg, v);
      push_down(u, v, std::move(next_upper_sub));
      task.merge_inplace(agg, std::move(edge_value));
    }
  }
};

// https://atcoder.jp/contests/abc201/tasks/abc201_e
struct InplaceTask {
  static constexpr int M = 60;
  using Mint = double;
  using Table = std::array<Mint, M * 2>;  // copy is expensive
  using NodeValue = Table;
  using EdgeValue = Table;

  EdgeValue id() const { return Table{0}; }

  NodeValue add_node(EdgeValue val, int node) const { return std::move(val); }

  EdgeValue add_edge(const NodeValue &val, const Edge &edge) const {
    auto res = Table{};
    for (int i = 0; i < M; ++i) {
      if (edge.len >> i & 1) {
        res[2 * i] = val[i * 2 + 1];
        res[2 * i + 1] = val[i * 2] + 1;
      } else {
        res[i * 2] = val[i * 2] + 1;
        res[i * 2 + 1] = val[i * 2 + 1];
      }
    }
    return res;
  }

  void merge_inplace(EdgeValue &agg, const EdgeValue &x) const {
    for (int i = 0; i < M * 2; ++i) {
      agg[i] += x[i];
    }
  }

  void subtract_inplace(EdgeValue &agg, const EdgeValue &x) const {
    for (int i = 0; i < M * 2; ++i) {
      agg[i] -= x[i];
    }
  }
};
