// GCC Builtins.

#include <cassert>
#include <functional>
#include <limits>
using u64 = unsigned long long;

// Compatible with C++20 <bit>.
int countl_zero(unsigned x) {
  if (x == 0) return std::numeric_limits<unsigned>::digits;
  return __builtin_clz(x);
}
int countr_zero(unsigned x) {
  if (x == 0) return std::numeric_limits<unsigned>::digits;
  return __builtin_ctz(x);
}
int countl_one(unsigned x) {
  x = ~x;
  if (x == 0) return std::numeric_limits<unsigned>::digits;
  return __builtin_clz(x);
}
int countr_one(unsigned x) {
  x = ~x;
  if (x == 0) return std::numeric_limits<unsigned>::digits;
  return __builtin_ctz(x);
}
inline int popcount(unsigned x) { return __builtin_popcount(x); }
inline bool has_single_bit(unsigned x) { return __builtin_popcount(x) == 1; }

int countl_zero(u64 x) {
  if (x == 0) return std::numeric_limits<u64>::digits;
  return __builtin_clzll(x);
}
int countr_zero(u64 x) {
  if (x == 0) return std::numeric_limits<u64>::digits;
  return __builtin_ctzll(x);
}
int countl_one(u64 x) {
  x = ~x;
  if (x == 0) return std::numeric_limits<u64>::digits;
  return __builtin_clzll(x);
}
int countr_one(u64 x) {
  x = ~x;
  if (x == 0) return std::numeric_limits<u64>::digits;
  return __builtin_ctzll(x);
}
inline int popcount(u64 x) { return __builtin_popcountll(x); }
inline bool has_single_bit(u64 x) { return __builtin_popcountll(x) == 1; }

int bit_width(unsigned x) {
  if (x == 0) return 0;
  return std::numeric_limits<unsigned>::digits - __builtin_clz(x);
}
int bit_width(u64 x) {
  if (x == 0) return 0;
  return std::numeric_limits<u64>::digits - __builtin_clzll(x);
}

// Most Significant Set Bit (Highest One Bit)
// - mssb(x) = bit_floor(x)
// - mssb_pos(x) = bit_width(x) - 1
template <typename U>
U bit_floor(U x) {
  if (x == 0) return 0;
  return U(1) << (bit_width(x) - 1);
}
template <typename U>
U bit_ceil(U x) {
  auto b = bit_floor(x);
  return (b == x) ? b : (b << 1);
}

// Least Significant Set Bit (Lowest One Bit)
// - lssb(x) = (x & -x)
// - lssb_pos(x) = countr_zero(x)
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
