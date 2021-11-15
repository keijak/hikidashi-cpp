#include <bits/stdc++.h>
using namespace std;

template <typename Mint>
vector<Mint> berlekamp_massey(const vector<Mint> &s) {
  const int N = (int)s.size();
  vector<Mint> b, c;
  b.reserve(N + 1);
  c.reserve(N + 1);
  b.push_back(Mint(1));
  c.push_back(Mint(1));
  Mint y = Mint(1);
  for (int ed = 1; ed <= N; ed++) {
    int l = int(c.size()), m = int(b.size());
    Mint x = 0;
    for (int i = 0; i < l; i++) x += c[i] * s[ed - l + i];
    b.emplace_back(Mint(0));
    m++;
    if (x == Mint(0)) continue;
    Mint freq = x / y;
    if (l < m) {
      auto tmp = c;
      c.insert(begin(c), m - l, Mint(0));
      for (int i = 0; i < m; i++) c[m - 1 - i] -= freq * b[m - 1 - i];
      b = tmp;
      y = x;
    } else {
      for (int i = 0; i < m; i++) c[l - 1 - i] -= freq * b[m - 1 - i];
    }
  }
  reverse(begin(c), end(c));
  return c;
}
