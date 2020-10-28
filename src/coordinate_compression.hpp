#include <bits/stdc++.h>

template <typename T>
struct Compress {
  std::vector<T> vec;

  explicit Compress(std::vector<T> v) : vec(v) {
    std::sort(vec.begin(), vec.end());
    vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
  }

  int size() const { return vec.size(); }

  int index(T x) const {
    return lower_bound(vec.begin(), vec.end(), x) - vec.begin();
  }

  const T &value(int i) const { return vec[i]; }
};
