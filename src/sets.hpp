#include <set>

// s |= t;
// Set Union.
// The first argument will be updated to the result.
// cf. https://qiita.com/i_saint/items/a8bdce5146bb38e69f72
template <typename T>
std::set<T> &operator|=(std::set<T> &s, const std::set<T> &t) {
  auto pos = s.begin();
  for (auto it = t.begin(); it != t.end(); ++it, ++pos) {
    pos = s.emplace_hint(pos, *it);
  }
  return s;
}
template <typename T>
std::set<T> &operator|=(std::set<T> &s, std::set<T> &&t) {
  if (s.size() < t.size()) s.swap(t);  // small-to-large merging
  for (auto pos = s.begin(); !t.empty(); ++pos) {
    pos = s.insert(pos, t.extract(t.begin()));
  }
  return s;
}

// s &= t;
// Set Intersection.
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
// Set Difference.
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
