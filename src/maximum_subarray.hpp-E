// Kadane's Algorithm
// `a` must not be empty (or provide a default value).
// `a[i]` can be negative.
template <typename T>
T maximum_subarray(const vector<T>& a) {
  assert(!a.empty());
  T res = a[0];  // default value
  T sum = 0;
  for (const auto& x : a) {
    sum = max(sum + x, x);
    res = max(res, sum);
  }
  return res;
}
