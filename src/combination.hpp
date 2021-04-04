#include <bits/stdc++.h>
using Mint = double;

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
  T C(int n, int k) const {
    if (k < 0 || k > n) return 0;
    return fact[n] * ifact[k] * ifact[n - k];
  }
};

// C[n][k] = nCk
// Cretates a combination matrix using Pascal's triangle.
// Time and space complexity: Θ(n^2)
// Useful to avoid factorials causing integer overflow.
template <class T = unsigned long long>
std::vector<std::vector<T>> comb_table(int n) {
  std::vector<std::vector<T>> C(n + 1, std::vector<T>(n + 1, 0));
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
template <typename T = Mint>
T comb(long long n, long long k) {
  if (k < 0 or k > n) return 0;
  if (k > n - k) k = n - k;
  T nume = 1, deno = 1;
  for (int i = 0; i < k; ++i) {
    nume *= T(n - i);
    deno *= T(i + 1);
  }
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

// Permutation (nPk)
template <class T>
T permutation(const Factorials<T>& fs, int n, int k) {
  if (k < 0 || k > n) return 0;
  return fs.fact[n] * fs.ifact[n - k];
}

// Catalan Number
template <typename T>
T catalan(const Factorials<T>& fs, int k) {
  auto ret = fs.C(2 * k, k);
  if (k > 0) ret -= fs.C(2 * k, k - 1);
  return ret;
}

// Montmort Number
// Total number of complete permutations of size n.
template <class T>
T montmort_number(const Factorials<T>& fs, int n) {
  T res = 0;
  for (int k = 2; k <= n; ++k) {
    if (k % 2 == 0) {
      res += fs.ifact[k];
    } else {
      res -= fs.ifact[k];
    }
  }
  res *= fs.fact[n];
  return res;
}

// Bell Number B(n, k)
// Number of ways of grouping n distinguishable balls into k indistinguishable
// boxes.
template <typename T>
T bell_number(const Factorials<T>& fs, int n, int k) {
  if (n == 0) return 1;
  k = std::min(k, n);
  T ret = 0;
  std::vector<T> p(k + 1);
  p[0] = 1;
  for (int i = 1; i <= k; i++) {
    if (i & 1) {
      p[i] = p[i - 1] - fs.ifact(i);
    } else {
      p[i] = p[i - 1] + fs.ifact(i);
    }
  }
  for (int i = 1; i <= k; i++) {
    ret += T(i).pow(n) * fs.ifact(i) * p[k - i];
  }
  return ret;
}
