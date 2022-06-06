#include <bits/stdc++.h>

template <int kMaxSize, int kMaxValue>
struct SparseIntSet {
  int size_;
  std::array<int, kMaxSize> dense_;
  std::array<int, kMaxValue> sparse_;

  SparseIntSet() : size_(0) {}

  // O(1)
  bool contains(int x) const {
    // May access uninitialized memory, but safe.
    const size_t i = sparse_[x];
    return i < (size_t)size_ and dense_[i] == x;
  }

  // O(1)
  void insert(int x) {
    if (contains(x)) return;
    dense_[size_] = x;
    sparse_[x] = size_++;
  }

  // O(1)
  void erase(int x) {
    const int i = sparse_[x];
    if (i < 0 or i >= size_ or dense_[i] != x) return;
    const auto y = dense_[--size_];
    dense_[i] = y;
    sparse_[y] = i;
  }

  // O(1)
  inline void clear() { size_ = 0; }
  inline int size() const { return size_; }
  inline bool empty() const { return size_ == 0; }

  // O(size)
  template <class F>
  void for_each(F func) {
    for (int i = 0; i < size_; ++i) {
      func(dense_[i]);
    }
  }

  // For silencing warnings on accessing uninitialized memory.
  void safe_init() {
    dense_.fill(0);
    sparse_.fill(0);
  }
};
using IntSet = SparseIntSet<200'005, 200'005>;
