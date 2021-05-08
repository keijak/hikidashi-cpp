#include <bits/stdc++.h>

#include <atcoder/maxflow>

const int BIG_COST = 1e8;

// T: flow capacity type (i32/i64)
template <typename T>
struct ProjectSelection {
 private:
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

 public:
  explicit ProjectSelection(int n)
      : kSource(n), kSink(n + 1), n_(n), m_(0), bonus_(0) {}

  // Returns the minimum cost.
  T min_cost() const {
    atcoder::mf_graph<T> g(n_ + m_ + 2);
    for (const Edge &e : edges_) {
      g.add_edge(e.from, e.to, e.cost);
    }
    return g.flow(kSource, kSink) - bonus_;
  }

  // Returns the maximum gain.
  T max_gain() const { return -min_cost(); }

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
      if (cost >= 0) {
        if_X_then_cost(x, cost);
      } else {
        if_X_then_gain(x, -cost);
      }
    } else {
      if (cost >= 0) {
        if_notX_then_cost(~x, cost);
      } else {
        if_notX_then_gain(~x, cost);
      }
    }
  }

  // === if-then API ===

  // {X=1, Y=0} => cost
  void if_X_and_notY_then_cost(int x, int y, T cost) {
    edges_.push_back(Edge{x, y, cost});
  }

  // {X=1} => cost
  void if_X_then_cost(int x, T cost) {
    if_X_and_notY_then_cost(x, kSink, cost);
  }

  // {X=0} => cost
  void if_notX_then_cost(int x, T cost) {
    if_X_and_notY_then_cost(kSource, x, cost);
  }

  // {X=1} => gain
  void if_X_then_gain(int x, T gain) {
    bonus_ += gain;
    if_notX_then_cost(x, gain);
  }

  // {X=0} => gain
  void if_notX_then_gain(int x, T gain) {
    bonus_ += gain;
    if_X_then_cost(x, gain);
  }

  // {Xi=1 forall i} => gain
  void if_all_of_X_then_gain(const std::vector<int> &xs, T gain) {
    const int y = n_ + 2 + m_++;
    bonus_ += gain;
    if_X_and_notY_then_cost(kSource, y, gain);
    for (const auto &x : xs) {
      if_X_and_notY_then_cost(y, x, BIG_COST);
    }
  }

  // {Xi=0 forall i} => gain
  void if_none_of_X_then_gain(const std::vector<int> &xs, T gain) {
    const int y = n_ + 2 + m_++;
    bonus_ += gain;
    if_X_and_notY_then_cost(y, kSink, gain);
    for (const auto &x : xs) {
      if_X_and_notY_then_cost(x, y, BIG_COST);
    }
  }

  // {X=0,Y=0} or {X=1,Y=1} => gain
  void if_X_equal_Y_then_gain(int x, int y, T gain) {
    if_all_of_X_then_gain({x, y}, gain);
    if_none_of_X_then_gain({x, y}, gain);
  }

  // {X=0,Y=1} or {X=1,Y=0} => cost
  void if_X_not_equal_Y_then_cost(int x, int y, T cost) {
    if_X_and_notY_then_cost(x, y, cost);
    if_X_and_notY_then_cost(y, x, cost);
  }
};
