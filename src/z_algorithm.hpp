#include <string_view>
#include <vector>

// Z-algorithm
// Returns the Z array of the given string.
// Z[i] := the longest common prefix length between S[0:] and S[i:].
std::vector<int> ZArray(std::string_view s) {
  const int n = s.size();
  std::vector<int> Z(n);
  Z[0] = n;
  int i = 1, j = 0;
  while (i < n) {
    while (i + j < n and s[j] == s[i + j]) ++j;
    Z[i] = j;
    if (j == 0) {
      ++i;
      continue;
    }
    int k = 1;
    for (; k < j and k + Z[k] < j; ++k) {
      Z[i + k] = Z[k];
    }
    i += k;
    j -= k;
  }
  return Z;
}
