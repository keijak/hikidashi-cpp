#include <bits/stdc++.h>

// Reference:
// https://research.swtch.com/sparse
// https://www.geeksforgeeks.org/sparse-set/

template <int kMaxSize, int kMaxValue>
struct SparseIntSet {
  int size_;
  int dense_[kMaxSize];
  int sparse_[kMaxValue + 1];

  SparseIntSet() noexcept : size_(0) {}

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

  using iterator = int *;
  using const_iterator = const int *;
  iterator begin() { return dense_; }
  const_iterator begin() const { return dense_; }
  iterator end() { return dense_ + size_; }
  const_iterator end() const { return dense_ + size_; }

  // For silencing warnings on accessing uninitialized memory.
  void safe_init() {
    dense_.fill(0);
    sparse_.fill(0);
  }
};
using IntSet = SparseIntSet<200'005, 200'005>;
