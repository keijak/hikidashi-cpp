#include <vector>

struct UnionFind {
  int n;
  mutable std::vector<int> par;  // positive: parent, negative: size
  int num_roots;

  explicit UnionFind(int sz) : n(sz), par(sz, -1), num_roots(sz) {}

  bool unite(int x, int y) {
    x = find(x), y = find(y);
    if (x == y) return false;
    if (par[x] > par[y]) std::swap(x, y);
    par[x] += par[y];
    par[y] = x;
    --num_roots;
    return true;
  }

  int find(int k) const {
    if (par[k] < 0) return k;
    return par[k] = find(par[k]);
  }

  int size(int k) const { return -par[find(k)]; }

  bool same(int x, int y) const { return find(x) == find(y); }

  std::vector<int> roots() const {
    std::vector<int> res;
    res.reserve(num_roots);
    for (int i = 0; i < (int)par.size(); ++i) {
      if (par[i] < 0) res.push_back(i);
    }
    return res;
  }
};

template <typename Abelian>  // Abelian Monoid
struct AggregatableUnionFind : public UnionFind {
  using T = typename Abelian::T;
  std::vector<T> agg_data;

  explicit AggregatableUnionFind(std::vector<T> data)
      : UnionFind(data.size()), agg_data(std::move(data)) {}

  bool unite(int x, int y) {
    int rx = this->find(x), ry = this->find(y);
    if (not UnionFind::unite(x, y)) return false;
    int r = this->find(x);
    agg_data[r] = Abelian::op(agg_data[rx], agg_data[ry]);
    return true;
  }

  const T& agg(int x) const { return agg_data[this->find(x)]; }
};

template <typename Abelian>  // Abelian Group
struct UpdatableUnionFind : public AggregatableUnionFind<Abelian> {
  using T = typename Abelian::T;
  std::vector<T> leaf_data;

  explicit UpdatableUnionFind(int sz)
      : AggregatableUnionFind<Abelian>(std::vector<T>(sz, Abelian::id())),
        leaf_data(std::vector<T>(sz, Abelian::id())) {}

  explicit UpdatableUnionFind(std::vector<T> data)
      : AggregatableUnionFind<Abelian>(data), leaf_data(std::move(data)) {}

  void set(int x, T val) {
    int r = this->find(x);
    T inv_val = Abelian::invert(std::move(leaf_data[x]));
    leaf_data[x] = std::move(val);
    this->agg_data[r] = Abelian::op(
        Abelian::op(std::move(this->agg_data[r]), std::move(inv_val)),
        leaf_data[x]);
  }
};
