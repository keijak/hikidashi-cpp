struct RollingHash {
  using u64 = unsigned long long;
  using u128 = __uint128_t;
  static const u64 mod = (1ULL << 61) - 1;
  vector<u64> hashe_val, base_power;

  RollingHash(string_view s)
      : hashe_val(s.size() + 1), base_power(s.size() + 1) {
    const int n = s.size();
    base_power[0] = 1;
    for (int i = 0; i < n; i++) {
      base_power[i + 1] = mul(base_power[i], base());
      hashe_val[i + 1] = add(mul(hashe_val[i], base()), s[i]);
    }
  }

  // Returns the hash value for the [l,r) interval.
  u64 get(int l, int r) {
    return add(hashe_val[r], mod - mul(hashe_val[l], base_power[r - l]));
  }

 private:
  static u64 base() {
    static u64 val = []() -> u64 {
      random_device seed_gen;
      mt19937_64 engine(seed_gen());
      uniform_int_distribution<u64> rand(1, mod - 1);
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
