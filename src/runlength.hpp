#include <vector>

template <class T>
std::vector<std::pair<T, int>> runlength(const std::vector<T> &a) {
  std::vector<std::pair<T, int>> res;
  const int n = a.size();
  if (n == 0) return res;
  res.reserve(n);
  res.emplace_back(a[0], 1);
  for (int i = 1; i < n; ++i) {
    if (a[i] == res.back().first) {
      res.back().second++;
    } else {
      res.emplace_back(a[i], 1);
    }
  }
  return res;
}
