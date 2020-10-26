template <class T, class Container = std::vector<T>,
          class Compare = std::less<typename Container::value_type>>
struct SlideMinQueue {
  Container vals;
  Compare compare;              // comparison funcation.
  int left, right;              // [left, right)
  std::deque<int> index_queue;  // indices where min values are stored.

  SlideMinQueue() : compare(), left(0), right(0) {}

  explicit SlideMinQueue(Container v)
      : vals(std::move(v)), compare(), left(0), right(0) {}

  SlideMinQueue(const SlideMinQueue &) = default;
  SlideMinQueue &operator=(const SlideMinQueue &) = default;
  SlideMinQueue(SlideMinQueue &&) = default;
  SlideMinQueue &operator=(SlideMinQueue &&) = default;

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
  const T &fold() const {
    assert(!empty());
    return vals[index_queue.front()];
  }

  bool empty() const { return index_queue.empty(); }

 private:
  // Enqueues the i-th value.
  void push_right(int i) {
    while (!index_queue.empty() &&
           !compare(vals[index_queue.back()], vals[i])) {
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

template <class T, class Container = std::vector<T>>
using SlideMaxQueue = SlideMinQueue<T, Container, std::greater<T>>;
