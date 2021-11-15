#include <bits/stdc++.h>

// Pointer-based 2d segment tree.
// More performant and less memory-efficient than a hashmap-based
// implementation.
template <typename Monoid>
struct SegmentTree2d {
  using Int = long long;
  using T = typename Monoid::T;
  struct Node;
  struct RowNode;
  using NodePtr = Node *;
  using RowNodePtr = RowNode *;

  struct Node {
    T data = Monoid::id();
    NodePtr l = nullptr;
    NodePtr r = nullptr;
    Node() = default;
    explicit Node(T _data) : data(std::move(_data)) {}
  };
  struct RowNode {
    NodePtr col = nullptr;
    RowNodePtr u = nullptr;
    RowNodePtr d = nullptr;
  };

  RowNodePtr root_;
  Int nrow_;
  Int ncol_;

  explicit SegmentTree2d(Int nrow, Int ncol)
      : root_(new RowNode{new Node{Monoid::id()}}), nrow_(nrow), ncol_(ncol) {}

  void set(Int i, Int j, T x) { set_x(i, j, std::move(x), root_, 0, nrow_); }

  T fold(Int iu, Int id, Int jl, Int jr) const {
    return fold_x(iu, id, jl, jr, root_, 0, nrow_);
  }
  T fold_all() const { return root_->col->data; }
  T get(Int i, Int j) const { return fold(i, i + 1, j, j + 1); }

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
  void set_y(Int k, T val, NodePtr np, NodePtr up, NodePtr dp, bool single_row,
             Int nl, Int nr) {
    check(np != nullptr);
    if (k < nl or k >= nr) return;
    if (nl + 1 == nr) {
      if (single_row) {
        np->data = std::move(val);
      } else {
        np->data = Monoid::op(up == nullptr ? Monoid::id() : up->data,
                              dp == nullptr ? Monoid::id() : dp->data);
      }
      return;
    }
    Int nm = (nl + nr) >> 1;
    if (k < nm) {
      if (np->l == nullptr) np->l = new Node{Monoid::id()};
      set_y(k, std::move(val), np->l, (up == nullptr ? nullptr : up->l),
            (dp == nullptr ? nullptr : dp->l), single_row, nl, nm);
    } else {
      if (np->r == nullptr) np->r = new Node{Monoid::id()};
      set_y(k, std::move(val), np->r, (up == nullptr ? nullptr : up->r),
            (dp == nullptr ? nullptr : dp->r), single_row, nm, nr);
    }
    np->data = Monoid::op(np->l == nullptr ? Monoid::id() : np->l->data,
                          np->r == nullptr ? Monoid::id() : np->r->data);
  }

  void set_x(Int i, Int j, T val, RowNodePtr np, Int nu, Int nd) {
    check(np != nullptr);
    if (i < nu or i >= nd) return;
    if (nu + 1 != nd) {
      Int nm = (nu + nd) >> 1;
      if (i < nm) {
        if (np->u == nullptr) np->u = new RowNode{};
        set_x(i, j, val, np->u, nu, nm);
      } else {
        if (np->d == nullptr) np->d = new RowNode{};
        set_x(i, j, val, np->d, nm, nd);
      }
    }
    bool single_row = nu + 1 == nd;
    if (np->col == nullptr) np->col = new Node{};
    set_y(j, std::move(val), np->col, (np->u == nullptr ? nullptr : np->u->col),
          (np->d == nullptr ? nullptr : np->d->col), single_row, 0, ncol_);
  }

  T fold_y(Int jl, Int jr, NodePtr np, Int nl, Int nr) const {
    if (np == nullptr) return Monoid::id();
    if (nr <= jl or jr <= nl) return Monoid::id();
    if (jl <= nl and nr <= jr) return np->data;
    Int nm = (nl + nr) >> 1;
    return Monoid::op(fold_y(jl, jr, np->l, nl, nm),
                      fold_y(jl, jr, np->r, nm, nr));
  }

  T fold_x(Int iu, Int id, Int jl, Int jr, RowNodePtr np, Int nu,
           Int nd) const {
    if (np == nullptr) return Monoid::id();
    if (nd <= iu or id <= nu) return Monoid::id();
    if (iu <= nu and nd <= id) return fold_y(jl, jr, np->col, 0, ncol_);
    Int nm = (nu + nd) >> 1;
    return Monoid::op(fold_x(iu, id, jl, jr, np->u, nu, nm),
                      fold_x(iu, id, jl, jr, np->d, nm, nd));
  }
};
