// GCC Builtins.

#include <cassert>
#include <functional>
using u64 = unsigned long long;

// Compatible with C++20 <bit>.
const int UINT_WIDTH = __builtin_clz(1U) + 1;
unsigned bit_width(unsigned x) {
  return (x == 0) ? 0 : UINT_WIDTH - __builtin_clz(x);
}
int countl_zero(unsigned x) { return (x == 0) ? UINT_WIDTH : __builtin_clz(x); }
int countr_zero(unsigned x) { return (x == 0) ? UINT_WIDTH : __builtin_ctz(x); }
int countl_one(unsigned x) {
  x = ~x;
  return (x == 0) ? UINT_WIDTH : __builtin_clz(x);
}
int countr_one(unsigned x) {
  x = ~x;
  return (x == 0) ? UINT_WIDTH : __builtin_ctz(x);
}
inline int popcount(unsigned x) { return __builtin_popcount(x); }
inline bool has_single_bit(unsigned x) { return __builtin_popcount(x) == 1; }

const int ULL_WIDTH = __builtin_clzll(1ULL) + 1;
int bit_width(u64 x) { return (x == 0) ? 0 : ULL_WIDTH - __builtin_clzll(x); }
int countl_zero(u64 x) { return (x == 0) ? ULL_WIDTH : __builtin_clzll(x); }
int countr_zero(u64 x) { return (x == 0) ? ULL_WIDTH : __builtin_ctzll(x); }
int countl_one(u64 x) {
  x = ~x;
  return (x == 0) ? ULL_WIDTH : __builtin_clzll(x);
}
int countr_one(u64 x) {
  x = ~x;
  return (x == 0) ? ULL_WIDTH : __builtin_ctzll(x);
}
inline int popcount(u64 x) { return __builtin_popcountll(x); }
inline bool has_single_bit(u64 x) { return __builtin_popcountll(x) == 1; }

// Most Significant Set Bit (Highest One Bit) = std::bit_floor(x)
int mssb_pos(unsigned x) {
  static const int CLZ_WIDTH = __builtin_clz(1);
  assert(x != 0);
  return CLZ_WIDTH - __builtin_clz(x);
}
int mssb_pos(u64 x) {
  static const int CLZLL_WIDTH = __builtin_clzll(1LL);
  assert(x != 0);
  return CLZLL_WIDTH - __builtin_clzll(x);
}
template <typename U>
inline U bit_floor(U x) {
  return U(1) << mssb_pos(x);
}
template <typename U>
U bit_ceil(U x) {
  auto ret = bit_floor(x);
  return (ret == x) ? ret : (ret << 1);
}

// Least Significant Set Bit (Lowest One Bit)
template <typename T>
inline T lssb(T x) {
  return (x & -x);
}

void enumerate_subsets(unsigned bits, std::function<void(unsigned)> func) {
  if (bits == 0) return;
  unsigned mssb_mask = bit_floor(bits);
  for (unsigned sub = bits;; sub = (sub - 1) & bits) {
    func(sub);
    if (sub != 0) break;
  }
}

// Enumerates subsets that contain the most significant bit of the bits.
void enumerate_subsets_half(unsigned bits, std::function<void(unsigned)> func) {
  if (bits == 0) return;
  unsigned fixed_bit = bit_floor(bits);
  for (unsigned sub = bits;; sub = (sub - 1) & bits) {
    func(sub);
    if (not(sub & fixed_bit)) break;
  }
}
