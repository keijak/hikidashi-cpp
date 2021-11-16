#include <bits/stdc++.h>

template<typename T>
struct Compressed {
  std::vector<T> values;

  explicit Compressed(std::vector<T> v) : values(std::move(v)) {
    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());
  }

  int size() const { return values.size(); }

  const T &value(int i) const { return values[i]; }

  int index(const T &x) const {
    return std::lower_bound(values.begin(), values.end(), x) - values.begin();
  }

  int lower_index(const T &x) const {
    int ub = std::upper_bound(values.begin(), values.end(), x) - values.begin();
    return ub - 1;
  }

  bool contains(const T &x) const {
    return std::binary_search(values.begin(), values.end(), x);
  }
};
