
template <class T, class Container = std::vector<T>,
          class Compare = std::less<typename Container::value_type>>
struct SlideMinQueue {
  Container& values;
  int left;
  int right;
  std::deque<int> que;
  const Compare compare;

  // Initial window: [0, 0) -- half open interval.
  explicit SlideMinQueue(Container& values)
      : values(values), left(0), right(0) {}

  // Shift both `left` and `right` to the right.
  void slide(int l, int r) {
    assert(l >= left);
    assert(r >= right);
    pop_left(l);
    left = l;
    for (int i = right; i < r; ++i) {
      push_right(i);
    }
    right = r;
  }

  // Returns the minimum value in [left, right).
  T fold() const {
    assert(!empty());
    return que.front();
  }

  bool empty() const { return que.empty(); }

 private:
  // Enqueues the i-th value.
  void push_right(int i) {
    while (!que.empty() && !compare(values[que.back()], values[i])) {
      que.pop_back();
    }
    que.emplace_back(i);
  }

  // Dequeues indices less than i.
  void pop_left(int i) {
    while (!que.empty() && que.front() < i) {
      que.pop_front();
    }
  }
};

template <class T, class Container = std::vector<T>>
using SlideMaxQueue = SlideMinQueue<T, Container, greater<T>>;
