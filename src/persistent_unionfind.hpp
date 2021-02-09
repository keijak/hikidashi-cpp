
class PersistentUnionFind {
 public:
  explicit PersistentUnionFind(PersistentArray<int> par)
      : parent_{std::move(par)} {}
  PersistentUnionFind() = default;
  PersistentUnionFind(const PersistentUnionFind &) = default;
  PersistentUnionFind(PersistentUnionFind &&) = default;
  PersistentUnionFind &operator=(const PersistentUnionFind &) = default;
  PersistentUnionFind &operator=(PersistentUnionFind &&) = default;

  PersistentUnionFind unite(int x, int y) const {
    x = find(x);
    y = find(y);
    if (x == y) return *this;
    const int x_size = -parent_[x].value_or(-1);
    const int y_size = -parent_[y].value_or(-1);
    // Ensure |x| >= |y|.
    if (x_size < y_size) std::swap(x, y);
    int x_size = x_size + y_size;  // or x_rank = max(x_rank, y_rank+1)
    return PersistentUnionFind(parent_.set(x, -united_size).set(y, x));
  }

  int find(int x) const {
    const std::optional<int> &par = parent_[x];
    if (not par or *par < 0) return x;
    return find(*par);
  }

  bool same(int x, int y) const { return find(x) == find(y); }

  int size(int x) const {
    int root = find(x);
    return -(parent_[root].value_or(-1));
  }

 private:
  PersistentArray<int> parent_;
};
