#include <bits/stdc++.h>
using namespace std;

// basis[j] = {y, i} means:
// - j-th bit を a[i] が担当する
// - y の highest 1-bit が (1 << j)
// - a[0] 〜 a[i] を使って y が作れる

using BitSet = unsigned long long;

std::vector<std::optional<std::pair<BitSet, int>>> xor_basis(
    const std::vector<BitSet> &a, int m) {
  const int n = (int)a.size();
  std::vector<std::optional<std::pair<BitSet, int>>> basis(m);
  for (int i = 0; i < n; ++i) {
    auto x = a[i];
    for (int j = m - 1; j >= 0; --j) {
      if (not(x >> j & 1)) continue;
      if (basis[j].has_value()) {
        auto [y, k] = basis[j].value();
        x ^= y;
      } else {
        basis[j] = pair(x, i);
        break;
      }
    }
  }
  return basis;
}
