#include <bits/stdc++.h>
using namespace std;

struct Query {
  int idx;
  int l;
  int r;
};

struct Task {
  int n;
  std::vector<long long> answer;

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

  std::vector<Query> queries_;

  void add_query(Query query) { queries_.push_back(std::move(query)); }

  void solve(Task &task) {
    std::vector<int> ord(queries_.size());
    std::iota(ord.begin(), ord.end(), 0);
    std::sort(ord.begin(), ord.end(), [&](int a, int b) {
      const int a_block = queries_[a].l / B;
      const int b_block = queries_[b].l / B;
      if (a_block != b_block) return a_block < b_block;
      return (a_block & 1) ? queries_[a].r > queries_[b].r
                           : queries_[a].r < queries_[b].r;
    });

    int l = 0, r = 0;
    for (auto idx : ord) {
      while (l > queries_[idx].l) task.add_left(--l);
      while (r < queries_[idx].r) task.add_right(r++);
      while (l < queries_[idx].l) task.remove_left(l++);
      while (r > queries_[idx].r) task.remove_right(--r);
      task.process(queries_[idx]);
    }
  }
};
