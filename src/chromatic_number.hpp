

// Chromatic Number
// g: adjacency matrix. Each row is a bitset.
int chromatic_number(const vector<uint32_t> &g) {
  uint32_t n = (int)g.size();

  // Requires: g[i][i] == true.
  for (uint32_t i = 0; i < n; ++i) assert(g[i] >> i & 1);

  const uint32_t nn = 1 << n;
  int ret = 0;
  for (uint32_t b = 0; b < nn; ++b) {
    [&]() {
      for (uint32_t i = 0; i < n; ++i) {
        if ((b & (1 << i)) and (g[i] & b) != b) return;
      }
      ret = max(ret, __builtin_popcount(b));
    }();
  }
  return ret;
}
