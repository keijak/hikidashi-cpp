#include <bits/stdc++.h>

#include <atcoder/string>

// Returns the longest common prefix length of s[i:] and s[j:].
int lcp_length(const std::string &s, int i, int j) {
  const int n = s.size();
  const std::vector<int> sa = atcoder::suffix_array(s);
  const std::vector<int> lcp = atcoder::lcp_array(s, sa);
  std::vector<int> rsa(n);  // the position of s[i:] in the suffix array.
  for (int i = 0; i < n; ++i) rsa[sa[i]] = i;

  SparseTableRMQ rmq(lcp.size(), [&](int i, int j) { return lcp[i] < lcp[j]; });
  int p = std::min(rsa[i], rsa[j]);
  int q = std::max(rsa[i], rsa[j]);
  return lcp[rmq.fold(p, q)];
}
