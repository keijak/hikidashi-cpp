#include <bits/stdc++.h>

template <typename T>
constexpr int num_bits = CHAR_BIT * sizeof(T);

// Most Significant Set Bit (Highest One Bit)
inline int mssb_pos(unsigned x) {
  assert(x != 0);
  return std::numeric_limits<unsigned>::digits - __builtin_clz(x) - 1;
}
inline int mssb_pos(unsigned long long x) {
  assert(x != 0);
  return std::numeric_limits<unsigned long long>::digits - __builtin_clzll(x) -
         1;
}

template <class T, class BetterOp = std::less<T>, class mask_t = uint32_t>
struct RMQ {
  static_assert(std::is_integral<mask_t>::value, "mask_t must be integral");
  static_assert(std::is_unsigned<mask_t>::value, "mask_t must be unsigned");
  static_assert(std::is_invocable_r_v<bool, BetterOp, const T &, const T &>);
  static constexpr int block_size_ = num_bits<mask_t>;

  int n_;                       // sequence size
  int block_count_;             // total number of blocks
  const std::vector<T> &data_;  // sequence data
  std::vector<mask_t> indicator_;
  std::vector<std::vector<int>> sparse_table_;
  BetterOp better_than_;  // checks if lhs is strictly better than rhs.

  explicit RMQ(const std::vector<T> &data, BetterOp better = BetterOp())
      : n_(data.size()),
        block_count_((n_ + block_size_ - 1) / block_size_),
        data_(std::move(data)),
        indicator_(n_),
        sparse_table_(
            block_count_ == 0 ? 0 : mssb_pos(unsigned(block_count_)) + 1,
            std::vector<int>(block_count_)),
        better_than_(std::move(better)) {
    static constexpr int bufsize = block_size_ + 1;
    std::array<int, bufsize> buf;            // ring buffer [lp,rp)
    int lp = 0, rp = 0, rpm1 = bufsize - 1;  // rpm1 = rp-1 (mod bufsize)
    for (int r = 0; r < n_; ++r) {
      while (lp != rp and (r - buf[lp] >= block_size_)) {
        ++lp;
        if (lp == bufsize) lp = 0;
      }
      const auto &cur = data_[r];
      while (lp != rp and not better_than_(data_[buf[rpm1]], cur)) {
        rp = rpm1--;
        if (rp == 0) rpm1 = bufsize - 1;
      }
      mask_t ind = 1;
      if (lp != rp) {
        int p = buf[rpm1];
        ind |= (indicator_[p] << (r - p));
      }
      indicator_[r] = ind;
      buf[rp] = r;
      rpm1 = rp++;
      if (rp == bufsize) rp = 0;
    }

    for (int i = 0; i < block_count_; ++i) {
      sparse_table_[0][i] =
          best_index_small(std::min(block_size_ * (i + 1), n_) - 1);
    }
    for (int i = 0; i + 1 < int(sparse_table_.size()); ++i) {
      for (int j = 0; j < block_count_; j++) {
        sparse_table_[i + 1][j] = better_index(
            sparse_table_[i][j],
            sparse_table_[i][std::min(j + (1 << i), block_count_ - 1)]);
      }
    }
  }

  // Returns the best value in [l, r) (half-open interval).
  inline const T &query(int l, int r) const {
    return data_[best_index(l, r - 1)];
  }

 private:
  inline int better_index(int i, int j) const {
    return better_than_(data_[i], data_[j]) ? i : j;
  }

  // Returns the index of the best value in [r - sz, r] (closed interval).
  inline int best_index_small(int r, int sz = block_size_) const {
    assert(r < n_);
    assert(sz > 0);
    assert(sz <= block_size_);
    mask_t ind = indicator_[r];
    if (sz < block_size_) {
      ind &= (mask_t(1) << sz) - 1;
    }
    return r - mssb_pos(ind);
  }

  // Returns the index of the best value in [l, r] (closed interval).
  inline int best_index(int l, int r) const {
    l = std::clamp(l, 0, n_ - 1);
    r = std::clamp(r, 0, n_ - 1);
    if (r - l + 1 <= block_size_) {
      return best_index_small(r, r - l + 1);
    }
    int ql = best_index_small(std::min(l + block_size_, n_) - 1);
    int qr = best_index_small(r);
    l = l / block_size_ + 1;
    r = r / block_size_ - 1;
    int ans = better_index(ql, qr);
    if (l <= r) {
      int i = mssb_pos(unsigned(r - l + 1));
      int qs =
          better_index(sparse_table_[i][l], sparse_table_[i][r - (1 << i) + 1]);
      ans = better_index(ans, qs);
    }
    return ans;
  }
};
