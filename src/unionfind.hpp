#include <bits/stdc++.h>

struct UnionFind {
  int n_;
  std::vector<int> parent_;  // negative: size

  explicit UnionFind(int n) : n_(n), parent_(n, -1) {}

  bool unite(int x, int y) {
    x = find(x), y = find(y);
    if (x == y) return false;
    if (-parent_[x] < -parent_[y]) std::swap(x, y);
    parent_[x] += parent_[y];
    parent_[y] = x;
    return true;
  }

  int find(int v) {
    if (parent_[v] < 0) return v;
    return parent_[v] = find(parent_[v]);
  }

  int size(int v) { return -parent_[find(v)]; }

  bool same(int x, int y) { return find(x) == find(y); }
};

template <typename Abelian>  // Abelian Monoid
struct AggUnionFind : public UnionFind {
  using T = typename Abelian::T;
  std::vector<T> agg_data_;

  explicit AggUnionFind(std::vector<T> data)
      : UnionFind(data.size()), agg_data_(std::move(data)) {}

  bool unite(int u, int v) {
    int ru = this->find(u), rv = this->find(v);
    bool united = UnionFind::unite(u, v);
    if (not united) return false;
    int r = this->find(u);
    agg_data_[r] = Abelian::op(agg_data_[ru], agg_data_[rv]);
    return true;
  }

  T agg(int v) const { return agg_data_[this->find(v)]; }
};

template <typename Abelian>  // Abelian Group (requires `invert`)
struct UpdatableAggUnionFind : public AggUnionFind<Abelian> {
  using T = typename Abelian::T;
  std::vector<T> leaf_data_;

  explicit UpdatableAggUnionFind(int sz)
      : AggUnionFind<Abelian>(std::vector<T>(sz, Abelian::id())),
        leaf_data_(sz, Abelian::id()) {}

  explicit UpdatableAggUnionFind(std::vector<T> data)
      : AggUnionFind<Abelian>(data), leaf_data_(std::move(data)) {}

  void set(int v, T val) {
    int r = this->find(v);
    T inv_val = Abelian::invert(std::move(leaf_data_[v]));
    this->agg_data_[r] = Abelian::op(
        val, Abelian::op(std::move(this->agg_data_[r]), std::move(inv_val)));
    leaf_data_[v] = std::move(val);
  }

  T get(int v) const { return leaf_data_[v]; }
};

struct WeightedUnionFind {
  using Int = long long;
  int n_;
  std::vector<int> parent_;
  std::vector<int> rank_;
  std::vector<Int> weight_;  // diff from the parent_ node

  explicit WeightedUnionFind(int n)
      : n_(n), parent_(n, -1), rank_(n, 0), weight_(n, 0) {
    std::iota(parent_.begin(), parent_.end(), 0);
  }

  bool unite(int x, int y, Int w) {
    w += weight(x);
    w -= weight(y);
    x = find(x), y = find(y);
    if (x == y) return false;
    if (rank_[x] < rank_[y]) {
      std::swap(x, y);
      w = -w;
    }
    if (rank_[x] == rank_[y]) ++rank_[x];
    parent_[y] = x;
    weight_[y] = w;
    return true;
  }

  int find(int v) {
    if (parent_[v] == v) return v;
    int r = find(parent_[v]);
    weight_[v] += weight_[parent_[v]];
    return parent_[v] = r;
  }

  bool same(int x, int y) { return find(x) == find(y); }

  // Returns the relative weight from the component root.
  Int weight(int x) {
    find(x);
    return weight_[x];
  }

  // Returns the delta between two nodes.
  // Assumes x and y are in the same component.
  Int diff(int x, int y) { return weight(y) - weight(x); }
};

// Partially Persistent UnionFind.
struct UnionFindWithTime {
  int n;
  mutable std::vector<int> parent_;  // positive: parent, negative: size
  std::vector<int> rank_;
  int num_roots_;
  int clock_;
  std::vector<int> parented_time_;
  std::vector<std::vector<std::pair<int, int>>> size_history_;

  explicit UnionFindWithTime(int sz)
      : n(sz),
        parent_(sz, -1),
        rank_(sz, 1),
        num_roots_(sz),
        clock_(0),
        parented_time_(sz, -1),
        size_history_(n, {{0, 1}}) {}

  // Returns current clock_.
  int unite(int x, int y) {
    ++clock_;
    x = find(x, clock_), y = find(y, clock_);
    if (x == y) return clock_;
    if (rank_[x] < rank_[y] or
        (rank_[x] == rank_[y] and -parent_[x] < -parent_[y])) {
      std::swap(x, y);
    }
    parent_[x] += parent_[y];
    parent_[y] = x;
    rank_[x] = std::max(rank_[x], rank_[y] + 1);
    parented_time_[y] = clock_;
    size_history_[x].emplace_back(clock_, -parent_[x]);
    --num_roots_;
    return clock_;
  }

