// Utility for an O(n) solution to Largest Rectangle in Histogram.
// https://qiita.com/kuuso1/items/318d42cd089a49eeb332

#include <deque>
#include <limits>
#include <vector>

// How many left consecutive items are greater than a[i].
template <typename T>
std::vector<int> left_min_streak(const std::vector<T> &a) {
  const int inf = std::numeric_limits<T>::lowest();
  const int n = a.size();
  std::vector<int> res(n, 0);
  std::deque<int> q;
  for (int i = n - 1; i >= -1; --i) {
    const auto &x = i >= 0 ? a[i] : inf;
    while (not q.empty() and a[q.back()] >= x) {
      int j = q.back();
      res[j] = j - i - 1;
      q.pop_back();
    }
    q.push_back(i);
  }
  return res;
}

// How many right consecutive items are greater than a[i].
template <typename T>
std::vector<int> right_min_streak(const std::vector<T> &a) {
  const int inf = std::numeric_limits<T>::lowest();
  const int n = a.size();
  std::vector<int> res(n, 0);
  std::deque<int> q;
  for (int i = 0; i <= n; ++i) {
    const auto &x = i < n ? a[i] : inf;
    while (not q.empty() and a[q.back()] >= x) {
      int j = q.back();
      res[j] = i - j - 1;
      q.pop_back();
    }
    q.push_back(i);
  }
  return res;
}
