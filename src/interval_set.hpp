#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

const i64 INF = 1e18;

// Set of disjoint half-open intervals.
struct IntervalSet {
  using PSet = std::set<std::pair<i64, i64>>;
  PSet set_;  // {{start, end}}

  // Sum of all interval lengths.
  i64 total_length_;

  IntervalSet() : total_length_(0) {}

  i64 total_length() const { return total_length_; }

  int count() const { return set_.size(); }

  const PSet &get() const { return set_; }

  // Adds an interval. It's merged with all existing intervals.
  // [istart, iend) - right-open interval
  void emplace(i64 istart, i64 iend) {
    if (istart >= iend) {
      return;  // Ignore empty intervals.
    }
    if (auto it = this->find(istart);
        it != this->end() and it->first <= istart and iend <= it->second) {
      return;  // Completely contained. Skip.
    }

    // New interval to be inserted.
    std::pair<i64, i64> inew = {istart, iend};

    auto it = set_.upper_bound({istart, INF});
    if (it != set_.begin()) --it;
    while (it != set_.end()) {
      if (it->first > iend) break;
      if (it->second < istart) {
        ++it;
        continue;
      }
      // Merge.
      inew.first = std::min(inew.first, it->first);
      inew.second = std::max(inew.second, it->second);
      total_length_ -= it->second - it->first;
      it = set_.erase(it);
    }
    set_.insert(inew);
    total_length_ += inew.second - inew.first;
  }

  PSet::iterator end() const { return set_.end(); }

  PSet::iterator find(i64 point) const {
    auto it = set_.upper_bound({point, INF});
    if (it == set_.begin()) return set_.end();
    --it;
    if (it->second <= point) return set_.end();
    return it;
  }

  // Returns true if the point is included in an interval in the set.
  bool contains(i64 point) const { return this->find(point) != this->end(); }
};
