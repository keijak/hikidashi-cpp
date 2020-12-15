#include <bits/stdc++.h>

template <class T = Mint>
struct Factorials {
  // factorials and inverse factorials.
  std::vector<T> fact, ifact;

  // n: max cached value.
  Factorials(int n) : fact(n + 1), ifact(n + 1) {
    assert(n > 0 and n < T::mod());
    fact[0] = 1;
    for (int i = 1; i <= n; ++i) fact[i] = fact[i - 1] * i;
    ifact[n] = fact[n].inv();
    for (int i = n; i >= 1; --i) ifact[i - 1] = ifact[i] * i;
  }

  // Combination (nCk)
  T C(int n, int k) {
    if (k < 0 || k > n) return 0;
    return fact[n] * ifact[k] * ifact[n - k];
  }

  // Permutation (nPk)
  T P(int n, int k) {
    if (k < 0 || k > n) return 0;
    return fact[n] * ifact[n - k];
  }
};

// C[n][k] = nCk
// Cretates a combination matrix using Pascal's triangle.
// Time and space complexity: Θ(n^2)
// Useful to avoid factorials causing integer overflow.
template <class T = unsigned long long>
std::vector<std::vector<T>> comb_table(int n) {
  std::vector<std::vector<T>> C(n + 1, vector<T>(n + 1, 0));
  for (int i = 0; i <= n; ++i) {
    C[i][0] = C[i][i] = 1;
  }
  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j < i; ++j) {
      C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
    }
  }
  return C;
}

// nCk where n can be large but min(k, n-k) is small.
Mint comb(i64 n, i64 k) {
  i64 p = std::min(k, n - k), q = std::max(k, n - k);
  assert(p >= 0);
  Mint nume = 1, deno = 1;
  for (i64 i = n; i > q; --i) nume *= i;
  for (i64 i = 2; i <= p; ++i) deno *= i;
  return nume / deno;
}

Mint factorial(int x) {
  static std::vector<Mint> facts = {1, 1, 2};
  facts.reserve(x + 1);
  while ((int)facts.size() <= x) {
    facts.push_back(facts.back() * facts.size());
  }
  return facts[x];
}
