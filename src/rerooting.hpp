#include <bits/stdc++.h>
using i64 = long long;

struct Edge {
  int to;
  i64 len;
};

template <typename Task>
class RerootingDP {
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
  explicit RerootingDP(Task task, std::vector<std::vector<Edge>> g, int r = 0)
      : task(move(task)),
        n((int)g.size()),
        g(move(g)),
        sub(n),
        full(n),
        base_root(r) {}

  const std::vector<NV> &run() {
    pull_up(base_root, -1);
    push_down(base_root, -1, std::nullopt);
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

  void push_down(int v, int par, std::optional<NV> upper_sub) {
    int m = g[v].size();
    std::vector<EV> cuml(m + 1, task.id()), cumr(m + 1, task.id());

    for (int i = 0; i < m; ++i) {
      auto &e = g[v][i];
      int u = e.to;
      if (u == par) {
        assert(upper_sub.has_value());
        cuml[i + 1] = task.merge(cuml[i], task.add_edge(*upper_sub, e));
      } else {
        cuml[i + 1] = task.merge(cuml[i], task.add_edge(sub[u], e));
      }
    }

    for (int i = m - 1; i >= 0; --i) {
      auto &e = g[v][i];
      int u = e.to;
      if (u == par) {
        cumr[i] = task.merge(task.add_edge(*upper_sub, e), cumr[i + 1]);
      } else {
        cumr[i] = task.merge(task.add_edge(sub[u], e), cumr[i + 1]);
      }
    }

    full[v] = task.add_node(cuml[m], v);

    for (int i = 0; i < m; ++i) {
      auto &e = g[v][i];
      int u = e.to;
      if (u == par) continue;
      std::optional<NV> next_upper_sub{
          task.add_node(task.merge(cuml[i], cumr[i + 1]), v)};
      push_down(u, v, std::move(next_upper_sub));
    }
  }
};

template <typename Task>
class InplaceRerooting {
 private:
  using NV = typename Task::NodeValue;
  using EV = typename Task::EdgeValue;

  Task task_;
  int n_;                             // number of nodes
  std::vector<std::vector<Edge>> g_;  // graph (tree)
  std::vector<NV> sub_result_;        // values for each subtree rooted at i
  std::vector<NV> full_result_;       // values for each entire tree rooted at i
  int base_root_;                     // base root node where we start DFS

 public:
  explicit InplaceRerooting(Task task, std::vector<std::vector<Edge>> g,
                            int r = 0)
      : task_(move(task)),
        n_((int)g.size()),
        g_(move(g)),
        sub_result_(n_),
        full_result_(n_),
        base_root_(r) {}

  const std::vector<NV> &run() {
    pull_up(base_root_, -1);
    push_down(base_root_, -1, std::nullopt);
    return full_result_;
  }

 private:
  NV pull_up(int v, int par) {
    EV res = task_.id();
    for (auto &e : g_[v]) {
      int u = e.to;
      if (u == par) continue;
      auto sub = task_.add_edge(pull_up(u, v), e);
      task_.merge_inplace(res, std::move(sub));
    }
    return (sub_result_[v] = task_.add_node(res, v));
  }

  void push_down(int v, int par, std::optional<NV> upper_sub) {
    int m = g_[v].size();
    EV agg = task_.id();

    for (int i = 0; i < m; ++i) {
      auto &e = g_[v][i];
      int u = e.to;
      if (u == par) {
        assert(upper_sub.has_value());
        task_.merge_inplace(agg, task_.add_edge(std::move(*upper_sub), e));
      } else {
        task_.merge_inplace(agg, task_.add_edge(sub_result_[u], e));
      }
    }
    full_result_[v] = task_.add_node(agg, v);

    for (int i = 0; i < m; ++i) {
      auto &e = g_[v][i];
      int u = e.to;
      if (u == par) continue;
      EV edge_value = task_.add_edge(sub_result_[u], e);
      task_.subtract_inplace(agg, edge_value);
      std::optional<NV> next_upper_sub{task_.add_node(agg, v)};
      push_down(u, v, std::move(next_upper_sub));
      task_.merge_inplace(agg, std::move(edge_value));
    }
  }
};

// https://atcoder.jp/contests/abc220/tasks/abc220_f
struct DistanceSums2Task {
  struct T {
    i64 node_count;
    i64 edge_count;
    T(i64 nc = 0, i64 ec = 0) : node_count(nc), edge_count(ec) {}
  };
  using NodeValue = T;
  using EdgeValue = T;

  EdgeValue id() const { return {0, 0}; }

  NodeValue add_node(EdgeValue val, int node) const {
    val.node_count += 1;
    return val;
  }

  EdgeValue add_edge(const NodeValue &val, const Edge &edge) const {
    EdgeValue res = val;
    res.edge_count += val.node_count;
    return res;
  }

  void merge_inplace(EdgeValue &agg, const EdgeValue &x) const {
    agg.node_count += x.node_count;
    agg.edge_count += x.edge_count;
  }

  void subtract_inplace(EdgeValue &agg, const EdgeValue &x) const {
    agg.node_count -= x.node_count;
    agg.edge_count -= x.edge_count;
  }
};

// https://atcoder.jp/contests/abc201/tasks/abc201_e
struct XorDistancesTask {
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
