#include <bits/stdc++.h>

#include <atcoder/maxflow>

const int BIG_COST = 1e8;

template <typename T>
struct ProjectSelection {
 private:
  // Assign Color 1 to kSource and Color 0 to kSink.
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

  T min_cost() const {
    atcoder::mf_graph<T> g(n_ + m_ + 2);
    for (const Edge &e : edges_) {
      g.add_edge(e.from, e.to, e.cost);
    }
    return g.flow(kSource, kSink) - bonus_;
  }

  // === High-level constraint API ===

  void constraint(int x, int y, T cost) {
    if (x >= 0 and y < 0) {
      assert(cost >= 0);
      X_and_notY_lose(x, ~y, cost);
    } else if (x < 0 and y >= 0) {
      assert(cost >= 0);
      X_and_notY_lose(y, ~x, cost);
    } else if (x >= 0 and y >= 0) {
      assert(cost <= 0);
      all_of_X_gain({x, y}, -cost);
    } else if (x < 0 and y < 0) {
      assert(cost <= 0);
      none_of_X_gain({~x, ~y}, -cost);
    } else {
      assert(false);
    }
  }

  void constraint(int x, T cost) {
    if (x >= 0) {
      if (cost >= 0) {
        X_lose(x, cost);
      } else {
        X_gain(x, -cost);
      }
    } else {
      if (cost >= 0) {
        notX_lose(~x, cost);
      } else {
        notX_gain(~x, cost);
      }
    }
  }

  // === Low-level constraint API ===

  // {X=1, Y=0} => penalty
  void X_and_notY_lose(int x, int y, T penalty) {
    edges_.push_back(Edge{x, y, penalty});
  }

  // {X=1} => penalty
  void X_lose(int x, T penalty) { X_and_notY_lose(x, kSink, penalty); }

  // {X=0} => penalty
  void notX_lose(int x, T penalty) { X_and_notY_lose(kSource, x, penalty); }

  // {X=1} => revenue
  void X_gain(int x, T revenue) {
    bonus_ += revenue;
    notX_lose(x, revenue);
  }

  // {X=0} => revenue
  void notX_gain(int x, T revenue) {
    bonus_ += revenue;
    X_lose(x, revenue);
  }

  // {Xi=1 forall i} => revenue
  void all_of_X_gain(std::vector<int> x, T revenue) {
    const int y = n_ + 2 + m_++;
    bonus_ += revenue;
    X_and_notY_lose(kSource, y, revenue);
    for (const auto &v : x) {
      X_and_notY_lose(y, v, BIG_COST);
    }
  }

  // {Xi=0 forall i} => revenue
  void none_of_X_gain(std::vector<int> x, T revenue) {
    const int y = n_ + 2 + m_++;
    bonus_ += revenue;
    X_and_notY_lose(y, kSink, revenue);
    for (const auto &v : x) {
      X_and_notY_lose(v, y, BIG_COST);
    }
  }

  // {X=0,Y=0} or {X=1,Y=1} => revenue
  void X_equal_Y_gain(int x, int y, T revenue) {
    all_of_X_gain({x, y}, revenue);
    none_of_X_gain({x, y}, revenue);
  }

  // {X=0,Y=1} or {X=1,Y=0} => penalty
  void X_not_equal_Y_lose(int x, int y, T penalty) {
    X_and_notY_lose(x, y, penalty);
    X_and_notY_lose(y, x, penalty);
  }
};
