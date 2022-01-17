#include <bits/stdc++.h>

template <typename T>
struct Array2d {
  std::vector<T> data_;
  size_t h_, w_;

  Array2d(size_t h, size_t w) : data_(h * w, T{}), h_(h), w_(w) {}

  inline T &get(size_t i0, size_t i1) { return data_[i0 * w_ + i1]; }

  // Fills one row with the specified value.
  inline void fill(size_t i0, T val) {
    std::fill(data_.begin() + i0 * w_, data_.begin() + (i0 + 1) * w_, val);
  }
};

template <typename T>
struct Array3d {
  std::vector<T> data_;
  size_t b0_ = 0, b1_ = 0;

  Array3d(size_t d0, size_t d1, size_t d2)
      : data_(d0 * d1 * d2, T{}), b0_(d1 * d2), b1_(d2) {}

  inline T &get(size_t i0, size_t i1, size_t i2) {
    return data_[i0 * b0_ + i1 * b1_ + i2];
  }
  // Fills one row with the specified value.
  inline void fill(size_t i0, T val) {
    std::fill(data_.begin() + i0 * b0_, data_.begin() + (i0 + 1) * b0_, val);
  }
};

template <typename T>
struct Array4d {
  std::vector<T> data_;
  size_t b0_ = 0, b1_ = 0, b2_ = 0;

  Array4d(size_t d0, size_t d1, size_t d2, size_t d3)
      : data_(d0 * d1 * d2 * d3, T{}),
        b0_(d1 * d2 * d3),
        b1_(d2 * d3),
        b2_(d3) {}

  inline T &get(size_t i0, size_t i1, size_t i2, size_t i3) {
    return data_[i0 * b0_ + i1 * b1_ + i2 * b2_ + i3];
  }
  // Fills one row with the specified value.
  inline void fill(size_t i0, T val) {
    std::fill(data_.begin() + i0 * b0_, data_.begin() + (i0 + 1) * b0_, val);
  }
};

template <typename T>
struct Array5d {
  std::vector<T> data_;
  size_t b0_ = 0, b1_ = 0, b2_ = 0, b3_ = 0;

  Array5d(size_t d0, size_t d1, size_t d2, size_t d3, size_t d4)
      : data_(d0 * d1 * d2 * d3 * d4, T{}),
        b0_(d1 * d2 * d3 * d4),
        b1_(d2 * d3 * d4),
        b2_(d3 * d4),
        b3_(d4) {}
  inline T &get(size_t i0, size_t i1, size_t i2, size_t i3, size_t i4) {
    return data_[i0 * b0_ + i1 * b1_ + i2 * b2_ + i3 * b3_ + i4];
  }
  // Fills one row with the specified value.
  inline void fill(size_t i0, T val) {
    std::fill(data_.begin() + i0 * b0_, data_.begin() + (i0 + 1) * b0_, val);
  }
};

// N-dimensional array.
template <typename T, int dim>
struct NdArray {
  std::vector<T> data_;
  std::array<int, dim> shape_;
  std::array<int, dim> base_;

  NdArray(const std::array<int, dim> &shape, T init = T{})
      : data_(std::accumulate(shape.begin(), shape.end(), size_t(1),
                              [](size_t x, size_t y) { return x * y; }),
              std::move(init)),
        shape_(shape) {
    size_t b = 1;
    for (int j = dim - 1; j >= 0; --j) {
      base_[j] = b;
      b *= shape_[j];
    }
  }

  T &operator[](const std::array<int, dim> &index) {
    size_t pos = 0;
    for (int j = dim - 1; j >= 0; --j) {
      pos += index[j] * base_[j];
    }
    return data_[pos];
  }

  // Fills one row with the specified value.
  inline void fill(int i0, T val) {
    std::fill(data_.begin() + i0 * base_[0],
              data_.begin() + (i0 + 1) * base_[0], val);
  }
};
