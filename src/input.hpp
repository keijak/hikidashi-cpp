// Usage:
//   int n = 0; read_unsigned(n);

#pragma GCC target("avx2")
#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")

#include <cctype>
#include <cstdio>
#include <type_traits>

template <typename T>
inline void read_unsigned(T& ret) {
  // ret = 0;  // assumption: ret is already zero-initialized.
  int ch = getchar_unlocked();
  for (; isspace(ch); ch = getchar_unlocked())
    ;
  for (; isdigit(ch); ch = getchar_unlocked()) {
    ret = (ret * 10) + (ch - '0');
  }
  // ungetc(ch, stdin);  // assumption: ch is an ignorable whitespace
}

template <typename T>
inline void read_signed(T& ret) {
  // ret = 0;  // assumption: ret is already zero-initialized.
  int ch = getchar_unlocked(), sign = 1;
  for (; isspace(ch); ch = getchar_unlocked())
    ;
  if (ch == '-') {
    sign = -1;
    ch = getchar_unlocked();
  }
  for (; isdigit(ch); ch = getchar_unlocked()) {
    ret = (ret * 10) + (ch - '0');
  }
  ret *= sign;
  // ungetc(ch, stdin);  // assumption: ch is an ignorable whitespace
}
