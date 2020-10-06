class IntervalSet {
 public:
  IntervalSet() : length_sum_(0) {}
  IntervalSet(const IntervalSet &other) = default;
  IntervalSet(IntervalSet &&other)
      : set_(std::move(other.set_)), length_sum_(other.length_sum_) {}
  IntervalSet &operator=(const IntervalSet &other) = default;
  IntervalSet &operator=(IntervalSet &&other) {
    set_ = std::move(other.set_);
    length_sum_ = other.length_sum_;
    return *this;
  }

  i64 length_sum() const { return length_sum_; }

  int count() const { return set_.size(); }

  const std::set<std::pair<i64, i64>> &get() const { return set_; }

  // Adds an interval. It's merged with all existing intervals.
  // [istart, iend) - right-open interval
  void emplace(i64 istart, i64 iend) {
    // New interval to be inserted.
    std::pair<i64, i64> inew = {iend, istart};

    auto it1 = set_.upper_bound({istart, 0});
    if (it1 != set_.end() && it1->second <= istart) {
      if (it1->first >= iend) return;
      inew.second = it1->second;
    }

    auto it2 = set_.lower_bound({iend, 0});
    if (it2 != set_.end() && it2->second < iend) {
      if (it2->second <= istart) return;
      inew.first = it2->first;
      ++it2;
    }

    for (auto it = it1; it != it2; ++it) {
      length_sum_ -= it->first - it->second;
    }
    set_.erase(it1, it2);

    set_.insert(inew);
    length_sum_ += inew.first - inew.second;
  }

  // Returns true if the point is included in an interval in the set.
  bool contains(i64 point) const {
    auto it = set_.upper_bound({point, 0});
    if (it == set_.end()) return false;
    return (it->second <= point);
  }

 private:
  // Set of disjoint right-open intervals.
  std::set<std::pair<i64, i64>> set_;  // {{end, start}}

  // Sum of all interval lengths.
  i64 length_sum_;
};
