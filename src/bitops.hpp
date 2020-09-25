// GCC Builtins.

int popcount(unsigned int x) { return __builtin_popcount(x); }
int popcount(unsigned long x) { return __builtin_popcountl(x); }
int popcount(unsigned long long x) { return __builtin_popcountll(x); }

int count_leading_zero(unsigned int x) { return __builtin_clz(x); }
int count_leading_zero(unsigned long x) { return __builtin_clzl(x); }
int count_leading_zero(unsigned long long x) { return __builtin_clzll(x); }

int count_trailing_zero(unsigned int x) { return __builtin_ctz(x); }
int count_trailing_zero(unsigned long x) { return __builtin_ctzl(x); }
int count_trailing_zero(unsigned long long x) { return __builtin_ctzll(x); }

// Bit extractors.

// Turn off all bits of x except the rightmost (least significant) bit.
int rightmost_bit(unsigned int x) { return x & -x; }

// Turn off all bits of x except the leftmost (most significant) bit.
int leftmost_bit(unsigned int x) {
  int l = 31 - __builtin_clz(x);
  return 1 << l;
}
