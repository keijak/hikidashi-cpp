#include <bits/stdc++.h>
using namespace std;

struct Query {
  int idx;
  int l;
  int r;
};

struct Task {
  vector<int> answer;

  explicit Task(int num_queries) : answer(num_queries) {}

  void add_left(int l) { add(l); }
  void add_right(int r) { add(r); }
  void erase_left(int l) { erase(l); }
  void erase_right(int r) { erase(r); }

  // Adds one item at position i.
  void add(int i) {
    // Update state.
  }

  // Erases one item at position i.
  void erase(int i) {
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
    int q = (int)queries.size();
    int bs = n / min<int>(n, sqrt(q));
    vector<int> ord(q);
    iota(begin(ord), end(ord), 0);
    sort(begin(ord), end(ord), [&](int a, int b) {
      int ablock = queries[a].l / bs;
      int bblock = queries[b].l / bs;
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
