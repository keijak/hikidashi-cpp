template <typename T>
struct Compress {
  vector<T> vec;

  explicit Compress(vector<T> v) : vec(v) {
    sort(vec.begin(), vec.end());
    vec.erase(unique(vec.begin(), vec.end()), vec.end());
  }
  int index(T x) const {
    return (int)(lower_bound(vec.begin(), vec.end(), x) - vec.begin());
  }
  const T &value(int i) const { return vec[i]; }
  int size() const { return vec.size(); }
};
