#include <bits/stdc++.h>

template <class T, class Compare = std::less<T>>
struct SlideMinQueue {
  const std::vector<T> &vals;   // reference to static values.
  Compare compare;              // comparison funcation.
  int left, right;              // [left, right)
  std::deque<int> index_queue;  // indices where min values are stored.

  SlideMinQueue() : compare(), left(0), right(0) {}

  explicit SlideMinQueue(const std::vector<T> &v)
      : vals(v), compare(), left(0), right(0) {}

  // Shifts the window to the right.
  // Sets `left` to `l`, and `right` to `r`.
  void slide(int l, int r) {
    l = std::max(l, 0);
    r = std::min(r, (int)vals.size());
    assert(l >= left);
    assert(r >= right);
    for (int i = right; i < r; ++i) {
      push_right(i);
    }
    right = r;
    pop_left(l);
    left = l;
  }

  // Returns the minimum value in [left, right).
  std::optional<int> fold_index() const {
    if (empty()) return std::nullopt;
    return index_queue.front();
  }

  bool empty() const { return index_queue.empty(); }

 private:
  // Enqueues the i-th value.
  void push_right(int i) {
    while (!index_queue.empty() && compare(vals[i], vals[index_queue.back()])) {
      index_queue.pop_back();
    }
    index_queue.emplace_back(i);
  }

  // Dequeues indices less than i.
  void pop_left(int i) {
    while (!index_queue.empty() && index_queue.front() < i) {
      index_queue.pop_front();
    }
  }
};

template <class T>
using SlideMaxQueue = SlideMinQueue<T, std::greater<T>>;
