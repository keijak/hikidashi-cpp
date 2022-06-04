#include <bits/stdc++.h>

template <class Semigroup>
struct DisjointSparseTable {
  using value_type = typename Semigroup::T;
  using size_type = size_t;
  std::vector<std::vector<value_type>> table;

  explicit DisjointSparseTable(const std::vector<value_type>& seq) {
    const size_type size = seq.size();
    table.reserve(32);
    table.push_back(seq);  // table[0] = copy of the original sequence
    for (size_type i = 2; i < size; i <<= 1) {
      std::vector<value_type> v;
      v.reserve(size);
      for (size_type j = i; j < size; j += i << 1) {
        v.push_back(seq[j - 1]);
        for (size_type k = 2; k <= i; ++k) {
          v.push_back(Semigroup::op(seq[j - k], v.back()));
        }
        v.push_back(seq[j]);
        for (size_type k = 1; k < i && j + k < size; ++k) {
          v.push_back(Semigroup::op(v.back(), seq[j + k]));
        }
      }
      table.push_back(std::move(v));
    }
  }

  int size() const { return table.empty() ? 0 : (int)table.front().size(); }

  bool empty() const { return size() == 0; }

  // Folds the range [first, last). O(1).
  value_type fold(int first, int last) const {
    assert(0 <= first and first < last and last <= size());
    if (first + 1 == last) {
      return table.front()[first];
    } else {
      const size_type p = msb_log(first ^ (last - 1));
      return Semigroup::op(table[p][first ^ (size_type(1) << p) - 1],
                           table[p][last - 1]);
    }
  }

  const value_type& operator[](int index) const {
    assert(index < size());
    return table.front()[index];
  }

 private:
  static inline int msb_log(unsigned x) {
    assert(x != 0);
    return std::numeric_limits<unsigned>::digits - __builtin_clz(x) - 1;
  }
};
