#include <set>

// s |= t;
// Merges two sets into their union.
// The first argument will be updated to the result.
template <typename T>
std::set<T> &operator|=(std::set<T> &s, const std::set<T> &t) {
  s.insert(t.begin(), t.end());
  return s;
}
template <typename T>
std::set<T> &operator|=(std::set<T> &s, std::set<T> &&t) {
  if (s.size() >= t.size()) {
    s.merge(t);
  } else {
    t.merge(s);
    s.swap(t);
  }
  return s;
}

// s &= t;
// Merges two sets into their intersection.
// The first argument will be updated to the result.
template <typename T>
std::set<T> &operator&=(std::set<T> &s, const std::set<T> &t) {
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
  return s;
}
template <typename T>
std::set<T> &operator&=(std::set<T> &s, std::set<T> &&t) {
  if (s.size() <= t.size()) {
    const std::set<T> &c(t);
    s &= c;
  } else {
    const std::set<T> &c(s);
    t &= c;
    s.swap(t);
  }
  return s;
}

// s -= t;
// Merges two sets into their difference.
// The first argument will be updated to the result.
template <typename T>
std::set<T> &operator-=(std::set<T> &s, const std::set<T> &t) {
  auto it = s.begin();
  for (auto jt = t.begin(); jt != t.end(); ++jt) {
    while (it != s.end() and *it < *jt) ++it;
    if (it == s.end()) break;
    if (*jt < *it) continue;
    it = s.erase(it);
  }
  return s;
}
