struct SwagQueue {
  std::vector<std::pair<i64, i64>> s1, s2;

  int size() { return s1.size() + s2.size(); }

  bool isEmpty() { return size() == 0; }

  long long getMax() {
    if (isEmpty()) {
      return std::numeric_limits<long long>::min();
    }
    if (!s1.empty() && !s2.empty()) {
      return std::max(s1.back().second, s2.back().second);
    }
    if (!s1.empty()) {
      return s1.back().second;
    }
    return s2.back().second;
  }

  void push(long long val) {
    if (s2.empty()) {
      s2.push_back(std::pair(val, val));
    } else {
      s2.push_back(std::pair(val, std::max(val, s2.back().second)));
    }
  }

  void pop() {
    if (s1.empty()) {
      while (!s2.empty()) {
        if (s1.empty()) {
          s1.push_back(std::pair(s2.back().first, s2.back().first));
        } else {
          s1.push_back(std::pair(s2.back().first,
                                 std::max(s2.back().first, s1.back().second)));
        }
        s2.pop_back();
      }
    }
    assert(!s1.empty());
    s1.pop_back();
  }
};
