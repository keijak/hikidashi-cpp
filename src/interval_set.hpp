#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

// Set of disjoint closed intervals [l, r].
class ClosedIntervalSet : public std::map<i64, i64> {
 private:
  // If true, automatically merges [l, c] and [c+1, r].
  bool merge_adjacent;

 public:
  explicit ClosedIntervalSet(bool merge_adjacent)
      : merge_adjacent(merge_adjacent) {}

  // Returns the interval [l, r] which contains p if available.
  // Otherwise returns this->end().
  std::map<i64, i64>::iterator get(i64 p) {
    auto it = upper_bound(p);
    if (it == begin() || (--it)->second < p) return end();
    return it;
  }

  // Inserts interval [l, r]
  void insert(i64 l, i64 r) {
    auto itl = upper_bound(l), itr = upper_bound(r + merge_adjacent);
    if (itl != begin()) {
      if ((--itl)->second < l - merge_adjacent) ++itl;
    }
    if (itl != itr) {
      l = std::min(l, itl->first);
      r = std::max(r, std::prev(itr)->second);
      erase(itl, itr);
    }
    (*this)[l] = r;
  }

  // Removes interval [l, r]
  void remove(i64 l, i64 r) {
    auto itl = upper_bound(l), itr = upper_bound(r);
    if (itl != begin()) {
      if ((--itl)->second < l) ++itl;
    }
    if (itl == itr) return;
    i64 tl = std::min(l, itl->first);
    i64 tr = std::max(r, std::prev(itr)->second);
    erase(itl, itr);
    if (tl < l) (*this)[tl] = l - 1;
    if (r < tr) (*this)[r + 1] = tr;
  }

  // Are p and q in the same interval?
  bool same(i64 p, i64 q) {
    const auto& it = get(p);
    return it != end() && it->first <= q && q <= it->second;
  }

  i64 mex() {
    auto it = get(0);
    if (it == end()) return 0;
    return it->second + 1;
  }
};

static constexpr i64 INF = 1e18;

// Set of disjoint half-open intervals.
struct HalfOpenIntervalSet {
  using PSet = std::set<std::pair<i64, i64>>;
  PSet set_;  // {{start, end}}

  // Sum of all interval lengths.
  i64 total_length_;

  HalfOpenIntervalSet() : total_length_(0) {}

  i64 total_length() const { return total_length_; }

  int count() const { return set_.size(); }

  const PSet& get() const { return set_; }

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
