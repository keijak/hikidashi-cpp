
class PersistentUnionFind {
 public:
  explicit PersistentUnionFind(PersistentArray<int> data)
      : data_{std::move(data)} {}
  PersistentUnionFind() : data_{} {}
  PersistentUnionFind(const PersistentUnionFind &) = default;
  PersistentUnionFind(PersistentUnionFind &&) = default;
  PersistentUnionFind &operator=(const PersistentUnionFind &) = default;
  PersistentUnionFind &operator=(PersistentUnionFind &&) = default;

  PersistentUnionFind unite(int x, int y) const {
    x = find(x);
    y = find(y);
    if (x == y) return *this;
    // Ensure x is bigger than y.
    int x_size = -data_[x].value_or(-1);
    int y_size = -data_[y].value_or(-1);
    if (x_size < y_size) {
      std::swap(x, y);
      std::swap(x_size, y_size);
    }
    return PersistentUnionFind(data_.set(x, -(x_size + y_size)).set(y, x));
  }

  int find(int x) const {
    const std::optional<int> &par = data_[x];
    if (not par or *par < 0) return x;
    return find(*par);
  }

  bool same(int x, int y) const { return find(x) == find(y); }

  int size(int x) const {
    int r = find(x);
    assert(data_[r]);
    int res = -*data_[r];
    assert(res > 0);
    return res;
  }

 private:
  PersistentArray<int> data_;
};
