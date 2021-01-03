// GCC Builtins.

using u64 = unsigned long long;

inline int popcount(unsigned x) { return __builtin_popcount(x); }
inline int popcount(u64 x) { return __builtin_popcountll(x); }

// Most Significant Set Bit (Highest One Bit)
int mssb_pos(unsigned x) {
  static const int CLZ_WIDTH = __builtin_clz(1);
  assert(x != 0);
  return CLZ_WIDTH - __builtin_clz(x);
}
inline unsigned mssb(unsigned int x) { return 1U << mssb_pos(x); }

int mssb_pos(u64 x) {
  static const int CLZLL_WIDTH = __builtin_clzll(1LL);
  assert(x != 0);
  return CLZLL_WIDTH - __builtin_clzll(x);
}
inline u64 mssb(u64 x) { return 1ULL << mssb_pos(x); }

// Least Significant Set Bit (Lowest One Bit)
int lssb_pos(unsigned x) {
  assert(x != 0);
  return __builtin_ctz(x);
}
inline unsigned lssb(unsigned int x) { return 1U << lssb_pos(x); }

int lssb_pos(u64 x) {
  assert(x != 0);
  return __builtin_ctzll(x);
}
inline u64 lssb(u64 x) { return 1ULL << lssb_pos(x); }

void enumerate_subsets(unsigned bits, std::function<void(unsigned)> func) {
  if (bits == 0) return;
  unsigned mssb_mask = mssb(bits);
  for (unsigned sub = bits;; sub = (sub - 1) & bits) {
    func(sub);
    if (sub != 0) break;
  }
}

// Enumerates subsets that contain the most significant bit of the bits.
void enumerate_subsets_half(unsigned bits, std::function<void(unsigned)> func) {
  if (bits == 0) return;
  unsigned fixed_bit = mssb(bits);
  for (unsigned sub = bits;; sub = (sub - 1) & bits) {
    func(sub);
    if (not(sub & fixed_bit)) break;
  }
}
