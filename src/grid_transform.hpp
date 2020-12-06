#include <vector>
using std::vector;

template <typename T>
void rotate90(vector<vector<T>> &grid) {
  int n = grid.size();
  if (n == 0) return;
  int m = grid[0].size();
  if (m == 0) return;
  auto tmp = vector(m, vector(n, grid[0][0]));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      tmp[m - 1 - j][i] = grid[i][j];
    }
  }
  grid = std::move(tmp);
}

template <typename T>
void transpose(vector<vector<T>> &grid) {
  int n = grid.size();
  if (n == 0) return;
  int m = grid[0].size();
  if (m == 0) return;
  auto tmp = vector(m, vector(n, grid[0][0]));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      tmp[j][i] = grid[i][j];
    }
  }
  grid = std::move(tmp);
}

template <typename T>
void flip_horizontally(vector<vector<T>> &grid) {
  int n = grid.size();
  if (n == 0) return;
  int m = grid[0].size();
  if (m == 0) return;
  auto tmp = grid;
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      grid[i][m - 1 - j] = tmp[i][j];
    }
  }
}

template <typename T>
void flip_vertically(vector<vector<T>> &grid) {
  int n = grid.size();
  if (n == 0) return;
  int m = grid[0].size();
  if (m == 0) return;
  auto tmp = grid;
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      grid[n - 1 - i][j] = tmp[i][j];
    }
  }
}
