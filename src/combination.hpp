#include <bits/stdc++.h>

#include <atcoder/modint>
using Int = long long;
using Mint = atcoder::modint1000000007;

// mod: prime
template <class T = Mint>
struct BinomialCoeff {
  // factorials and inverse factorials.
  std::vector<T> fact, ifact;

  // n: max cached value.
  explicit BinomialCoeff(int n) : fact(n + 1), ifact(n + 1) {
    assert(n >= 0);
    assert(n < T::mod());
    fact[0] = 1;
    for (int i = 1; i <= n; ++i) {
      fact[i] = fact[i - 1] * i;
    }
    ifact[n] = fact[n].inv();
    for (int i = n; i >= 1; --i) {
      ifact[i - 1] = ifact[i] * i;
    }
  }

  // Combination (binomial coefficients)
  T operator()(Int n, Int k) const {
    if (k < 0 || k > n) return 0;
    return fact[n] * ifact[k] * ifact[n - k];
  }
};

// mod: prime
template <class T = Mint>
struct BinomialCoeffSmallMod {
  // factorials and inverse factorials.
  std::vector<T> fact, ifact;
  std::vector<int> mcount;  // How many MOD factors the factorial includes.

  // n: max cached value. can be greater than the mod.
  BinomialCoeffSmallMod(int n) : fact(n + 1), ifact(n + 1), mcount(n + 1) {
    assert(n >= 0);
    fact[0] = 1;
    for (int i = 1; i <= n; ++i) {
      int x = i;
      int count = 0;
      while (x % Mint::mod() == 0) {
        x /= Mint::mod();
        ++count;
      }
      fact[i] = fact[i - 1] * x;
      mcount[i] = mcount[i - 1] + count;
    }
    ifact[n] = fact[n].inv();
    for (int i = n; i >= 1; --i) {
      int x = i;
      while (x % Mint::mod() == 0) {
        x /= Mint::mod();
      }
      ifact[i - 1] = ifact[i] * x;
    }
  }

  // Combination (binomial coefficients)
  T operator()(Int n, Int k) const {
    if (k < 0 || k > n) return 0;
    int p = mcount[n] - mcount[k] - mcount[n - k];
    if (p > 0) return 0;
    assert(p == 0);
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
T comb(Int n, Int k) {
  if (k < 0 or k > n) return 0;
  if (k > n - k) k = n - k;
  T nume = 1, deno = 1;
  for (int i = 0; i < k; ++i) {
    nume *= T(n - i);
    deno *= T(i + 1);
  }
  return nume / deno;
}

// Lucas' Theorem
// P: prime
Mint comb_mod(Int n, Int k) {
  int P = Mint::mod();
  Mint ret = 1;
  while (n > 0 and k > 0) {
    Int ni = n % P, ki = k % P;
    ret *= comb<Mint>(ni, ki);
    n /= P, k /= P;
  }
  return ret;
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
T P(const BinomialCoeff<T>& binom, int n, int k) {
  if (k < 0 || k > n) return 0;
  return binom.fact[n] * binom.ifact[n - k];
}

// Catalan Number
template <typename T>
T catalan(const BinomialCoeff<T>& binom, int k) {
  auto ret = binom(2 * k, k);
  if (k > 0) ret -= binom(2 * k, k - 1);
  return ret;
}

// Montmort Number
// Total number of complete permutations of size n.
template <class T>
T montmort_number(const BinomialCoeff<T>& binom, int n) {
  T res = 0;
  for (int k = 2; k <= n; ++k) {
    if (k % 2 == 0) {
      res += binom.ifact[k];
    } else {
      res -= binom.ifact[k];
    }
  }
  res *= binom.fact[n];
  return res;
}

// Bell Number B(n, k)
// Number of ways of grouping n distinguishable balls into k or less
// indistinguishable boxes.
template <typename T>
T bell_number(const BinomialCoeff<T>& binom, int n, int k) {
  if (n == 0) return 1;
  k = std::min(k, n);
  T ret = 0;
  std::vector<T> p(k + 1);
  p[0] = 1;
  for (int i = 1; i <= k; i++) {
    if (i & 1) {
      p[i] = p[i - 1] - binom.ifact(i);
    } else {
      p[i] = p[i - 1] + binom.ifact(i);
    }
  }
  for (int i = 1; i <= k; i++) {
    ret += T(i).pow(n) * binom.ifact(i) * p[k - i];
  }
  return ret;
}
