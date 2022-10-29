// Usage:
//   int n = 0; read_unsigned(n);

#pragma GCC target("avx2")
#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")

#include <cctype>
#include <cstdio>
#include <type_traits>

template <typename T>
inline T rd() {
  T ret = 0;
  int ch = getchar_unlocked();
  for (; isspace(ch); ch = getchar_unlocked())
    ;
  for (; isdigit(ch); ch = getchar_unlocked()) {
    ret = (ret * 10) + (ch - '0');
  }
  // ungetc(ch, stdin);  // assumption: ch is an ignorable whitespace
  return ret;
}

template <size_t BufSize>
class StdinReader {
 public:
  StdinReader() : p{buf} {
    const size_t len = fread /* _unlocked */ (buf, 1, sizeof(buf) - 1, stdin);
    buf[len] = '\0';
    bufend = buf + len;
  }

  template <typename T>
  operator T() {
    T x;
    skip();
    assert(not is_eof());  // Couldn't read reached the end of input.
    read_one(x);
    return x;
  }

  struct Sized {
    StdinReader &reader;
    int n;
    template <typename T>
    operator T() const {
      T xs(n);
      for (auto &x : xs) {
        reader.skip();
        assert(not reader.is_eof());
        read(x);
      }
      return xs;
    }
  };
  Sized operator()(int n) const { return {*this, n}; }

  void skip() {
    while (isspace(*p)) ++p;
  }

  bool is_eof() { return p >= bufend; }

 private:
  static inline char buf[BufSize];
  char *p, *bufend;

  template <class T>
  std::enable_if_t<std::is_integral_v<T>> read_one(T &x) {
    [[maybe_unused]] int sign = 1;
    if constexpr (std::is_signed_v<T>) {
      sign = (*p == '-') ? (++p, -1) : 1;
    }
    x = 0;
    while (isdigit(*p)) x = x * 10 + (*p++ & 0x0F);
    if constexpr (std::is_signed_v<T>) {
      x *= sign;
    }
  }
  void read_one(std::string &s) {
    char *p0 = p;
    while (not isspace(*p)) p++;
    s.assign(p0, p);
  }
  void read_one(std::string_view &s) {
    const char *p0 = p;
    while (not isspace(*p)) p++;
    s = std::string_view(p0, p - p0);
  }
};
StdinReader<(1 << 26)> in;
