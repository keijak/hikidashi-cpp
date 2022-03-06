#include <bits/stdc++.h>
using namespace std;

struct Query {
  int idx;
  int l;
  int r;
};

struct Task {
  int n;
  vector<long long> answer;

  Task(int N, int Q) : n(N), answer(Q) {}

  void add_left(int l) { add_one(l); }
  void add_right(int r) { add_one(r); }
  void remove_left(int l) { remove_one(l); }
  void remove_right(int r) { remove_one(r); }

  // Adds one item at position i.
  void add_one(int i) {
    // Update state.
  }

  // Removes one item at position i.
  void remove_one(int i) {
    // Update state.
  }

  // Processes one query on the current state.
  void process(const Query &q) {
    // Fill the answer of the query.
    answer[q.idx] = -1;
  }
};

struct Mo {
  // Note: optimal block size: √3 N / √(2Q)
  // https://nyaannyaan.github.io/library/misc/mo.hpp
  static constexpr int B = 100;

  vector<Query> queries_;
  vector<int> ord_;
  bool build_done_ = false;

  void add_query(Query query) { queries_.push_back(std::move(query)); }

  void build() {
    ord_.resize(queries_.size());
    iota(begin(ord_), end(ord_), 0);
    sort(begin(ord_), end(ord_), [&](int a, int b) {
      int ablock = queries_[a].l / B;
      int bblock = queries_[b].l / B;
      if (ablock != bblock) return ablock < bblock;
      return (ablock & 1) ? queries_[a].r > queries_[b].r
                          : queries_[a].r < queries_[b].r;
    });
    build_done_ = true;
  }

  void solve(Task &task) {
    if (not build_done_) build();
    int l = 0, r = 0;
    for (auto idx : ord_) {
      while (l > queries_[idx].l) task.add_left(--l);
      while (r < queries_[idx].r) task.add_right(r++);
      while (l < queries_[idx].l) task.remove_left(l++);
      while (r > queries_[idx].r) task.remove_right(--r);
      task.process(queries_[idx]);
    }
  }
};
