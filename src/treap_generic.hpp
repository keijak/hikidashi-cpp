// Treap with lazy propagation.
//
// - split: O(log N)
// - merge (concatenation): O(log N)
// - random access: O(log N)
// - insert at pos: O(log N)
// - erase at pos: O(log N)
// - fold(l, r): O(log N)
// - apply(l, r, val): O(log N)
// - reverse(l, r): O(log N)
//
// base:
// https://github.com/maze1230/Library/blob/c20e57b9cc290664407146438c4f62712a7fcb81/Tree/Treap.cpp

template <class Monoids, size_t NODE_NUM = 1000000>
class GenericTreap {
 private:
  using T = typename Monoids::T;
  using F = typename Monoids::F;

  struct Node {
   public:
    T v, acc;
    F lazy;
    bool rev;
    size_t pri;
    int size;
    Node *lch = nullptr, *rch = nullptr;

    Node()
        : v(Monoids::id()),
          acc(v),
          lazy(Monoids::f_id()),
          rev(false),
          pri(0),
          size(1) {}
    Node(T v, size_t pri)
        : v(v), acc(v), lazy(Monoids::f_id()), rev(false), pri(pri), size(1) {}
  };
  using NodePtr = struct Node *;
  NodePtr root;

  explicit GenericTreap(NodePtr p) : root(p) {}

  static NodePtr create_node(const T v, size_t pri) {
    static std::unique_ptr<Node[]> pool(new Node[NODE_NUM]);
    static size_t i = 0;
    return &(pool[i++] = Node(v, pri));
  }

  static size_t random_priority() {
    static std::mt19937_64 engine(std::random_device{}());
    static std::uniform_int_distribution<size_t> dist(0);
    return dist(engine);
  }

  static inline int count_(NodePtr p) { return p == nullptr ? 0 : p->size; }

  static inline T fold_(NodePtr p) {
    return p == nullptr ? Monoids::id() : p->acc;
  }

  static inline void apply_(NodePtr p, const F &val) {
    if (p != nullptr) {
      p->v = Monoids::f_apply(val, p->v);
      p->acc = Monoids::f_apply(val, p->acc);
      p->lazy = Monoids::f_compose(val, p->lazy);
    }
  }

  static inline void reverse_(NodePtr p) {
    if (p != nullptr) {
      Monoids::reverse(p->v);
      Monoids::reverse(p->acc);
      p->rev ^= true;
    }
  }

  static NodePtr push_down_(NodePtr p) {
    if (p->lazy != Monoids::f_id()) {
      if (p->lch != nullptr) apply_(p->lch, p->lazy);
      if (p->rch != nullptr) apply_(p->rch, p->lazy);
      p->lazy = Monoids::f_id();
    }
    if (p->rev) {
      std::swap(p->lch, p->rch);
      if (p->lch != nullptr) reverse_(p->lch);
      if (p->rch != nullptr) reverse_(p->rch);
      p->rev = false;
    }
    return p;
  }

  static NodePtr pull_up_(NodePtr p) {
    assert(not p->rev);
    p->size = count_(p->lch) + count_(p->rch) + 1;
    p->acc = p->lch ? Monoids::op(p->lch->acc, p->v) : p->v;
    if (p->rch) p->acc = Monoids::op(p->acc, p->rch->acc);
    return p;
  }

  static NodePtr merge_(NodePtr l, NodePtr r) {
    if (l == nullptr || r == nullptr) {
      return l == nullptr ? r : l;
    }
    if (l->pri > r->pri) {
      l = push_down_(l);
      l->rch = merge_(l->rch, r);
      return pull_up_(l);
    } else {
      r = push_down_(r);
      r->lch = merge_(l, r->lch);
      return pull_up_(r);
    }
  }

  static std::pair<NodePtr, NodePtr> split_(NodePtr p, int k) {
    if (p == nullptr) return {nullptr, nullptr};

    p = push_down_(p);
    if (k <= count_(p->lch)) {
      auto ret = split_(p->lch, k);
      p->lch = ret.second;
      return {ret.first, pull_up_(p)};
    } else {
      auto ret = split_(p->rch, k - count_(p->lch) - 1);
      p->rch = ret.first;
      return {pull_up_(p), ret.second};
    }
  }

  void insert_(int k, T v, size_t pri) {
    if (root == nullptr) {
      root = create_node(std::move(v), pri);
      return;
    }
    NodePtr l, r;
    std::tie(l, r) = split_(root, k);
    root = merge_(l, create_node(std::move(v), pri));
    root = merge_(root, r);
  }

