// Mapping disjoint half-open intervals [l, r) to values.
// Each interval is associated with a value.
// ref. https://noimi.hatenablog.com/entry/2021/05/02/195143
#include <bits/stdc++.h>
using namespace std;
using Int = long long;

struct Interval {
  Int l, r;
  Int val;
};
ostream &operator<<(ostream &os, const Interval &iv) {
  return os << "{[" << iv.l << ", " << iv.r << ") => " << iv.val << "}";
}

struct EventHandler {
  Int length_sum = 0;

  EventHandler() = default;
  void on_add(const Interval &iv) { length_sum += iv.r - iv.l; }
  void on_remove(const Interval &iv) { length_sum -= iv.r - iv.l; }
};

class IntervalMap : public std::map<Int, Interval> {
 public:
  EventHandler event_handler_;

  IntervalMap() = default;

  // Returns the interval which contains p if available.
  // Otherwise returns this->end().
  std::map<Int, Interval>::iterator find_interval(Int p) {
    auto it = upper_bound(p);
    if (it != begin()) {
      --it;
      if (it->second.r > p) return it;
    }
    return end();
  }

  // Sets interval [l, r) => value.
  void set_interval(const Interval &interval) {
    if (interval.l >= interval.r) return;  // empty interval
    remove_interval(interval.l, interval.r);
    event_handler_.on_add(interval);
    (*this)[interval.l] = interval;
  }

  // Removes interval [l, r)
  void remove_interval(Int l, Int r) {
    if (l >= r) return;  // empty interval
    auto itl = upper_bound(l), itr = lower_bound(r);
    if (itl != begin()) {
      --itl;
      if (itl->second.r <= l) ++itl;
    }
    if (itl == itr) return;
    std::optional<Interval> ltip, rtip;
    if (itl->first < l) {
      ltip = itl->second;
      assert(ltip->r >= l);
      ltip->r = l;
    }
    if (auto riv = std::prev(itr)->second; riv.r > r) {
      rtip = riv;
      assert(rtip->l <= r);
      rtip->l = r;
    }
    for (auto it = itl; it != itr; it = erase(it)) {
      event_handler_.on_remove(it->second);
    }
    if (ltip) {
      event_handler_.on_add(*ltip);
      (*this)[ltip->l] = *ltip;
    }
    if (rtip) {
      event_handler_.on_add(*rtip);
      (*this)[rtip->l] = *rtip;
    }
  }
};
