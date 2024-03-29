#include <bits/stdc++.h>

template <typename T>
struct Array2d {
  std::vector<T> data_;
  size_t h_, w_;
  bool transposed_ = false;
  int rotate_ = 0;

  Array2d(size_t h, size_t w) : data_(h * w, T{0}), h_(h), w_(w) {}

  inline T &get(size_t i, size_t j) {
    int h = h_, w = w_;
    REP(k, rotate_) {
      size_t tmp = i;
      i = j;
      j = h - 1 - tmp;
      swap(h, w);
    }
    if (transposed_) {
      swap(i, j);
      swap(h, w);
    }
    return data_[i * w + j];
  }

  void rotate90(int k = 1) { rotate_ = (rotate_ + k + 4) % 4; }

  void transpose() {
    transposed_ = !transposed_;
    rotate_ = (4 - rotate_) % 4;
  }

  void flip_v() {
    transpose();
    rotate90(1);
  }

  void flip_h() {
    transpose();
    rotate90(-1);
  }
};

auto make_neighbors(int H, int W) {
  static const int dx[4] = {1, 0, -1, 0};
  static const int dy[4] = {0, 1, 0, -1};
  auto inside = [&](int i, int j) {
    return 0 <= i and i < H and 0 <= j and j < W;
  };
  auto neighbors = [&](int x, int y) {
    std::vector<std::pair<int, int>> ret;
    ret.reserve(4);
    for (int d = 0; d < 4; ++d) {
      const int nx = x + dx[d];
      const int ny = y + dy[d];
      if (not inside(nx, ny)) continue;
      ret.emplace_back(nx, ny);
    }
    return ret;
  };
  return std::pair{inside, neighbors};
}

template <typename V>
void transpose(std::vector<V> &grid) {
  const int h = grid.size();
  if (h == 0) return;
  const int w = grid[0].size();
  if (w == 0) return;
  std::vector<V> tmp(w, V(h, grid[0][0]));
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      tmp[j][i] = grid[i][j];
    }
  }
  std::swap(grid, tmp);
}
template <typename V>
void transpose(std::vector<V> &grid, int &H, int &W) {
  transpose(grid);
  std::swap(H, W);
}

template <typename V>
void rotate90(std::vector<V> &grid) {
  const int h = grid.size();
  if (h == 0) return;
  const int w = grid[0].size();
  if (w == 0) return;
  std::vector<V> tmp(w, V(h, grid[0][0]));
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      tmp[w - 1 - j][i] = grid[i][j];
    }
  }
  std::swap(grid, tmp);
}
template <typename V>
void rotate90(std::vector<V> &grid, int &H, int &W) {
  rotate90(grid);
  std::swap(H, W);
}

template <typename V>
void rotate180(std::vector<V> &grid) {
  const int h = grid.size();
  if (h == 0) return;
  const int w = grid[0].size();
  if (w == 0) return;
  auto tmp = grid;
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      tmp[h - 1 - i][w - 1 - j] = grid[i][j];
    }
  }
  std::swap(grid, tmp);
}

// Swaps i-th column and (W-1-i)-th column.
template <typename V>
void flip_horizontally(std::vector<V> &grid) {
  const int h = grid.size();
  if (h == 0) return;
  const int w = grid[0].size();
  if (w == 0) return;
  for (int i = 0; i < h; ++i) {
    for (int j = 0;; ++j) {
      const int r = w - 1 - j;
      if (r <= j) break;
      std::swap(grid[i][j], grid[i][r]);
    }
  }
}

// Swaps i-th row and (H-1-i)-th row.
template <typename V>
void flip_vertically(std::vector<V> &grid) {
  const int h = grid.size();
  if (h == 0) return;
  const int w = grid[0].size();
  if (w == 0) return;
  for (int i = 0;; ++i) {
    const int r = h - 1 - i;
    if (r <= i) break;
    std::swap(grid[i], grid[r]);
  }
}
