#include <algorithm>
#include <limits>
#include <type_traits>
#include <utility>
#include <vector>

struct SuccinctBitVector {
  int size_;
  int blocks_;
  std::vector<unsigned> bit_, sum_;

  SuccinctBitVector() : size_(0), blocks_(0){};

  explicit SuccinctBitVector(int size) {
    size_ = size;
    blocks_ = (size_ + 31) >> 5;
    bit_.assign(blocks_, 0U);
    sum_.assign(blocks_, 0U);
  }

  void set(int k) { bit_[k >> 5] |= 1U << (k & 31); }

  void build() {
    sum_[0] = 0U;
    for (int i = 1; i < blocks_; i++) {
      sum_[i] = sum_[i - 1] + __builtin_popcount(bit_[i - 1]);
    }
  }

  // O(1)
  bool operator[](int k) const { return (bit_[k >> 5] >> (k & 31)) & 1; }

  // O(1)
  int rank(int k) const {
    return (sum_[k >> 5] +
            __builtin_popcount(bit_[k >> 5] & ((1U << (k & 31)) - 1)));
  }

  // O(1)
  int rank(bool val, int k) const { return (val ? rank(k) : k - rank(k)); }

  // O(log(n))
  int select(bool val, int k) const {
    if (k < 0 || rank(val, size_) <= k) return (-1);
    int low = 0, high = size_;
    while (high - low > 1) {
      int mid = (low + high) >> 1;
      if (rank(val, mid) >= k + 1)
        high = mid;
      else
        low = mid;
    }
    return (high - 1);
  }

  // O(log(n))
  int select(bool val, int i, int l) const {
    return select(val, i + rank(val, l));
  }
};

template <class T, int MAXLOG = std::numeric_limits<T>::digits>
struct WaveletMatrix {
  static_assert(std::is_unsigned<T>::value, "Requires unsigned type");
  int size_;
  SuccinctBitVector matrix[MAXLOG];
  int mid[MAXLOG];

  WaveletMatrix() = default;

  explicit WaveletMatrix(std::vector<T> v) : size_(v.size()) {
    std::vector<T> l(size_), r(size_);
    for (int level = MAXLOG - 1; level >= 0; level--) {
      matrix[level] = SuccinctBitVector(size_ + 1);
      int left = 0, right = 0;
      for (int i = 0; i < size_; i++) {
        if (((v[i] >> level) & 1)) {
          matrix[level].set(i);
          r[right++] = v[i];
        } else {
          l[left++] = v[i];
        }
      }
      mid[level] = left;
      matrix[level].build();
      v.swap(l);
      for (int i = 0; i < right; i++) {
        v[left + i] = r[i];
      }
    }
  }

  inline int size() const { return size_; }

  // access(i): the value at i (0-indexed).
  T operator[](int i) const {
    T ret = 0;
    for (int level = MAXLOG - 1; level >= 0; level--) {
      bool f = matrix[level][i];
      if (f) ret |= T(1) << level;
      i = matrix[level].rank(f, i) + mid[level] * f;
    }
    return ret;
  }

  // count i s.t. (0 <= i < r) && v[i] == x
  int rank(const T &x, int r) {
    int l = 0;
    for (int level = MAXLOG - 1; level >= 0; level--) {
      std::tie(l, r) = succ((x >> level) & 1, l, r, level);
    }
    return r - l;
  }

  // k-th(0-indexed) smallest number in v[l,r)
  T kth_smallest(int l, int r, int k) const {
    assert(0 <= k && k < r - l);
    T ret = 0;
    for (int level = MAXLOG - 1; level >= 0; level--) {
      int cnt = matrix[level].rank(false, r) - matrix[level].rank(false, l);
      bool f = cnt <= k;
      if (f) {
        ret |= T(1) << level;
        k -= cnt;
      }
      std::tie(l, r) = succ(f, l, r, level);
    }
    return ret;
  }

  // k-th(0-indexed) largest number in v[l,r)
  T kth_largest(int l, int r, int k) const {
    return kth_smallest(l, r, r - l - k - 1);
  }

  // count i s.t. (l <= i < r) && (v[i] < upper)
  int range_freq(int l, int r, T upper) const {
    int ret = 0;
    for (int level = MAXLOG - 1; level >= 0; level--) {
      bool f = ((upper >> level) & 1);
      if (f) ret += matrix[level].rank(false, r) - matrix[level].rank(false, l);
      std::tie(l, r) = succ(f, l, r, level);
    }
    return ret;
  }

  // count i s.t. (l <= i < r) && (lower <= v[i] < upper)
  int range_freq(int l, int r, T lower, T upper) const {
    return range_freq(l, r, upper) - range_freq(l, r, lower);
  }

  // max v[i] s.t. (l <= i < r) && (v[i] < upper)
  T prev_value(int l, int r, T upper) const {
    int cnt = range_freq(l, r, upper);
    return cnt == 0 ? T(-1) : kth_smallest(l, r, cnt - 1);
  }

  // min v[i] s.t. (l <= i < r) && (lower <= v[i])
  T next_value(int l, int r, T lower) const {
    int cnt = range_freq(l, r, lower);
    return cnt == r - l ? T(-1) : kth_smallest(l, r, cnt);
  }

 private:
  std::pair<int, int> succ(bool f, int l, int r, int level) const {
    return {matrix[level].rank(f, l) + mid[level] * f,
            matrix[level].rank(f, r) + mid[level] * f};
  }
};

// T: can be large nubmers or negative numbers
template <typename T>
struct CompressedWaveletMatrix {
  std::vector<T> values;
  WaveletMatrix<size_t> indices;

  explicit CompressedWaveletMatrix(const std::vector<T> &v) : values(v) {
    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());
    std::vector<size_t> t(v.size());
    for (size_t i = 0; i < v.size(); i++) {
      t[i] = index(v[i]);
    }
    indices = WaveletMatrix<size_t>(t);
  }

  T operator[](int k) const { return values[indices[k]]; }

  int rank(const T &x, int r) const {
    auto pos = index(x);
    if (pos == values.size() || values[pos] != x) return 0;
    return indices.rank(pos, r);
  }

  T kth_smallest(int l, int r, int k) const {
    return values[indices.kth_smallest(l, r, k)];
  }

  T kth_largest(int l, int r, int k) const {
    return values[indices.kth_largest(l, r, k)];
  }

  int range_freq(int l, int r, T upper) const {
    return indices.range_freq(l, r, index(upper));
  }

  int range_freq(int l, int r, T lower, T upper) const {
    return indices.range_freq(l, r, index(lower), index(upper));
  }

  T prev_value(int l, int r, T upper) const {
    auto ret = indices.prev_value(l, r, index(upper));
    return ret == -1 ? T(-1) : values[ret];
  }

  T next_value(int l, int r, T lower) const {
    auto ret = indices.next_value(l, r, index(lower));
    return ret == -1 ? T(-1) : values[ret];
  }

 private:
  // value -> index
  size_t index(const T &x) const {
    return std::lower_bound(values.begin(), values.end(), x) - values.begin();
  }
};
