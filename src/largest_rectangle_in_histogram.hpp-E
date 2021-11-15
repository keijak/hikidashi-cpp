#include <bits/stdc++.h>

template <typename T>
std::vector<int> min_range_left(const std::vector<T> &a) {
  const int n = a.size();
  std::vector<int> stk, res(n);
  for (int i = n - 1; i >= -1; --i) {
    T val = (i >= 0) ? a[i] : std::numeric_limits<T>::lowest();
    while (not stk.empty() and a[stk.back()] >= val) {
      int j = stk.back();
      stk.pop_back();
      res[j] = i;
    }
    stk.push_back(i);
  }
  return res;
}

template <typename T>
std::vector<int> min_range_right(const std::vector<T> &a) {
  const int n = a.size();
  std::vector<int> stk, res(n);
  for (int i = 0; i <= n; ++i) {
    T val = (i < n) ? a[i] : std::numeric_limits<T>::lowest();
    while (not stk.empty() and a[stk.back()] >= val) {
      int j = stk.back();
      stk.pop_back();
      res[j] = i;
    }
    stk.push_back(i);
  }
  return res;
}
