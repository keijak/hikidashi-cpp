Mint count_permutations_no_neighbors_from_same_group(
    const std::vector<int> &freq) {
  const int n = freq.size();
  const i64 max_slots = std::accumulate(ALL(freq), 0LL) + 1;

  Factorials<Mint> fs(max_slots);
  auto dp = vector(n + 1, vector(max_slots + 1, Mint(0)));
  int slots = freq[0] + 1;  // insertion points
  dp[1][slots - 2] = 1;     // exclude left/right end points.
  for (int i = 1; i < n; ++i) {
    // j slots are violating the no-neighbor-from-same-group rule.
    for (int j = 0; j <= slots - 2; ++j) {
      if (dp[i][j].val() == 0) continue;
      const int slots_ok = slots - j;
      // Break `freq[i]` items into `p` buckets.
      for (int p = 1; p <= freq[i]; ++p) {
        const auto x = fs.C(freq[i] - 1, p - 1);
        for (int q = 0; q <= std::min(j, p); ++q) {
          const int nj = j - q + freq[i] - p;
          if (nj <= max_slots) {
            dp[i + 1][nj] += dp[i][j] * x * fs.C(j, q) * fs.C(slots_ok, p - q);
          }
        }
      }
    }
    slots += freq[i];
  }
  return dp[n][0];
}
