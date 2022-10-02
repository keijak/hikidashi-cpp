#include <bits/stdc++.h>
using namespace std;

using Uint = unsigned long long;

// Noshi Basis
//
// basis[j] = {x, i} means:
// - x の highest bit を a[i] が担当する
// - a[0] 〜 a[i] を使って x が作れる
//
// ref. https://qiita.com/Sooh31/items/97e2d9724fe47ed238b3

std::vector<std::pair<Uint, int>> xor_basis(const std::vector<Uint>& a) {
  std::vector<std::pair<Uint, int>> basis;
  for (int i = 0; i < (int)a.size(); ++i) {
    auto x = a[i];
    for (const auto& [xk, k] : basis) {
      x = std::min(x, x ^ xk);
    }
    if (x != 0) {
      basis.emplace_back(x, i);
    }
  }
  return basis;
}

// basis[j] = {x, i} means:
// - x の highest bit が j であり、a[i] が担当する
// - a[0] 〜 a[i] を使って x が作れる

std::vector<std::optional<std::pair<Uint, int>>> xor_basis(
    const std::vector<Uint>& a, int m) {
  std::vector<std::optional<std::pair<Uint, int>>> basis(m);
  for (int i = 0; i < (int)a.size(); ++i) {
    auto x = a[i];
    for (int j = m - 1; j >= 0; --j) {
      if (not(x >> j & 1)) continue;
      if (basis[j].has_value()) {
        auto [xk, k] = basis[j].value();
        x ^= xk;
      } else {
        basis[j] = pair(x, i);
        break;
      }
    }
  }
  return basis;
}
