// Usage:
//   int n = input;

#include <cctype>
#include <cstdio>

struct Input {
  operator int() { return read_int<int>(); }
  operator long long() { return read_int<long long>(); }

 private:
  template <typename T>
  T read_int() {
    T ret = 0, sgn = 1;
    int ch = getchar_unlocked();
    while (isspace(ch)) {
      ch = getchar_unlocked();
    }
    if (ch == '-') {
      sgn = -1;
      ch = getchar_unlocked();
    }
    for (; isdigit(ch); ch = getchar_unlocked()) ret = (ret * 10) + (ch - '0');
    ungetc(ch, stdin);
    return sgn * ret;
  }
} input;
