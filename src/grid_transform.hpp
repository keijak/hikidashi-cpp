#include <vector>
using std::vector;

// Note: Don't forget to update variables H and W.
template <typename T>
void rotate90(vector<vector<T>> &grid) {
  const int h = grid.size();
  if (h == 0) return;
  const int w = grid[0].size();
  if (w == 0) return;
  auto tmp = vector(w, vector(h, T()));
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      tmp[w - 1 - j][i] = grid[i][j];
    }
  }
  std::swap(grid, tmp);
}

template <typename T>
void rotate180(vector<vector<T>> &grid) {
  const int h = grid.size();
  if (h == 0) return;
  const int w = grid[0].size();
  if (w == 0) return;
  auto tmp = vector(w, vector(h, T()));
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      tmp[h - 1 - i][w - 1 - j] = grid[i][j];
    }
  }
  std::swap(grid, tmp);
}

// Note: Don't forget to update variables H and W.
template <typename T>
void transpose(vector<vector<T>> &grid) {
  const int h = grid.size();
  if (h == 0) return;
  const int w = grid[0].size();
  if (w == 0) return;
  auto tmp = vector(w, vector(h, T()));
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      tmp[j][i] = grid[i][j];
    }
  }
  std::swap(grid, tmp);
}

template <typename T>
void flip_horizontally(vector<vector<T>> &grid) {
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

template <typename T>
void flip_vertically(vector<vector<T>> &grid) {
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
