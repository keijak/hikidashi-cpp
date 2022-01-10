// Offline 2D point-update range-fold
// All (x, y) coordinates must be known in advance.
// No need to compress X and Y coordinates.
// Verified with: https://judge.yosupo.jp/problem/point_add_rectangle_sum

#include <bits/stdc++.h>
using namespace std;
using Int = long long;

template <typename Monoid>
class SegmentTree;

template <typename Monoid>
struct SegmentTree2d {
  using T = typename Monoid::T;
  int offset_;
  vector<SegmentTree<Monoid>> segs_;

  // [row_id][col_id] => (y, x)
  vector<vector<pair<Int, Int>>> yx_;
  // [(x, y)] sorted in ascending order
  vector<pair<Int, Int>> xy_;

  // [(x,y) => value]
  explicit SegmentTree2d(const map<pair<Int, Int>, T> &data)
      : offset_(1), xy_(data.size()) {
    const int n = (int)data.size();  // number of data points
    while (offset_ < n) offset_ *= 2;

    vector<const pair<Int, Int> *> coords(n);
    vector<const T *> vals(n);
    {
      int i = 0;
      for (auto it = data.begin(); it != data.end(); ++it, ++i) {
        coords[i] = &it->first;
        vals[i] = &it->second;
        // Temporarily fill xy_ with (x, coord_id).
        xy_[i] = {it->first.first, i};
      }
    }

    std::sort(ALL(xy_),
              [&](const auto &p1, const auto &p2) {  // Compare by X, then Y.
                return std::tie(p1.first, coords[p1.second]->second) <
                       std::tie(p2.first, coords[p2.second]->second);
              });
    yx_.resize(2 * offset_ - 1);
    segs_.resize(2 * offset_ - 1, SegmentTree<Monoid>(0));
    // Build leaves.
    for (int i = 0; i < n; ++i) {
      // Temporarily fill yx_ with (coord_id, x).
      yx_[i + offset_ - 1] = {{xy_[i].second, xy_[i].first}};
      vector<T> row_data = {*vals[xy_[i].second]};
      segs_[i + offset_ - 1] = SegmentTree<Monoid>{std::move(row_data)};
      // Set the second elements of xy_ to y.
      xy_[i].second = coords[xy_[i].second]->second;
    }
    // Build inner nodes.
    for (int i = offset_ - 2; i >= 0; --i) {
      yx_[i].resize(yx_[2 * i + 1].size() + yx_[2 * i + 2].size());
      if (yx_[i].empty()) continue;
      std::merge(ALL(yx_[2 * i + 1]), ALL(yx_[2 * i + 2]), yx_[i].begin(),
                 [&](const auto &p1, const auto &p2) {
                   return std::tie(coords[p1.first]->second, p1.second) <
                          std::tie(coords[p2.first]->second, p2.second);
                 });
      vector<T> row_data(yx_[i].size());
      for (int j = 0; j < (int)yx_[i].size(); ++j) {
        row_data[j] = *vals[yx_[i][j].first];
      }
      segs_[i] = SegmentTree<Monoid>{std::move(row_data)};
    }

    // Set the first elements of yx_ to y.
    for (int i = 0; i < 2 * offset_ - 1; ++i) {
      for (auto &[y, x] : yx_[i]) {
        y = coords[y]->second;
      }
    }
  }

  // Query a rectangle: [x_lo, x_hi) x [y_lo, y_hi).
  T fold(Int x_lo, Int x_hi, Int y_lo, Int y_hi) const {
    const int xid_lo =
        std::lower_bound(ALL(xy_),
                         pair{x_lo, std::numeric_limits<Int>::min()}) -
        xy_.begin();
    const int xid_hi =
        std::lower_bound(ALL(xy_),
                         pair{x_hi, std::numeric_limits<Int>::min()}) -
        xy_.begin();
    if (xid_lo >= xid_hi) return Monoid::id();
    return fold_x(xid_lo, xid_hi, y_lo, y_hi, 0, 0, offset_);
  }

  T get(Int x, Int y) const {
    auto key = std::pair(x, y);
    auto it = std::lower_bound(ALL(xy_), key);
    if (it == xy_.end() or *it != key) return Monoid::id();
    int xid = int(it - xy_.begin()) + offset_ - 1;
    int yid = std::lower_bound(ALL(yx_[xid]), pair(y, x)) - yx_[xid].begin();
    return segs_[xid][yid];
  }

  // Requirement: (x, y) must be one of existing points.
  // Cannot insert a new point.
  void set(Int x, Int y, T val) {
    auto key = std::pair(x, y);
    auto it = std::lower_bound(ALL(xy_), key);
    assert(it != xy_.end() and *it == key);  // must be found
    int xid = it - xy_.begin();
    return set_x(xid, x, y, std::move(val));
  }

 private:
  T fold_x(int xid_lo, int xid_hi, Int y_lo, Int y_hi, int k, int nxid_lo,
           int nxid_hi) const {
    if (nxid_hi <= xid_lo or xid_hi <= nxid_lo) return Monoid::id();
    if (xid_lo <= nxid_lo and nxid_hi <= xid_hi) {
      const int yid_lo =
          lower_bound(ALL(yx_[k]),
                      pair{y_lo, std::numeric_limits<Int>::min()}) -
          yx_[k].begin();
      const int yid_hi =
          lower_bound(ALL(yx_[k]),
                      pair{y_hi, std::numeric_limits<Int>::min()}) -
          yx_[k].begin();
      return (yid_lo >= yid_hi) ? Monoid::id() : segs_[k].fold(yid_lo, yid_hi);
    } else {
      int m = (nxid_lo + nxid_hi) >> 1;
      return Monoid::op(
          fold_x(xid_lo, xid_hi, y_lo, y_hi, 2 * k + 1, nxid_lo, m),
          fold_x(xid_lo, xid_hi, y_lo, y_hi, 2 * k + 2, m, nxid_hi));
    }
  }

  void set_x(int xid, Int x, Int y, T val) {
    xid += offset_ - 1;
    int yid = std::lower_bound(ALL(yx_[xid]), pair(y, x)) - yx_[xid].begin();
    segs_[xid].set(yid, val);
    while (xid > 0) {
      xid = (xid - 1) / 2;
      yid = std::lower_bound(ALL(yx_[xid]), pair(y, x)) - yx_[xid].begin();
      segs_[xid].set(yid, val);
    }
  }
};
