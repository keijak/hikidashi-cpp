#include <bits/stdc++.h>

// Bucketing a sequence data into block ranges.
template <typename T>
struct SqrtDecomp {
  std::vector<T> data_;
  int n_;
  int block_size_;
  int block_count_;
  std::vector<std::map<T, int>> blocks_;

  explicit SqrtDecomp(std::vector<T> data)
      : n_(data.size()), data_(std::move(data)) {
    block_size_ = int(std::sqrt(n_)) + 1;
    block_count_ = (n_ + block_size_ - 1) / block_size_;
    blocks_.resize(block_count_);
    for (int i = 0; i < block_count_; ++i) {
      for (int j = 0; j < block_size_; ++j) {
        int k = i * block_size_ + j;
        blocks_[i][data_[k]]++;
      }
    }
  }

  void query(int l, int r, const T& q) {
    const int lb = l / block_size_;
    const int rb = r / block_size_;
    for (int b = lb + 1; b < rb; ++b) {
      process_block(b, q);
    }
    for (int i = l; i < r;) {
      if (i == (lb + 1) * block_size_) {
        i = rb * block_size_;  // Jump
      } else {
        process_item(i, q);
        ++i;
      }
    }
  }

 private:
  void process_item(int i, const T& q) { return (data_[i] == q) ? 1 : 0; }

  void process_block(int b, const T& q) { return blocks_[b][q]; }
};
