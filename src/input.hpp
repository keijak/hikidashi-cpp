// Usage:
//   int n = input;
//   unsigned x = (unsigned)(input) | 1;

#include <cctype>
#include <cstdio>
#include <type_traits>

struct Input {
  operator int() { return read_int<int>(); }
  operator long long() { return read_int<long long>(); }
  operator unsigned() { return read_int<unsigned>(); }

 private:
  template <typename T>
  T read_int() {
    T ret = 0, sgn = 1;
    int ch = getchar_unlocked();
    while (isspace(ch)) {
      ch = getchar_unlocked();
    }
    if constexpr (!std::is_unsigned<T>::value) {
      if (ch == '-') {
        sgn = -1;
        ch = getchar_unlocked();
      }
    }
    for (; isdigit(ch); ch = getchar_unlocked()) {
      ret = (ret * 10) + (ch - '0');
    }
    ungetc(ch, stdin);
    if constexpr (std::is_unsigned<T>::value) {
      return ret;
    } else {
      return ret * sgn;
    }
  }
} input;
