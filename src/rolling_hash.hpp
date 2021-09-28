#include <bits/stdc++.h>

struct RollingHash {
  using u64 = unsigned long long;
  using u128 = __uint128_t;
  static const u64 mod = (1ULL << 61) - 1;
  std::vector<u64> cum_hash, pow_base;

  explicit RollingHash(std::string_view s)
      : cum_hash(s.size() + 1), pow_base(s.size() + 1) {
    const int n = s.size();
    pow_base[0] = 1;
    for (int i = 0; i < n; i++) {
      pow_base[i + 1] = mul(pow_base[i], base());
      cum_hash[i + 1] = add(mul(cum_hash[i], base()), u64(s[i]));
    }
  }

  // Returns the hash value of the substring s[l:r].
  u64 get(int l, int r) {
    // Compute `hash(s[0:r]) - hash(s[0:l]) * B^(r-l) (mod M)`
    return add(cum_hash[r], mod - mul(cum_hash[l], pow_base[r - l]));
  }

 private:
  static u64 base() {
    static const u64 kBase = []() -> u64 {
      std::random_device seed_gen;
      std::mt19937_64 engine(seed_gen());
      std::uniform_int_distribution<u64> rand(1, mod - 1);
      return rand(engine);
    }();
    return kBase;
  }

  static inline u64 add(u64 a, u64 b) {
    a += b;
    return (a >= mod) ? (a - mod) : a;
  }

  static inline u64 mul(u64 a, u64 b) {
    u128 t = u128(a) * b;
    u64 na = u64(t >> 61);
    u64 nb = u64(t & mod);
    na += nb;
    return (na >= mod) ? (na - mod) : na;
  }
};

struct RollingHash2d {
  using u64 = unsigned long long;
  using u128 = __uint128_t;
  static const u64 mod = (1ULL << 61) - 1;
  std::vector<u64> pow_base1, pow_base2;
  std::vector<std::vector<u64>> cum_hash;

  explicit RollingHash2d(const std::vector<std::string> &g) {
    const int n = g.size();
    const int m = g[0].size();
    cum_hash.assign(n + 1, std::vector<u64>(m + 1, 0));
    pow_base1.assign(n + 1, 0);
    pow_base2.assign(m + 1, 0);
    auto [b1, b2] = base();
    pow_base1[0] = pow_base2[0] = 1;
    for (int i = 0; i < n; ++i) {
      pow_base1[i + 1] = mul(pow_base1[i], b1);
    }
    for (int i = 0; i < m; ++i) {
      pow_base2[i + 1] = mul(pow_base2[i], b2);
    }
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) {
        u64 h = u64(g[i][j]);
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
  static std::pair<u64, u64> base() {
    static const auto kBase = []() -> std::pair<u64, u64> {
      std::random_device seed_gen;
      std::mt19937_64 engine(seed_gen());
      std::uniform_int_distribution<u64> rand(1, mod - 1);
      return {rand(engine), rand(engine)};
    }();
    return kBase;
  }

  static inline u64 add(u64 a, u64 b) {
    a += b;
    return (a >= mod) ? (a - mod) : a;
  }

  static inline u64 sub(u64 a, u64 b) { return add(a, mod - b); }

  static inline u64 mul(u64 a, u64 b) {
    u128 t = u128(a) * b;
    u64 na = u64(t >> 61);
    u64 nb = u64(t & mod);
    na += nb;
    return (na >= mod) ? (na - mod) : na;
  }
};
