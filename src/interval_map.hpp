#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

// Disjoint closed intervals [l, r] (mapping l to r).
class ClosedIntervalMap : public std::map<i64, i64> {
 private:
  // If true, automatically merges [l, c] and [c+1, r].
  bool merge_adjacent;

 public:
  ClosedIntervalMap(bool merge_adjacent = true)
      : merge_adjacent(merge_adjacent) {}

  // Returns the interval [l, r] which contains p if available.
  // Otherwise returns this->end().
  std::map<i64, i64>::iterator find_interval(i64 p) {
    auto it = upper_bound(p);
    if (it == begin() || (--it)->second < p) return end();
    return it;
  }

  // Inserts interval [l, r]
  void add_interval(i64 l, i64 r) {
    auto itl = upper_bound(l), itr = upper_bound(r + merge_adjacent);
    if (itl != begin()) {
      if ((--itl)->second < l - merge_adjacent) ++itl;
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

  // Removes interval [l, r]
  void remove_interval(i64 l, i64 r) {
    auto itl = upper_bound(l), itr = upper_bound(r);
    if (itl != begin()) {
      if ((--itl)->second < l) ++itl;
    }
    if (itl == itr) return;
    i64 tl = std::min(l, itl->first);
    i64 tr = std::max(r, std::prev(itr)->second);
    for (auto it = itl; it != itr;) {
      it = erase(it);
    }
    if (tl < l) {
      (*this)[tl] = l - 1;
    }
    if (r < tr) {
      (*this)[r + 1] = tr;
    }
  }

  // Are p and q in the same interval?
  bool same(i64 p, i64 q) {
    const auto it = find_interval(p);
    return it != end() and it->first <= q and q <= it->second;
  }

  // Minimum excluded value greater than or equal to X.
  i64 mex(int bottom = 0) {
    const auto it = find_interval(bottom);
    if (it == end()) return bottom;
    return it->second + 1;
  }
};

// Disjoint half-open intervals [l, r) (mapping l to r).
class IntervalMap : public std::map<i64, i64> {
 private:
  // If true, automatically merges [l, c) and [c, r).
  bool merge_adjacent;
  i64 length_sum_;

 public:
  IntervalMap(bool merge_adjacent = true)
      : merge_adjacent(merge_adjacent), length_sum_(0) {}

  i64 length_sum() const { return length_sum_; }

  // Returns the interval [l, r) which contains p if available.
  // Otherwise returns this->end().
  std::map<i64, i64>::iterator find_interval(i64 p) {
    auto it = upper_bound(p);
    if (it != begin()) {
      --it;
      if (it->second > p) return it;
    }
    return end();
  }

  // Inserts interval [l, r)
  void add_interval(i64 l, i64 r) {
    auto itl = upper_bound(l), itr = lower_bound(r + merge_adjacent);
    if (itl != begin()) {
      --itl;
      if (itl->second <= l - merge_adjacent) ++itl;
    }
    if (itl != itr) {
      l = std::min(l, itl->first);
      r = std::max(r, std::prev(itr)->second);
      for (auto it = itl; it != itr;) {
        length_sum_ -= it->second - it->first;
        it = erase(it);
      }
    }
    (*this)[l] = r;
    length_sum_ += r - l;
  }

  // Removes interval [l, r)
  void remove_interval(i64 l, i64 r) {
    auto itl = upper_bound(l), itr = lower_bound(r);
    if (itl != begin()) {
      --itl;
      if (itl->second <= l) ++itl;
    }
    if (itl == itr) return;
    i64 tl = std::min(l, itl->first);
    i64 tr = std::max(r, std::prev(itr)->second);
    for (auto it = itl; it != itr;) {
      length_sum_ -= it->second - it->first;
      it = erase(it);
    }
    if (tl < l) {
      (*this)[tl] = l;
      length_sum_ += l - tl;
    }
    if (r < tr) {
      (*this)[r] = tr;
      length_sum_ += tr - r;
    }
  }

  // Are p and q in the same interval?
  bool same(i64 p, i64 q) {
    const auto it = find_interval(p);
    return it != end() and it->first <= q and q < it->second;
  }

  // Minimum excluded value greater than or equal to X.
  i64 mex(int bottom = 0) {
    const auto it = find_interval(bottom);
    if (it == end()) return bottom;
    return it->second;
  }
};
