#include <bits/stdc++.h>

struct RollingHash {
  using u64 = unsigned long long;
  using u128 = __uint128_t;
  static const u64 mod = (1ULL << 61) - 1;
  std::vector<u64> hash_val, base_power;

  explicit RollingHash(std::string_view s)
      : hash_val(s.size() + 1), base_power(s.size() + 1) {
    const int n = s.size();
    base_power[0] = 1;
    for (int i = 0; i < n; i++) {
      base_power[i + 1] = mul(base_power[i], base());
      hash_val[i + 1] = add(mul(hash_val[i], base()), u64(s[i]));
    }
  }

  // Returns the hash value for the [l,r) interval.
  u64 get(int l, int r) {
    return add(hash_val[r], mod - mul(hash_val[l], base_power[r - l]));
  }

 private:
  static u64 base() {
    static u64 val = []() -> u64 {
      std::random_device seed_gen;
      std::mt19937_64 engine(seed_gen());
      std::uniform_int_distribution<u64> rand(1, mod - 1);
      return rand(engine);
    }();
    return val;
  }

  static inline u64 add(u64 a, u64 b) {
    a += b;
    return (a >= mod) ? (a - mod) : a;
  }

  static inline u64 mul(u64 a, u64 b) {
    u128 t = (u128)a * b;
    u64 na = t >> 61;
    u64 nb = t & mod;
    na += nb;
    return (na >= mod) ? (na - mod) : na;
  }
};

struct RollingHash2d {
  using u64 = unsigned long long;
  using u128 = __uint128_t;
  static const u64 mod = (1ULL << 61) - 1;
  std::vector<u64> base1_pow, base2_pow;
  std::vector<std::vector<u64>> hash_val;

  explicit RollingHash2d(const std::vector<std::string> &g) {
    const int n = g.size();
    const int m = g[0].size();
    hash_val.assign(n + 1, std::vector<u64>(m + 1, 0));
    base1_pow.assign(n + 1, 0);
    base2_pow.assign(m + 1, 0);
    auto [b1, b2] = base();
    base1_pow[0] = base2_pow[0] = 1;
    for (int i = 0; i < n; ++i) {
      base1_pow[i + 1] = mul(base1_pow[i], b1);
    }
    for (int i = 0; i < m; ++i) {
      base2_pow[i + 1] = mul(base2_pow[i], b2);
    }
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) {
        u64 h = u64(g[i][j]);
        h = add(h, mul(hash_val[i][j + 1], b1));
        h = add(h, mul(hash_val[i + 1][j], b2));
        h = sub(h, mul(mul(hash_val[i][j], b1), b2));
        hash_val[i + 1][j + 1] = h;
      }
    }
  }

  // Returns the hash value for the [(r0,c0), (r1,c1)) rectangle.
  // (r1,c1) is exclusive.
  u64 get(int r0, int c0, int r1, int c1) const {
    u64 h = hash_val[r1][c1];
    h = sub(h, mul(hash_val[r0][c1], base1_pow[r1 - r0]));
    h = sub(h, mul(hash_val[r1][c0], base2_pow[c1 - c0]));
    u64 tmp = mul(hash_val[r0][c0], base1_pow[r1 - r0]);
    h = add(h, mul(tmp, base2_pow[c1 - c0]));
    return h;
  }

 private:
  static std::pair<u64, u64> base() {
    static auto val = []() -> std::pair<u64, u64> {
      std::random_device seed_gen;
      std::mt19937_64 engine(seed_gen());
      std::uniform_int_distribution<u64> rand(1, mod - 1);
      return {rand(engine), rand(engine)};
    }();
    return val;
  }

  static inline u64 add(u64 a, u64 b) {
    a += b;
    return (a >= mod) ? (a - mod) : a;
  }

  static inline u64 sub(u64 a, u64 b) { return add(a, mod - b); }

  static inline u64 mul(u64 a, u64 b) {
    u128 t = (u128)a * b;
    u64 na = t >> 61;
    u64 nb = t & mod;
    na += nb;
    return (na >= mod) ? (na - mod) : na;
  }
};