  void to_vec_(NodePtr p, typename std::vector<T>::iterator itr) {
    if (p == nullptr) return;
    to_vec_(p->lch, itr);
    *(itr + count_(p->lch)) = p->v;
    to_vec_(p->rch, itr + count_(p->lch) + 1);
  }

 public:
  GenericTreap() : root(nullptr) {}

  int size() { return count_(root); }

  static GenericTreap merge(GenericTreap l, GenericTreap r) {
    return GenericTreap(merge_(l.root, r.root));
  }

  std::pair<GenericTreap, GenericTreap> split(int k) {
    auto ret = split_(root, k);
    return {GenericTreap(ret.first), GenericTreap(ret.second)};
  }

  void insert(int k, T v) { insert_(k, std::move(v), random_priority()); }

  T erase(int k) {
    GenericTreap l, m, r;
    std::tie(l, r) = split(k + 1);
    std::tie(l, m) = l.split(k);
    root = merge_(l.root, r.root);
    return m.root->v;
  }

  T operator[](int k) {
    GenericTreap l, m, r;
    std::tie(l, r) = split(k + 1);
    std::tie(l, m) = l.split(k);
    T ret = m.root->v;
    root = merge_(merge_(l.root, m.root), r.root);
    return ret;
  }

  void set(int k, T v) {
    GenericTreap l, m, r;
    std::tie(l, r) = split(k + 1);
    std::tie(l, m) = l.split(k);
    m.root->v = std::move(v);
    m.root = pull_up_(m.root);
    root = merge_(merge_(l.root, m.root), r.root);
  }

  T fold(int first, int last) {
    GenericTreap l, m, r;
    T ret;
    std::tie(l, r) = split(last);
    std::tie(l, m) = l.split(first);
    ret = fold_(m.root);
    root = merge_(merge_(l.root, m.root), r.root);
    return ret;
  }

  T fold_all() const { return fold_(root); }

  void apply(int first, int last, const F &val) {
    GenericTreap l, m, r;
    std::tie(l, r) = split(last);
    std::tie(l, m) = l.split(first);
    apply_(m.root, val);
    root = merge_(merge_(l.root, m.root), r.root);
  }

  void reverse(int first, int last) {
    GenericTreap l, m, r;
    std::tie(l, r) = split(last);
    std::tie(l, m) = l.split(first);
    reverse_(m.root);
    root = merge_(merge_(l.root, m.root), r.root);
  }

  std::vector<T> to_vec() {
    std::vector<T> v(size());
    to_vec_(root, v.begin());
    return v;
  }
};

// Example Monoids:
// https://codeforces.com/gym/102787/problem/C
struct XorSumOp {
  struct T {
    int ones;
    int width;  // NOTE: Must be initialized with width=1!
    int max_ones;
    int max_zeros;
    int l_ones;
    int l_zeros;
    int r_ones;
    int r_zeros;
  };
  using F = std::pair<bool, int>;

  // Fold: Sum
  static T op(const T &x, const T &y) {
    int l_ones = x.l_ones;
    if (x.l_ones == x.width) l_ones += y.l_ones;
    int l_zeros = x.l_zeros;
    if (x.l_zeros == x.width) l_zeros += y.l_zeros;

    int r_ones = y.r_ones;
    if (y.r_ones == y.width) r_ones += x.r_ones;
    int r_zeros = y.r_zeros;
    if (y.r_zeros == y.width) r_zeros += x.r_zeros;

    return {
        x.ones + y.ones,
        x.width + y.width,
        std::max({x.max_ones, y.max_ones, x.r_ones + y.l_ones}),
        std::max({x.max_zeros, y.max_zeros, x.r_zeros + y.l_zeros}),
        l_ones,
        l_zeros,
        r_ones,
        r_zeros,
    };
  }
  static T id() { return {0, 0, 0, 0, 0, 0, 0, 0}; }

  // Update: 0-1 flip by f=1.
  static T f_apply(const F &f, const T &x) {
    if (f.first) {
      if (f.second == 0) {
        // all zero
        return {0, x.width, 0, x.width, 0, x.width, 0, x.width};
      } else {
        // all one
        return {x.width, x.width, x.width, 0, x.width, 0, x.width, 0};
      }
    } else {
      if (f.second == 0) return x;
      return {x.width - x.ones, x.width,  x.max_zeros, x.max_ones,
              x.l_zeros,        x.l_ones, x.r_zeros,   x.r_ones};
    }
  }
  static F f_compose(const F &f, const F &g) {
    if (f.first) {
      return f;  // overwrite
    }
    return {g.first, 1 ^ g.second};
  }
  static constexpr F f_id() { return {false, 0}; }

  static void reverse(T &x) {
    std::swap(x.l_ones, x.r_ones);
    std::swap(x.l_zeros, x.r_zeros);
  }
};
