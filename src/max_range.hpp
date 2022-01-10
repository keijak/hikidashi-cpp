// Max/Min Range.
// Utility for an O(n) solution to Largest Rectangle in Histogram.
// ref: https://qiita.com/kuuso1/items/318d42cd089a49eeb332

#include <deque>
#include <limits>
#include <utility>
#include <vector>

// For each i, what's the maximum consecutive range in which a[i] is the
// maximum?
// Tie-breaking with index: comparing (a[i],i) with (a[j],j).
// Time complexity: O(n)
template <typename T>
std::vector<std::pair<int, int>> max_range(const std::vector<T> &a) {
  static const T kSentinel = std::numeric_limits<T>::max();
  const int n = a.size();
  std::vector<std::pair<int, int>> res(n);
  std::deque<int> q;

  // left bound
  for (int i = n - 1; i >= -1; --i) {
    const auto &x = i >= 0 ? a[i] : kSentinel;
    while (not q.empty() and a[q.back()] < x) {
      int j = q.back();
      res[j].first = i + 1;
      q.pop_back();
    }
    q.push_back(i);
  }
  q.clear();

  // right bound
  for (int i = 0; i <= n; ++i) {
    const auto &x = i < n ? a[i] : kSentinel;
    while (not q.empty() and a[q.back()] <= x) {
      int j = q.back();
      res[j].second = i;
      q.pop_back();
    }
    q.push_back(i);
  }
  return res;
}

// For each i, what's the maximum consecutive range in which a[i] is the
// minimum?
// Tie-breaking with index: comparing (a[i],i) with (a[j],j).
// Time complexity: O(n)
template <typename T>
std::vector<std::pair<int, int>> min_range(const std::vector<T> &a) {
  static const T kSentinel = std::numeric_limits<T>::lowest();
  const int n = a.size();
  std::vector<std::pair<int, int>> res(n);
  std::deque<int> q;

  // left bound
  for (int i = n - 1; i >= -1; --i) {
    const auto &x = i >= 0 ? a[i] : kSentinel;
    while (not q.empty() and a[q.back()] >= x) {
      int j = q.back();
      res[j].first = i + 1;
      q.pop_back();
    }
    q.push_back(i);
  }
  q.clear();

  // right bound
  for (int i = 0; i <= n; ++i) {
    const auto &x = i < n ? a[i] : kSentinel;
    while (not q.empty() and a[q.back()] > x) {
      int j = q.back();
      res[j].second = i;
      q.pop_back();
    }
    q.push_back(i);
  }
  return res;
}
