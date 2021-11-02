#include <bits/stdc++.h>
using namespace std;
using Int = long long;

template <typename Monoid>
struct StaticSegmentTree2d {
  using T = typename Monoid::T;
  int offset_;
  vector<SegmentTree<Monoid>> segs_;

  // [row_id][col_id] => (y, x)
  vector<vector<pair<Int, Int>>> yx_;
  // [(x, y)] sorted in ascending order
  vector<pair<Int, Int>> xy_;

  // [(x,y) => value]
  explicit StaticSegmentTree2d(const vector<pair<Int, Int>> &xy,
                               const vector<T> &vals)
      : offset_(1), xy_(xy.size()) {
    const int n = (int)xy.size();  // number of data points
    while (offset_ < n) offset_ *= 2;
    for (int i = 0; i < n; ++i) {
      xy_[i] = {xy[i].first, i};
    }
    std::sort(ALL(xy_),
              [&](const auto &p1, const auto &p2) {  // Compare by X, then Y.
                return std::tie(p1.first, xy[p1.second].second) <
                       std::tie(p2.first, xy[p2.second].second);
              });
    yx_.resize(2 * offset_ - 1), segs_.resize(2 * offset_ - 1);
    // Build leaves.
    for (int i = 0; i < n; ++i) {
      // Temporarily fill yx_ with (row_id, x).
      yx_[i + offset_ - 1] = {{xy_[i].second, xy_[i].first}};
      vector<T> row_data = {vals[xy_[i].second]};
      segs_[i + offset_ - 1] = SegmentTree<Monoid>{std::move(row_data)};
      xy_[i].second = xy[xy_[i].second].second;
    }
    // Build inner nodes.
    for (int i = offset_ - 2; i >= 0; --i) {
      yx_[i].resize(yx_[2 * i + 1].size() + yx_[2 * i + 2].size());
      if (yx_[i].empty()) continue;
      std::merge(ALL(yx_[2 * i + 1]), ALL(yx_[2 * i + 2]), yx_[i].begin(),
                 [&](const auto &p1, const auto &p2) {
                   return std::tie(xy[p1.first].second, p1.second) <
                          std::tie(xy[p2.first].second, p2.second);
                 });
      vector<T> row_data(yx_[i].size());
      for (int j = 0; j < (int)yx_[i].size(); ++j) {
        row_data[j] = vals[yx_[i][j].first];
      }
      segs_[i] = SegmentTree<Monoid>{std::move(row_data)};
    }

    // Set first elements of yx_ to y.
    for (int i = 0; i < 2 * offset_ - 1; ++i) {
      for (auto &[y, x] : yx_[i]) {
        y = xy[y].second;
      }
    }
  }

  // Query a rectangle: top_left x bottom_right.
  T fold(const Int tlx, const Int tly, const Int brx, const Int bry) {
    const int qtid =
        lower_bound(ALL(xy_), pair{tlx, numeric_limits<Int>::min()}) -
        xy_.begin();
    const int qbid =
        upper_bound(ALL(xy_), pair{brx, numeric_limits<Int>::min()}) -
        xy_.begin();
    return (qtid >= qbid) ? Monoid::id()
                          : fold_x(qtid, qbid, tly, bry, 0, 0, offset_);
  }

  T get(Int x, Int y) { return fold(x, y, x + 1, y + 1); }

  void set(Int x, Int y, T val) {
    int id = lower_bound(ALL(xy_), pair(x, y)) - xy_.begin();
    return set_x(id, x, y, std::move(val));
  }

 private:
  T fold_x(const int qtid, const int qbid, const Int ql, const Int qr,
           const int k, const int ntid, const int nbid) {
    if (nbid <= qtid or qbid <= ntid) return Monoid::id();
    if (qtid <= ntid and nbid <= qbid) {
      const int qlid =
          lower_bound(ALL(yx_[k]), pair{ql, numeric_limits<Int>::min()}) -
          yx_[k].begin();
      const int qrid =
          upper_bound(ALL(yx_[k]), pair{qr, numeric_limits<Int>::min()}) -
          yx_[k].begin();
      return (qlid >= qrid) ? Monoid::id() : segs_[k].fold(qlid, qrid);
    } else {
      int m = (ntid + nbid) >> 1;
      return Monoid::op(fold_x(qtid, qbid, ql, qr, 2 * k + 1, ntid, m),
                        fold_x(qtid, qbid, ql, qr, 2 * k + 2, m, nbid));
    }
  }

  void set_x(int id, Int x, Int y, T val) {
    id += offset_ - 1;
    int yid = lower_bound(ALL(yx_[id]), pair(y, x)) - yx_[id].begin();
    segs_[id].set(yid, val);
    while (id > 0) {
      id = (id - 1) / 2;
      yid = lower_bound(ALL(yx_[id]), pair(y, x)) - yx_[id].begin();
      segs_[id].set(yid, val);
    }
  }
};