  int find(int v, int time) const {
    if (parent_[v] < 0) return v;
    if (time < parented_time_[v]) return v;
    return find(parent_[v], time);
  }
  int find(int v) const { return find(v, clock_); }

  int size(int v, int time) const {
    int r = find(v, time);
    const auto& h = size_history_[r];
    auto it = std::lower_bound(h.begin(), h.end(), std::pair(time + 1, -1));
    return (--it)->second;
  }
  int size(int v) const { return -parent_[find(v)]; }

  bool same(int u, int v, int time) const {
    return find(u, time) == find(v, time);
  }
  bool same(int u, int v) const { return find(u) == find(v); }

  std::optional<int> united_time(int u, int v) {
    if (not same(u, v)) {
      return std::nullopt;
    }
    int fv = 0, tv = clock_;
    while (tv - fv > 1) {
      int mid = (tv + fv) / 2;
      if (same(u, v, mid)) {
        tv = mid;
      } else {
        fv = mid;
      }
    }
    return tv;
  }
};

struct UndoableUnionFind {
  std::vector<int> parent_;
  struct Record {
    int x, y;
    int parent_x, parent_y;
  };
  std::stack<Record> history_;

  explicit UndoableUnionFind(int sz) : parent_(sz, -1) {}

  bool unite(int x, int y) {
    x = find(x), y = find(y);
    if (-parent_[x] < -parent_[y]) std::swap(x, y);
    history_.push({x, y, parent_[x], parent_[y]});
    if (x == y) return false;
    parent_[x] += parent_[y];
    parent_[y] = x;
    return true;
  }

  int find(int k) {
    if (parent_[k] < 0) return k;
    return find(parent_[k]);  // no path compression
  }

  int size(int k) { return -parent_[find(k)]; }

  bool same(int x, int y) { return find(x) == find(y); }

  // Undoes one unite() call.
  void undo() {
    auto record = history_.top();
    history_.pop();
    parent_[record.x] = record.parent_x;
    parent_[record.y] = record.parent_y;
  }

  // Returns a checkpoint.
  int snapshot() const { return history_.size(); }

  // Undoes till the checkpoint.
  void rollback(int checkpoint) {
    while ((int)history_.size() > checkpoint) undo();
  }
};

// https://codeforces.com/blog/entry/83467
struct FIFOUndoableUnionFind {
  std::vector<int> parent_;

  struct Record {
    int record_type;  // 1: reversed
    int time;
    int x, y;    // original arguments
    int xr, yr;  // component representatives
    int parent_xr, parent_yr;
  };
  std::stack<Record> history_;

  void restore(const Record& record) {
    parent_[record.xr] = record.parent_xr;
    parent_[record.yr] = record.parent_yr;
  }

  explicit FIFOUndoableUnionFind(int sz) : parent_(sz, -1) {}

  bool unite(int x, int y, int time, int record_type = 0) {
    int xr = find(x), yr = find(y);
    if (xr == yr) return false;
    if (-parent_[xr] < -parent_[yr]) std::swap(xr, yr);
    history_.push({record_type, time, x, y, xr, yr, parent_[xr], parent_[yr]});
    parent_[x] += parent_[y];
    parent_[y] = x;
    return true;
  }

  int find(int k) {
    if (parent_[k] < 0) return k;
    return find(parent_[k]);  // no path compression
  }

  int size(int k) { return -parent_[find(k)]; }

  bool same(int x, int y) { return find(x) == find(y); }

  void undo(int until) {
    while (not history_.empty()) {
      auto record = history_.top();
      if (record.record_type == 1 and record.time > until) break;
      history_.pop();
      restore(record);
      if (record.record_type == 1) continue;
      std::vector<Record> records0 = {record}, records1 = {};
      while (not history_.empty()) {
        if (records0.size() == records1.size()) break;
        auto r = history_.top();
        history_.pop();
        restore(r);
        (r.record_type == 1 ? records1 : records0).push_back(r);
      }
      bool reversing = false;
      if (history_.empty()) {
        reversing = true;
        std::reverse(records0.begin(), records0.end());
      }
      for (auto it = records0.rbegin(); it != records0.rend(); ++it) {
        unite(it->x, it->y, it->time, reversing);
      }
      for (auto it = records1.rbegin(); it != records1.rend(); ++it) {
        unite(it->x, it->y, it->time, true);
      }
    }
  }
};
