struct UnionFind {
  vector<int> data;  // positive: parent, negative: size
  int num_roots;

  explicit UnionFind(size_t sz) : data(sz, -1), num_roots(sz) {}

  bool unite(int x, int y) {
    x = find(x), y = find(y);
    if (x == y) return false;
    if (data[x] > data[y]) swap(x, y);
    data[x] += data[y];
    data[y] = x;
    --num_roots;
    return true;
  }

  int find(int k) {
    if (data[k] < 0) return k;
    return data[k] = find(data[k]);
  }

  int size(int k) { return -data[find(k)]; }

  bool same(int x, int y) { return find(x) == find(y); }

  vector<int> roots() {
    vector<int> res;
    res.reserve(num_roots);
    for (int i = 0; i < (int)data.size(); ++i) {
      if (data[i] < 0) res.push_back(i);
    }
    return res;
  }
};
