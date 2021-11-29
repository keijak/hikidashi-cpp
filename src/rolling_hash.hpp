#include <bits/stdc++.h>

struct RollingHash {
  using u64 = std::uint64_t;
  using u128 = __uint128_t;
  static const u64 kMod = (1ULL << 61) - 1;

  static u64 base() {
    static const auto kBase = []() -> u64 {
      std::random_device seed_gen;
      std::mt19937_64 engine(seed_gen());
      std::uniform_int_distribution<u64> rand(1, kMod - 1);
      return rand(engine);
    }();
    return kBase;
  }

  static inline u64 add(u64 a, u64 b) {
    a += b;
    return (a >= kMod) ? (a - kMod) : a;
  }

  static inline u64 sub(u64 a, u64 b) { return add(a, kMod - b); }

  static inline u64 mul(u64 a, u64 b) {
    u128 t = u128(a) * b;
    u64 na = u64(t >> 61);
    u64 nb = u64(t & kMod);
    na += nb;
    return (na >= kMod) ? (na - kMod) : na;
  }

  static u64 pow_base(int i) {
    static std::vector<u64> kPowBase(1, 1);
    while (int(kPowBase.size()) <= i) {
      u64 val = mul(kPowBase.back(), base());
      kPowBase.push_back(val);
    }
    return kPowBase[i];
  }

  // Calculates hash(s || t) from hash(s), hash(t) and len(t).
  static u64 concat(u64 a, u64 b, int b_length) {
    return add(mul(a, pow_base(b_length)), b);
  }
};

// Computes hash for any substring in O(1).
struct SpanHash : public RollingHash {
  std::vector<u64> cum_hash;

  // Constructionn: O(n).
  // All elements must be non-zero. Otherwise we won't be able to distinguish
  // between [1] and [0, 1].
  template <class Sequence>
  explicit SpanHash(const Sequence &s) : cum_hash(s.size() + 1) {
    int i = 0;
    for (const auto &x : s) {
      u64 val = static_cast<u64>(x);
      assert(val != 0);  // Ensure all elements are non-zero!
      cum_hash[i + 1] = add(mul(cum_hash[i], base()), val);
      ++i;
    }
  }

  // Returns the hash value of the substring s[l:r]. O(1).
  u64 get(int l, int r) const {
    // Compute `hash(s[0:r]) - hash(s[0:l]) * B^(r-l) (mod M)`
    return sub(cum_hash[r], mul(cum_hash[l], pow_base(r - l)));
  }
};

// Monoid for segment tree.
struct RollingHashOp {
  using u64 = std::uint64_t;
  struct T {
    u64 hash;   // Initialize this field with a non-zero element.
    int width;  // Initialize this field with 1.
  };
  static T op(const T &x, const T &y) {
    u64 hash = RollingHash::mul(x.hash, RollingHash::pow_base(y.width));
    hash = RollingHash::add(hash, y.hash);
    return T{hash, x.width + y.width};
  }
  static constexpr T id() { return {0, 0}; }
};

// Monoid for segment tree with lazy propagation.
// https://www.codechef.com/problems/SUBINVER
struct XorRollingHashOp {
  using u64 = std::uint64_t;
  struct T {
    u64 hash;   // Initialize this field with a non-zero element.
    int width;  // Initialize this field with 1.
  };
  using F = bool;

  static T op(const T &x, const T &y) {
    u64 hash = RollingHash::mul(x.hash, RollingHash::pow_base(y.width));
    hash = RollingHash::add(hash, y.hash);
    return T{hash, x.width + y.width};
  }
  static constexpr T id() { return {0, 0}; }

  static T f_apply(const F &f, const T &x) {
    if (f == 0) return x;
    return {RollingHash::sub(
                RollingHash::sub(RollingHash::pow_base(x.width), 1), x.hash),
            x.width};
  }
  static F f_compose(const F &f, const F &g) { return f ^ g; }
  static constexpr F f_id() { return 0; }
};

struct Range2dHasher {
  using u64 = unsigned long long;
  using u128 = __uint128_t;
  static const u64 kMod = (1ULL << 61) - 1;
  std::vector<u64> pow_base1, pow_base2;
  std::vector<std::vector<u64>> cum_hash;

  explicit Range2dHasher(const std::vector<std::string> &g) {
    const int n = g.size();
    const int m = g[0].size();
    cum_hash.assign(n + 1, std::vector<u64>(m + 1, 0));
    pow_base1.assign(n + 1, 0);
    pow_base2.assign(m + 1, 0);
    pow_base1[0] = pow_base2[0] = 1;
    const auto &[b1, b2] = bases();
    for (int i = 0; i < n; ++i) {
      pow_base1[i + 1] = mul(pow_base1[i], b1);
    }
    for (int i = 0; i < m; ++i) {
      pow_base2[i + 1] = mul(pow_base2[i], b2);
    }
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) {
        u64 h = u64(g[i][j]);
        assert(h != 0);  // Ensure all elements are non-zero!
        h = add(h, mul(cum_hash[i][j + 1], b1));
        h = add(h, mul(cum_hash[i + 1][j], b2));
        h = sub(h, mul(mul(cum_hash[i][j], b1), b2));
        cum_hash[i + 1][j + 1] = h;
      }
    }
  }

  // Returns the hash value for the [(r0,c0), (r1,c1)) rectangle.
  // (r1,c1) is exclusive.
  u64 get(int r0, int c0, int r1, int c1) const {
    u64 h = cum_hash[r1][c1];
    h = sub(h, mul(cum_hash[r0][c1], pow_base1[r1 - r0]));
    h = sub(h, mul(cum_hash[r1][c0], pow_base2[c1 - c0]));
    u64 tmp = mul(cum_hash[r0][c0], pow_base1[r1 - r0]);
    h = add(h, mul(tmp, pow_base2[c1 - c0]));
    return h;
  }

 private:
  static std::pair<u64, u64> bases() {
    static const auto kBase = []() -> std::pair<u64, u64> {
      std::random_device seed_gen;
      std::mt19937_64 engine(seed_gen());
      std::uniform_int_distribution<u64> rand(1, kMod - 1);
      return {rand(engine), rand(engine)};
    }();
    return kBase;
  }

  static inline u64 add(u64 a, u64 b) {
    a += b;
    return (a >= kMod) ? (a - kMod) : a;
  }

  static inline u64 sub(u64 a, u64 b) { return add(a, kMod - b); }

  static inline u64 mul(u64 a, u64 b) {
    u128 t = u128(a) * b;
    u64 na = u64(t >> 61);
    u64 nb = u64(t & kMod);
    na += nb;
    return (na >= kMod) ? (na - kMod) : na;
  }
};
