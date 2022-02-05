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
  void erase_left(int l) { erase_one(l); }
  void erase_right(int r) { erase_one(r); }

  // Adds one item at position i.
  void add_one(int i) {
    // Update state.
  }

  // Erases one item at position i.
  void erase_one(int i) {
    // Update state.
  }

  // Processes one query on the current state.
  void process(const Query &q) {
    // Fill the answer of the query.
    answer[q.idx] = q.idx;
  }
};

struct Mo {
  int n;
  vector<Query> queries;

  explicit Mo(int n) : n(n) {}

  void add_query(Query query) { queries.push_back(std::move(query)); }

  void solve(Task &task) const {
    const int Q = (int)queries.size();
    const int B = n / min<int>(n, sqrt(Q));
    vector<int> ord(Q);
    iota(begin(ord), end(ord), 0);
    sort(begin(ord), end(ord), [&](int a, int b) {
      int ablock = queries[a].l / B;
      int bblock = queries[b].l / B;
      if (ablock != bblock) return ablock < bblock;
      return (ablock & 1) ? queries[a].r > queries[b].r
                          : queries[a].r < queries[b].r;
    });
    int l = 0, r = 0;
    for (auto idx : ord) {
      while (l > queries[idx].l) task.add_left(--l);
      while (r < queries[idx].r) task.add_right(r++);
      while (l < queries[idx].l) task.erase_left(l++);
      while (r > queries[idx].r) task.erase_right(--r);
      task.process(queries[idx]);
    }
  }
};
