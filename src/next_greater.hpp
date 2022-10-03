// Max/Min Range.
// Utility for an O(n) solution to Largest Rectangle in Histogram.
//
// Barbay, Fischer & Navarro (2012)
// ref: https://en.wikipedia.org/wiki/All_nearest_smaller_values

#include <utility>
#include <vector>

// For each i, what's the maximum consecutive range in which a[i] is the
// maximum?
// Tie-breaking with index: comparing (a[i],i) with (a[j],j).
// Time complexity: O(n)
template <typename T>
std::vector<std::pair<int, int>> next_greater(const std::vector<T> &a) {
  const int n = a.size();
  std::vector<std::pair<int, int>> res(n);
  // left bound
  for (int i = 0; i < n; ++i) {
    int j = i - 1;
    while (j >= 0 and a[j] <= a[i]) j = res[j].first - 1;
    res[i].first = j + 1;
  }
  // right bound
  for (int i = n - 1; i >= 0; --i) {
    int j = i + 1;
    while (j < n and a[j] < a[i]) j = res[j].second;
    res[i].second = j;
  }
  return res;
}

// For each i, what's the maximum consecutive range in which a[i] is the
// minimum?
// Tie-breaking with index: comparing (a[i],i) with (a[j],j).
// Time complexity: O(n)
template <typename T>
std::vector<std::pair<int, int>> next_smaller(const std::vector<T> &a) {
  const int n = a.size();
  std::vector<std::pair<int, int>> res(n);
  // left bound
  for (int i = 0; i < n; ++i) {
    int j = i - 1;
    while (j >= 0 and a[j] > a[i]) j = res[j].first - 1;
    res[i].first = j + 1;
  }
  // right bound
  for (int i = n - 1; i >= 0; --i) {
    int j = i + 1;
    while (j < n and a[j] >= a[i]) j = res[j].second;
    res[i].second = j;
  }
  return res;
}
