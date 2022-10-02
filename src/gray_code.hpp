// https://en.wikipedia.org/wiki/Gray_code

unsigned to_gray_code(unsigned n) { return n ^ (n >> 1); }

unsigned from_gray_code(unsigned num) {
  auto mask = num;
  while (mask) {
    mask >>= 1;
    num ^= mask;
  }
  return num;
}

// A more efficient version for Gray codes 32 bits or fewer through the use of
// SWAR (SIMD within a register) techniques. It implements a parallel prefix
// XOR function. The assignment statements can be in any order.

uint32_t from_gray_code32(uint32_t num) {
  num ^= num >> 16;
  num ^= num >> 8;
  num ^= num >> 4;
  num ^= num >> 2;
  num ^= num >> 1;
  return num;
}

uint64_t from_gray_code64(uint64_t num) {
  num ^= num >> 32;
  num ^= num >> 16;
  num ^= num >> 8;
  num ^= num >> 4;
  num ^= num >> 2;
  num ^= num >> 1;
  return num;
}
