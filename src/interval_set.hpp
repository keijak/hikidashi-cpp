#include <bits/stdc++.h>
using namespace std;
using Int = long long;

// A set of disjoint half-open intervals [l, r).
// (mapping l to r)
class IntervalSet : public std::map<Int, Int> {
 private:
  // If true, automatically merges [l, c) and [c, r).
  // Otherwise only merges intervals when they have non-empty overlap.
  bool merge_adjacent;

 public:
  explicit IntervalSet(bool merge_adjacent = true)
      : merge_adjacent(merge_adjacent) {}

  // Returns the interval [l, r) which contains p if available.
  // Otherwise returns this->end().
  std::map<Int, Int>::iterator find_interval(Int p) {
    auto it = upper_bound(p);
    if (it != begin()) {
      --it;
      if (it->second > p) return it;
    }
    return end();
  }

  // Inserts interval [l, r)
  void add_interval(Int l, Int r) {
    auto itl = upper_bound(l), itr = lower_bound(r + merge_adjacent);
    if (itl != begin()) {
      --itl;
      if (itl->second + merge_adjacent <= l) ++itl;
    }
    if (itl != itr) {
      l = std::min(l, itl->first);
      r = std::max(r, std::prev(itr)->second);
      for (auto it = itl; it != itr;) {
        it = erase(it);
      }
    }
    (*this)[l] = r;
  }

  // Removes interval [l, r)
  void remove_interval(Int l, Int r) {
    auto itl = upper_bound(l), itr = lower_bound(r);
    if (itl != begin()) {
      --itl;
      if (itl->second <= l) ++itl;
    }
    if (itl == itr) return;
    Int tl = std::min(l, itl->first);
    Int tr = std::max(r, std::prev(itr)->second);
    for (auto it = itl; it != itr;) {
      it = erase(it);
    }
    if (tl < l) {
      (*this)[tl] = l;
    }
    if (r < tr) {
      (*this)[r] = tr;
    }
  }

  // Are p and q in the same interval?
  bool same(Int p, Int q) {
    const auto it = find_interval(p);
    return (it != end()) and (it->first <= q) and (q < it->second);
  }

  // Non-negative minimum excluded value.
  Int mex() {
    const auto it = find_interval(0);
    return (it != end()) ? it->second : 0;
  }
};
