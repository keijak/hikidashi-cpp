#include <bits/stdc++.h>

#include <atcoder/string>

// Returns the longest common prefix length of s[i:] and s[j:].
int lcp_length(const std::string &s) {
  // Initialize
  assert(not s.empty());
  const std::vector<int> sa = atcoder::suffix_array(s);
  const std::vector<int> lcp = atcoder::lcp_array(s, sa);
  assert((int)lcp.size() == (int)s.size() - 1);
  std::vector<int> rsa(s.size());  // the position of s[i:] in the suffix array.
  for (int i = 0; i < (int)s.size(); ++i) {
    rsa[sa[i]] = i;
  }
  SparseTableRMQ rmq(lcp.size(), [&](int i, int j) { return lcp[i] < lcp[j]; });

  // Query
  auto query_lcp = [&](int i, int j) -> int {
    if (i == j) {
      return n - i;
    }
    int ri = rsa[i], rj = rsa[j];
    if (ri > rj) std::swap(ri, rj);
    assert(ri < rj);
    return lcp[rmq.fold_index(ri, rj)];
  };
}
