#include <bits/stdc++.h>

#include <atcoder/modint>
using std::vector;

// Adjacency Matrix
using Graph = vector<vector<bool>>;

// Chromatic Number: O(2^n n)
// ref. https://ei1333.github.io/luzhiled/snippets/graph/chromatic-number.html

template <uint32_t mod>
int chromatic_number_internal(const vector<uint32_t> &es,
                              const vector<uint32_t> &ind, int upper) {
  const uint32_t N = es.size();
  const uint32_t NN = 1u << N;
  vector<uint32_t> aux(NN, 1);
  for (int i = 1; i < upper; ++i) {
    uint64_t all = 0;
    for (uint32_t j = 0; j < NN; ++j) {
      uint32_t S = j ^ (j >> 1);
      aux[S] = ((uint64_t)aux[S] * ind[S]) % mod;
      all += (j & 1) ? aux[S] : mod - aux[S];
    }
    if (all % mod) return i;
  }
  return upper;
}

int chromatic_number(const Graph &graph) {
  int N = (int)graph.size();
  vector<uint32_t> es(N);
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (graph[i][j]) es[i] |= 1 << j;
    }
  }
  vector<uint32_t> ind(1 << N);
  ind[0] = 1;
  for (uint32_t S = 1; S < 1 << N; S++) {
    const uint32_t u = __builtin_ctz(S);
    ind[S] = ind[S ^ (1 << u)] + ind[(S ^ (1 << u)) & ~es[u]];
  }
  int ret = N;
  ret = chromatic_number_internal<(int)1e9 + 7>(es, ind, ret);
  ret = chromatic_number_internal<(int)1e9 + 11>(es, ind, ret);
  ret = chromatic_number_internal<(int)1e9 + 21>(es, ind, ret);
  return ret;
}

// Chromatic Number: O(2^n n log n)
// ref. https://drken1215.hatenablog.com/entry/2019/01/16/030000
int chromatic_number2(const Graph &graph) {
  using Mint = atcoder::modint1000000007;
  int n = (int)graph.size();
  vector<int> neighbor(n, 0);
  for (int i = 0; i < n; ++i) {
    int S = (1 << i);
    for (int j = 0; j < n; ++j) {
      if (graph[i][j]) S |= (1 << j);
    }
    neighbor[i] = S;
  }

  // I[S] := #. of inndepndet subset of S
  vector<int> I(1 << n);
  I[0] = 1;
  for (int S = 1; S < (1 << n); ++S) {
    int v = __builtin_ctz(S);
    I[S] = I[S & ~(1 << v)] + I[S & ~neighbor[v]];
  }
  int low = 0, high = n;
  while (high - low > 1) {
    int mid = (low + high) >> 1;

    // Inclusion-Exclusion Principle.
    //   g[S] := #. of "k independent sets" which cover S just
    //   f[S] := #. of "k independent sets" which consist of subseta of S
    // then
    //   f[S] = sum_{T in S} g(T)
    //   g[S] = sum_{T in S} (-1)^(|S|-|T|)f[T]
    Mint g = 0;
    for (int S = 0; S < (1 << n); ++S) {
      int sign = ((n - __builtin_popcount(S)) & 1) ? -1 : 1;
      g += sign * Mint(I[S]).pow(mid);
    }
    if (g.val() != 0) {
      high = mid;
    } else {
      low = mid;
    }
  }
  return high;
}
