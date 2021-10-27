#include <bits/stdc++.h>

// A functional graph is a directed graph in which each vertex has outdegree
// one, and can therefore be specified by a function mapping {0,...,n-1} onto
// itself.
// https://mathworld.wolfram.com/FunctionalGraph.html

// Computes node transition only.
struct SimpleFunctionalGraph {
 private:
  static const int kMaxBits = 60;

  // number of nodes.
  int size;

  // next_pos[d][i] := starting from i, what's the position after 2^d steps.
  std::vector<std::vector<int>> next_pos;

  bool build_done_;

 public:
  explicit SimpleFunctionalGraph(int n)
      : size(n), next_pos(kMaxBits, std::vector<int>(n)), build_done_(false) {
    for (int d = 0; d < kMaxBits; ++d) {
      for (int i = 0; i < n; ++i) {
        next_pos[d][i] = i;  // self-loop by default
      }
    }
  }

  // Sets `next` as the next position of node `pos`.
  void set_next(int pos, int next) { next_pos[0][pos] = next; }

  // Builds the transition table.
  void build() {
    if (build_done_) return;
    for (int d = 0; d + 1 < kMaxBits; ++d) {
      for (int i = 0; i < size; ++i) {
        int p = next_pos[d][i];
        next_pos[d + 1][i] = next_pos[d][p];
      }
    }
    build_done_ = true;
  }

  // Starting from `start`, `steps` times goes forward and returns where it
  // ends up.
  int go(int start, long long steps) {
    // steps >= 2^kMaxBits is not supported.
    assert(steps < (1LL << kMaxBits));
    build();

    int i = start;
    for (int d = kMaxBits - 1; d >= 0; --d) {
      if ((steps >> d) & 1) {
        i = next_pos[d][i];
      }
    }
    return i;
  }

  template <class F>
  long long min_steps(int start, F pred) {
    static_assert(std::is_invocable_r_v<bool, F, int>);
    build();
    long long max_false = 0;
    int i = start;
    for (int d = kMaxBits - 1; d >= 0; --d) {
      auto j = next_pos[d][i];
      if (pred(j)) continue;
      max_false += 1LL << d;
      i = j;
    }
    return max_false + 1;
  }
};

// Computes node transition and aggregates values along transition.
template <typename Monoid>
struct AggFunctionalGraph {
 private:
  using T = typename Monoid::T;
  static const int kMaxBits = 32;

  // number of nodes.
  int size;

  // acc_value[d][i] := starting from i, what's the value accumulated after 2^d
  // steps. Be careful about overflow!
  std::vector<std::vector<T>> acc_value;

  // next_pos[d][i] := starting from i, what's the position after 2^d steps.
  std::vector<std::vector<int>> next_pos;

  bool build_done_;

 public:
  explicit AggFunctionalGraph(int n)
      : size(n),
        acc_value(kMaxBits, std::vector<T>(n, Monoid::id())),
        next_pos(kMaxBits, std::vector<int>(n)),
        build_done_(false) {
    for (int d = 0; d < kMaxBits; ++d) {
      for (int i = 0; i < n; ++i) {
        next_pos[d][i] = i;  // self-loop by default
      }
    }
  }

  // Sets `next` as the next position of node `pos`.
  void set_next(int pos, int next) { next_pos[0][pos] = next; }

  // Sets value `x` at node `pos`.
  void set_value(int pos, T value) { acc_value[0][pos] = value; }

  // Builds transition tables.
  void build() {
    if (build_done_) return;
    for (int d = 0; d + 1 < kMaxBits; ++d) {
      for (int i = 0; i < size; ++i) {
        int p = next_pos[d][i];
        next_pos[d + 1][i] = next_pos[d][p];
        acc_value[d + 1][i] = Monoid::op(acc_value[d][i], acc_value[d][p]);
      }
    }
    build_done_ = true;
  }

  // Starting from `start`, `steps` times goes forward and accumulates values.
  std::pair<T, int> go(int start, long long steps) const {
    build();
    // steps >= 2^kMaxBits is not supported.
    assert(steps < (1LL << kMaxBits));

    T res = Monoid::id();
    int i = start;
    for (int d = kMaxBits - 1; d >= 0; --d) {
      if ((steps >> d) & 1) {
        res = Monoid::op(res, acc_value[d][i]);
        i = next_pos[d][i];
      }
    }
    return {res, i};
  }

  long long min_steps(int start,
                      std::function<bool(const T &, int)> pred) const {
    build();
    long long max_false = 0;
    T val = Monoid::id();
    int i = start;
    for (int d = kMaxBits - 1; d >= 0; --d) {
      T tmp = Monoid::op(val, acc_value[d][i]);
      auto j = next_pos[d][i];
      if (pred(tmp, j)) continue;
      max_false += 1LL << d;
      val = std::move(tmp);
      i = j;
    }
    return max_false + 1;
  }
};
