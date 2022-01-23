#include <bits/stdc++.h>

// A functional graph is a directed graph in which each vertex has outdegree
// one, and can therefore be specified by a function mapping {0,...,n-1} onto
// itself.
// https://mathworld.wolfram.com/FunctionalGraph.html

// Node transition only.
// Precomputes every (2^d)-th step (0 <= d < kNumBits).
template <int kNumBits = 60>
struct SimpleFunctionalGraph {
 private:
  // number of nodes.
  int size;

  // next_pos[d][i] := starting from i, what's the position after 2^d steps.
  std::vector<std::vector<int>> next_pos;

  bool build_done_;

  // Builds the transition table.
  void build_() {
    if (build_done_) return;
    for (int d = 0; d + 1 < kNumBits; ++d) {
      for (int i = 0; i < size; ++i) {
        int p = next_pos[d][i];
        next_pos[d + 1][i] = next_pos[d][p];
      }
    }
    build_done_ = true;
  }

 public:
  explicit SimpleFunctionalGraph(int n)
      : size(n), next_pos(kNumBits, std::vector<int>(n)), build_done_(false) {
    for (int d = 0; d < kNumBits; ++d) {
      for (int i = 0; i < n; ++i) {
        next_pos[d][i] = i;  // self-loop by default
      }
    }
  }

  // Sets `next` as the next position of node `pos`.
  void set_next(int pos, int next) { next_pos[0][pos] = next; }

  // Starting from `start`, `steps` times goes forward and returns
  // the end node.
  int go(int start, long long steps) {
    // steps >= 2^kNumBits is not supported.
    assert(steps < (1LL << kNumBits));
    build_();
    int cur_node = start;
    for (int d = kNumBits - 1; d >= 0; --d) {
      if ((steps >> d) & 1) {
        cur_node = next_pos[d][cur_node];
      }
    }
    return cur_node;
  }

  template <class F>
  long long min_steps(int start, F pred) {
    static_assert(std::is_invocable_r_v<bool, F, int>);
    build_();
    long long max_steps_false = 0;
    int cur_node = start;
    for (int d = kNumBits - 1; d >= 0;) {
      int next_node = next_pos[d][cur_node];
      if (pred(next_node)) {
        // Overrun. Retry with a smaller jump.
        --d;
      } else {
        // Continue jumping by the same d.
        max_steps_false += 1LL << d;
        cur_node = next_node;
      }
    }
    return max_steps_false + 1;
  }
};

// Node transition and aggregated values along transition.
//
// Precomputes every (2^d)-th step (0 <= d < kNumBits).
// Be careful about overflow!
template <typename Monoid, int kNumBits = 32>
struct AggFunctionalGraph {
 private:
  using T = typename Monoid::T;

  // number of nodes.
  int size;

  // acc_value[d][i] := starting from i, what's the value accumulated after 2^d
  // steps.
  std::vector<std::vector<T>> acc_value;

  // next_pos[d][i] := starting from i, what's the position after 2^d steps.
  std::vector<std::vector<int>> next_pos;

  bool build_done_;

  // Builds transition tables.
  void build_() {
    if (build_done_) return;
    for (int d = 0; d + 1 < kNumBits; ++d) {
      for (int i = 0; i < size; ++i) {
        int p = next_pos[d][i];
        next_pos[d + 1][i] = next_pos[d][p];
        acc_value[d + 1][i] = Monoid::op(acc_value[d][i], acc_value[d][p]);
      }
    }
    build_done_ = true;
  }

 public:
  explicit AggFunctionalGraph(int n)
      : size(n),
        acc_value(kNumBits, std::vector<T>(n, Monoid::id())),
        next_pos(kNumBits, std::vector<int>(n)),
        build_done_(false) {
    for (int d = 0; d < kNumBits; ++d) {
      for (int i = 0; i < n; ++i) {
        next_pos[d][i] = i;  // self-loop by default
      }
    }
  }

  // Sets `next` as the next position of node `pos`.
  void set_next(int pos, int next) { next_pos[0][pos] = next; }

  // Sets value `x` at node `pos`.
  void set_value(int pos, T value) { acc_value[0][pos] = value; }

  // Starting from `start`, `steps` times goes forward and accumulates values.
  std::pair<T, int> go(int start, long long steps) {
    // steps >= 2^kNumBits is not supported.
    assert(steps < (1LL << kNumBits));
    build_();

    T agg = Monoid::id();
    int cur_node = start;
    for (int d = kNumBits - 1; d >= 0; --d) {
      if ((steps >> d) & 1) {
        agg = Monoid::op(agg, acc_value[d][cur_node]);
        cur_node = next_pos[d][cur_node];
      }
    }
    return {agg, cur_node};
  }

  // Returns the minimum steps to reach pred() == true.
  template <class F>
  long long min_steps(int start, F pred) {
    static_assert(std::is_invocable_r_v<bool, F, T, int>);
    build_();
    long long max_steps_false = 0;
    T cur_agg = Monoid::id();
    int cur_node = start;
    for (int d = kNumBits - 1; d >= 0;) {
      T next_agg = Monoid::op(cur_agg, acc_value[d][cur_node]);
      int next_node = next_pos[d][cur_node];
      if (pred(next_agg, next_node)) {
        // Overrun. Retry with a smaller jump.
        --d;
      } else {
        // Continue jumping by the same d.
        max_steps_false += 1LL << d;
        std::swap(cur_agg, next_agg);
        std::swap(cur_node, next_node);
      }
    }
    return max_steps_false + 1;
  }
};
