#include <bits/stdc++.h>

template <typename T>
struct Compressed {
  std::vector<T> values;

  explicit Compressed(std::vector<T> v) : values(v) {
    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());
  }

  int size() const { return values.size(); }

  const T& value(int i) const { return values[i]; }

  int index(const T& x) const {
    return std::lower_bound(values.begin(), values.end(), x) - values.begin();
  }

  bool contains(const T& x) const {
    return std::binary_search(values.begin(), values.end(), x);
  }
};
