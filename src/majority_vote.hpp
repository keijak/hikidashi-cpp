#include <vector>

// Boyer-Moore Majority Vote Algorithm
// https://en.wikipedia.org/wiki/Boyer%E2%80%93Moore_majority_vote_algorithm
//
// Returns the majority value in O(N), assuming more than N/2 elements
// have the same value in the vector.

template <class T>
T majority_vote(const std::vector<T> a) {
  T m{};
  int c = 0;
  for (const T &x : a) {
    if (c == 0) {
      m = x;
      ++c;
    } else if (x == m) {
      ++c;
    } else {
      --c;
    }
  }
  return m;
}
