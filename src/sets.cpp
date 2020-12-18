#include <set>

// s |= t;
// Merges two sets into their union.
// The first argument will be updated to the result.
template <typename T>
void merge_sets_by_union(std::set<T> &s, const std::set<T> &t) {
  s.insert(t.begin(), t.end());
}
template <typename T>
void merge_sets_by_union(std::set<T> &s, std::set<T> &&t) {
  if (s.size() >= t.size()) {
    s.merge(t);
  } else {
    t.merge(s);
    s = std::move(t);
  }
}

// s &= t;
// Merges two sets into their intersection.
// The first argument will be updated to the result.
template <typename T>
void merge_sets_by_intersection(std::set<T> &s, const std::set<T> &t) {
  auto it = s.begin();
  auto jt = t.begin();
  while (it != s.end()) {
    while (jt != t.end() and *jt < *it) ++jt;
    if (jt == t.end()) {
      s.erase(it, s.end());
      break;
    }
    if (*it < *jt) {
      it = s.erase(it);
    } else {
      ++it, ++jt;
    }
  }
}
template <typename T>
void merge_sets_by_intersection(std::set<T> &s, std::set<T> &&t) {
  if (s.size() <= t.size()) {
    merge_sets_by_intersection(s, t);
  } else {
    merge_sets_by_intersection(t, s);
    s = std::move(t);
  }
}

// s -= t;
// Merges two sets into their intersection.
// The first argument will be updated to the result.
template <typename T>
void merge_sets_by_difference(std::set<T> &s, const std::set<T> &t) {
  auto it = s.begin();
  for (auto jt = t.begin(); jt != t.end(); ++jt) {
    while (it != s.end() and *it < *jt) ++it;
    if (it == s.end()) break;
    if (*jt < *it) continue;
    it = s.erase(it);
  }
}
