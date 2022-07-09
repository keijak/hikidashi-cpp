#include <bits/stdc++.h>

#include <atcoder/maxflow>

// T: flow capacity type (int or long long)
template <typename T>
struct ProjectSelection {
 public:
  // For each node v, make sure:
  // - sum(cost(*, v)) < kBigCost
  // - sum(cost(v, *)) < kBigCost
  static constexpr T kBigCost = std::numeric_limits<T>::max() / 10;

  // Assign true to kSource and false to kSink.
  const int kSource, kSink;
  const int n_;  // number of initial nodes
  int m_;        // number of additional nodes

  struct Edge {
    int from;
    int to;
    T cost;
  };
  std::vector<Edge> edges_;

  T bonus_;

  std::optional<std::vector<bool>> assignment_;

  explicit ProjectSelection(int n)
      : kSource(n), kSink(n + 1), n_(n), m_(0), bonus_(0) {}

  // Returns the minimum cost.
  T min_cost() {
    atcoder::mf_graph<T> g(n_ + m_ + 2);
    for (const Edge &e : edges_) {
      g.add_edge(e.from, e.to, e.cost);
    }
    T res = g.flow(kSource, kSink) - bonus_;
    // Get the assignment.
    // assignment_ = g.min_cut(kSource);
    return res;
  }

  // Returns the maximum gain.
  T max_gain() { return -min_cost(); }

  // === if-then API ===

  // {X=1, Y=0} => cost
  void if_X_and_notY_then_cost(int x, int y, T cost) {
    if (cost == 0) return;
    assert(cost > 0);
    edges_.push_back(Edge{x, y, cost});
  }

  // {X=1} => cost
  void if_X_then_cost(int x, T cost) {
    if (cost < 0) return if_X_then_gain(x, -cost);
    if_X_and_notY_then_cost(x, kSink, cost);
  }

  // {X=0} => cost
  void if_notX_then_cost(int x, T cost) {
    if (cost < 0) return if_notX_then_gain(x, -cost);
    if_X_and_notY_then_cost(kSource, x, cost);
  }

  // {X=1} => gain
  void if_X_then_gain(int x, T gain) {
    if (gain < 0) return if_X_then_cost(x, -gain);
    bonus_ += gain;
    if_notX_then_cost(x, gain);
  }

  // {X=0} => gain
  void if_notX_then_gain(int x, T gain) {
    if (gain < 0) return if_notX_then_cost(x, -gain);
    bonus_ += gain;
    if_X_then_cost(x, gain);
  }

  // {X=0,Y=1} or {X=1,Y=0} => cost
  void if_X_not_equal_Y_then_cost(int x, int y, T cost) {
    if_X_and_notY_then_cost(x, y, cost);
    if_X_and_notY_then_cost(y, x, cost);
  }

  // {X=0,Y=0} or {X=1,Y=1} => gain
  void if_X_equal_Y_then_gain(int x, int y, T gain) {
    bonus_ += gain;
    if_X_and_notY_then_cost(x, y, gain);
    if_X_and_notY_then_cost(y, x, gain);
  }

  // {X[i]=1 forall i} => gain
  void if_all_of_X_then_gain(const std::vector<int> &xs, T gain) {
    const int y = n_ + 2 + m_++;
    bonus_ += gain;
    if_X_and_notY_then_cost(kSource, y, gain);
    for (const auto &x : xs) {
      if_X_and_notY_then_cost(y, x, kBigCost);
    }
  }

  // {X[i]=0 forall i} => gain
  void if_none_of_X_then_gain(const std::vector<int> &xs, T gain) {
    const int y = n_ + 2 + m_++;
    bonus_ += gain;
    if_X_and_notY_then_cost(y, kSink, gain);
    for (const auto &x : xs) {
      if_X_and_notY_then_cost(x, y, kBigCost);
    }
  }

  // === constraint API ===

  void constraint(int x, int y, T cost) {
    if (x >= 0 and y < 0) {
      assert(cost >= 0);
      if_X_and_notY_then_cost(x, ~y, cost);
    } else if (x < 0 and y >= 0) {
      assert(cost >= 0);
      if_X_and_notY_then_cost(y, ~x, cost);
    } else if (x >= 0 and y >= 0) {
      assert(cost <= 0);
      if_all_of_X_then_gain({x, y}, -cost);
    } else if (x < 0 and y < 0) {
      assert(cost <= 0);
      if_none_of_X_then_gain({~x, ~y}, -cost);
    } else {
      assert(false);
    }
  }

  void constraint(int x, T cost) {
    if (x >= 0) {
      if_X_then_cost(x, cost);
    } else {
      if_notX_then_cost(~x, cost);
    }
  }
};
