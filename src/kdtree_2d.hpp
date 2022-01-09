// k-d tree (https://en.wikipedia.org/wiki/K-d_tree)
// base: https://tjkendev.github.io/procon-library/cpp/range_query/kd-tree.html
#include <bits/stdc++.h>

struct Point {
  using T = long long;
  T x, y;
  int index;

  static T square_distance(T x1, T y1, T x2, T y2) {
    T dx = x1 - x2, dy = y1 - y2;
    return dx * dx + dy * dy;
  }
};

template <class P>
class KdTree2d {
  using T = typename P::T;
  using PItr = typename std::vector<P>::iterator;
  std::vector<P> points_;

  class NodePool;

  class Node {
    std::optional<Point> p;
    Node *left, *right;

   public:
    Node() : p{}, left(nullptr), right(nullptr) {}

    void init(PItr begin, PItr end, int depth, NodePool &pool) {
      if (begin == end) return;

      const int n = int(end - begin);
      auto mid = begin + (n >> 1);

      if (depth & 1) {
        std::nth_element(begin, mid, end,
                         [](const P &a, const P &b) { return a.y < b.y; });
      } else {
        std::nth_element(begin, mid, end,
                         [](const P &a, const P &b) { return a.x < b.x; });
      }
      p = *mid;
      if (begin != mid) {
        left = pool.get();
        left->init(begin, mid, depth + 1, pool);
      }
      if (std::next(mid) != end) {
        right = pool.get();
        right->init(std::next(mid), end, depth + 1, pool);
      }
    }

    // Returns the square distance and the pointer to the nearest neighbor.
    std::pair<T, P *> find_nearest(T x, T y, std::pair<T, P *> radius,
                                   int depth) {
      if (not p) return radius;

      T dist = P::square_distance(x, y, p->x, p->y);
      if (radius.first > dist) radius = std::pair{dist, &*p};

      if (depth & 1) {
        if (left and y - p->y <= radius.first) {
          auto r = left->find_nearest(x, y, radius, depth + 1);
          if (radius.first > r.first) radius = std::move(r);
        }
        if (right and p->y - y <= radius.first) {
          auto r = right->find_nearest(x, y, radius, depth + 1);
          if (radius.first > r.first) radius = std::move(r);
        }
      } else {
        if (left and x - p->x <= radius.first) {
          auto r = left->find_nearest(x, y, radius, depth + 1);
          if (radius.first > r.first) radius = std::move(r);
        }
        if (right and p->x - x <= radius.first) {
          auto r = right->find_nearest(x, y, radius, depth + 1);
          if (radius.first > r.first) radius = std::move(r);
        }
      }
      return radius;
    }
  };
  Node root_;

  class NodePool {
    std::vector<Node> nodes;
    size_t counter;

   public:
    explicit NodePool(size_t n) : nodes(n), counter(0) {}
    inline Node *get() { return &nodes[counter++]; }
  };
  NodePool pool_;

 public:
  // Builds the k-d tree: O(NlogN)
  explicit KdTree2d(std::vector<P> points)
      : points_(std::move(points)), pool_(points_.size() * 2) {
    root_.init(points_.begin(), points_.end(), 0, pool_);
  }

  // Queries the nearest neighbor: O(logN)
  // Returns the square distance and the pointer to the nearest neighbor.
  std::pair<T, P *> find_nearest(T x, T y) {
    return root_.find_nearest(x, y, {std::numeric_limits<T>::max(), nullptr},
                              0);
  }
};
