// Bit Operations
#include <cassert>
#include <functional>
#include <limits>
#include <type_traits>
using u64 = unsigned long long;

namespace bitops {

// Bit width of type T.
// Unlike `numeric_limits<T>::digits` it includes the sign bit, so:
// num_bits<int32_t> == 32 (not 31).
template <typename T>
constexpr int num_bits = CHAR_BIT * sizeof(T);

// Compatible with C++20 <bit>.
inline int popcount(unsigned x) { return __builtin_popcount(x); }
inline bool has_single_bit(unsigned x) { return __builtin_popcount(x) == 1; }
inline int countl_zero(unsigned x) {
  if (x == 0) return std::numeric_limits<unsigned>::digits;
  return __builtin_clz(x);
}
inline int countr_zero(unsigned x) {
  if (x == 0) return std::numeric_limits<unsigned>::digits;
  return __builtin_ctz(x);
}
inline int countl_one(unsigned x) {
  x = ~x;
  if (x == 0) return std::numeric_limits<unsigned>::digits;
  return __builtin_clz(x);
}
inline int countr_one(unsigned x) {
  x = ~x;
  if (x == 0) return std::numeric_limits<unsigned>::digits;
  return __builtin_ctz(x);
}

inline int popcount(u64 x) { return __builtin_popcountll(x); }
inline bool has_single_bit(u64 x) { return __builtin_popcountll(x) == 1; }
inline int countl_zero(u64 x) {
  if (x == 0) return std::numeric_limits<u64>::digits;
  return __builtin_clzll(x);
}
inline int countr_zero(u64 x) {
  if (x == 0) return std::numeric_limits<u64>::digits;
  return __builtin_ctzll(x);
}
inline int countl_one(u64 x) {
  x = ~x;
  if (x == 0) return std::numeric_limits<u64>::digits;
  return __builtin_clzll(x);
}
inline int countr_one(u64 x) {
  x = ~x;
  if (x == 0) return std::numeric_limits<u64>::digits;
  return __builtin_ctzll(x);
}

inline int bit_width(unsigned x) {
  if (x == 0) return 0;
  return std::numeric_limits<unsigned>::digits - __builtin_clz(x);
}
inline int bit_width(u64 x) {
  if (x == 0) return 0;
  return std::numeric_limits<u64>::digits - __builtin_clzll(x);
}
template <typename T, typename U = std::make_unsigned_t<T>>
inline U bit_floor(T x) {
  if (x <= 2) return x;
  return U(1) << (bit_width(U(x)) - 1);
}
template <typename T, typename U = std::make_unsigned_t<T>>
inline U bit_ceil(T x) {
  if (x <= 2) return x;
  return bit_floor(U(x - 1)) << 1;
}

// Most significant bit that is set.
// - msb(x) = bit_floor(x)
// - msb_log(x) = bit_width(x) - 1
inline int msb_log(unsigned x) {
  assert(x != 0);
  return std::numeric_limits<unsigned>::digits - __builtin_clz(x) - 1;
}
inline int msb_log(u64 x) {
  assert(x != 0);
  return std::numeric_limits<u64>::digits - __builtin_clzll(x) - 1;
}
template <typename T, typename U = std::make_unsigned_t<T>>
inline U msb(T x) {
  if (x == 0) return 0;
  return U(1) << msb_log(U(x));
}
template <typename T, typename U = std::make_unsigned_t<T>>
inline U drop_msb(T x) {
  return U(x) ^ msb(U(x));
}

// Least significant bit that is set.
// - lsb(x) = (x & -x)
// - lsb_log(x) = countr_zero(x)
inline int lsb_log(unsigned x) {
  assert(x != 0);
  return __builtin_ctz(x);
}
inline int lsb_log(u64 x) {
  assert(x != 0);
  return __builtin_ctzll(x);
}
template <typename T, typename U = std::make_unsigned_t<T>>
inline U lsb(T x) {
  return (x & -x);
}
template <typename T, typename U = std::make_unsigned_t<T>>
inline U drop_lsb(T x) {
  return x & (x - 1);
}

void enumerate_subsets(unsigned bits, std::function<void(unsigned)> func) {
  if (bits == 0) return;
  for (unsigned sub = bits;; sub = (sub - 1) & bits) {
    func(sub);
    if (sub == 0) break;
  }
}

// Enumerates subsets that contain the most significant set bit of the bits.
void enumerate_half_subsets(unsigned bits, std::function<void(unsigned)> func) {
  if (bits == 0) return;
  const unsigned fixed_bit = msb(bits);
  for (unsigned sub = bits; sub & fixed_bit; sub = (sub - 1) & bits) {
    func(sub);
  }
}

}  // namespace bitops
