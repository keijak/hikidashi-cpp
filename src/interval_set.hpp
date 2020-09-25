#include <set>
#include <utility>
using namespace std;
using i64 = long long;

struct IntervalSet {
  // Set of disjoint right-open intervals.
  set<pair<i64, i64>> _set;  // {{end, start}}

  // Sum of all interval lengths.
  i64 _length_sum;

  IntervalSet() : _length_sum(0) {}

  i64 length_sum() const { return _length_sum; }

  int count() const { return _set.size(); }

  // Adds an interval. It's merged with all existing intervals.
  // [istart, iend) - right-open interval
  void emplace(i64 istart, i64 iend) {
    // New interval to be inserted.
    pair<i64, i64> inew = {iend, istart};

    auto it1 = _set.upper_bound({istart, 0});
    if (it1 != _set.end() && it1->second <= istart) {
      if (it1->first >= iend) return;
      inew.second = it1->second;
    }

    auto it2 = _set.lower_bound({iend, 0});
    if (it2 != _set.end() && it2->second < iend) {
      if (it2->second <= istart) return;
      inew.first = it2->first;
      ++it2;
    }

    for (auto it = it1; it != it2; ++it) {
      _length_sum -= it->first - it->second;
    }
    _set.erase(it1, it2);

    _set.insert(inew);
    _length_sum += inew.first - inew.second;
  }

  // Returns true if the point is included in an interval in the set.
  bool contains(i64 point) const {
    auto it = _set.upper_bound({point, 0});
    if (it == _set.end()) return false;
    return (it->second <= point);
  }
};
