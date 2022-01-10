// Pointer-based 2d segment tree.
// More performant and less memory-efficient than a hashmap-based
// implementation.

#include <bits/stdc++.h>

template <typename Monoid>
struct SegmentTree2d {
  using Int = long long;
  using T = typename Monoid::T;
  struct Node;
  struct RowNode;
  using NodePtr = Node *;
  using RowNodePtr = RowNode *;

  struct Node {
    T data;
    NodePtr left, right;  // children
    Node() : data(Monoid::id()), left(nullptr), right(nullptr) {}
    explicit Node(T _data)
        : data(std::move(_data)), left(nullptr), right(nullptr) {}
  };
  struct RowNode {
    NodePtr col;
    RowNodePtr lower, upper;  // children
    RowNode(NodePtr c = nullptr) : col(c), lower(nullptr), upper(nullptr) {}
  };

  RowNodePtr root_;
  Int nrow_;
  Int ncol_;

  explicit SegmentTree2d(Int nrow, Int ncol)
      : root_(new RowNode(new Node())), nrow_(nrow), ncol_(ncol) {}

  void set(Int x, Int y, T val) {
    set_x(x, y, std::move(val), root_, 0, nrow_);
  }

  // Query a rectangle: [x_lo, x_hi) x [y_lo, y_hi).
  T fold(Int x_lo, Int x_hi, Int y_lo, Int y_hi) const {
    return fold_x(x_lo, x_hi, y_lo, y_hi, root_, 0, nrow_);
  }
  T fold_all() const { return root_->col->data; }
  T get(Int x, Int y) const { return fold(x, x + 1, y, y + 1); }

  std::vector<std::vector<T>> to_vec(Int nrow = -1, Int ncol = -1) const {
    if (nrow < 0) nrow = nrow_;
    if (ncol < 0) ncol = ncol_;
    nrow = min(nrow, nrow_);
    ncol = min(ncol, ncol_);
    auto res = std::vector(nrow, std::vector(ncol, T{}));
    for (Int i = 0; i < nrow; ++i) {
      for (Int j = 0; j < ncol; ++j) {
        res[i][j] = get(i, j);
      }
    }
    return res;
  }

 private:
  void set_y(Int y, T val, NodePtr node, NodePtr lower, NodePtr upper,
             bool single_row, Int ny_lo, Int ny_hi) {
    assert(node != nullptr);
    if (y < ny_lo or y >= ny_hi) return;
    if (ny_lo + 1 == ny_hi) {
      if (single_row) {
        node->data = std::move(val);
      } else {
        node->data = Monoid::op(lower == nullptr ? Monoid::id() : lower->data,
                                upper == nullptr ? Monoid::id() : upper->data);
      }
      return;
    }
    Int ny_mid = (ny_lo + ny_hi) >> 1;
    if (y < ny_mid) {
      if (node->left == nullptr) node->left = new Node();
      set_y(y, std::move(val), node->left,
            (lower == nullptr ? nullptr : lower->left),
            (upper == nullptr ? nullptr : upper->left), single_row, ny_lo,
            ny_mid);
    } else {
      if (node->right == nullptr) node->right = new Node();
      set_y(y, std::move(val), node->right,
            (lower == nullptr ? nullptr : lower->right),
            (upper == nullptr ? nullptr : upper->right), single_row, ny_mid,
            ny_hi);
    }
    node->data =
        Monoid::op(node->left == nullptr ? Monoid::id() : node->left->data,
                   node->right == nullptr ? Monoid::id() : node->right->data);
  }

  void set_x(Int x, Int y, T val, RowNodePtr node, Int nx_lo, Int nx_hi) {
    assert(node != nullptr);
    if (x < nx_lo or x >= nx_hi) return;
    if (nx_lo + 1 != nx_hi) {
      // Update a child node.
      Int nx_mid = (nx_lo + nx_hi) >> 1;
      if (x < nx_mid) {
        if (node->lower == nullptr) node->lower = new RowNode();
        set_x(x, y, val, node->lower, nx_lo, nx_mid);
      } else {
        if (node->upper == nullptr) node->upper = new RowNode();
        set_x(x, y, val, node->upper, nx_mid, nx_hi);
      }
    }
    // Update this node.
    bool single_row = (nx_lo + 1 == nx_hi);
    if (node->col == nullptr) node->col = new Node();
    set_y(y, std::move(val), node->col,
          (node->lower == nullptr ? nullptr : node->lower->col),
          (node->upper == nullptr ? nullptr : node->upper->col), single_row, 0,
          ncol_);
  }

  T fold_y(Int y_lo, Int y_hi, NodePtr node, Int ny_lo, Int ny_hi) const {
    if (node == nullptr) return Monoid::id();
    if (ny_hi <= y_lo or y_hi <= ny_lo) return Monoid::id();
    if (y_lo <= ny_lo and ny_hi <= y_hi) return node->data;
    Int ny_mid = (ny_lo + ny_hi) >> 1;
    return Monoid::op(fold_y(y_lo, y_hi, node->left, ny_lo, ny_mid),
                      fold_y(y_lo, y_hi, node->right, ny_mid, ny_hi));
  }

  T fold_x(Int x_lo, Int x_hi, Int y_lo, Int y_hi, RowNodePtr node, Int nx_lo,
           Int nx_hi) const {
    if (node == nullptr) return Monoid::id();
    if (nx_hi <= x_lo or x_hi <= nx_lo) return Monoid::id();
    if (x_lo <= nx_lo and nx_hi <= x_hi) {
      return fold_y(y_lo, y_hi, node->col, 0, ncol_);
    }
    Int nx_mid = (nx_lo + nx_hi) >> 1;
    return Monoid::op(
        fold_x(x_lo, x_hi, y_lo, y_hi, node->lower, nx_lo, nx_mid),
        fold_x(x_lo, x_hi, y_lo, y_hi, node->upper, nx_mid, nx_hi));
  }
};
