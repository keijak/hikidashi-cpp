// Kadane's Algorithm
// Returns max(sum(s[l:r])).
// - `a[i]` can be negative.
// - Selecting an empty range is allowed (returns 0).
template <typename T>
T max_subarray(const vector<T>& a) {
  T res = 0;
  T sum = 0;
  for (const auto& x : a) {
    sum = max(sum + x, x);
    res = max(res, sum);
  }
  return res;
}
