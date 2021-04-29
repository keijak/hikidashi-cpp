#include <bits/stdc++.h>
using namespace std;

int randint_example(int lo, int hi) {
  std::random_device seed_gen;
  std::mt19937_64 engine(seed_gen());
  std::uniform_int_distribution<int> rand(lo, hi);  // values in [lo, hi].
  return rand(engine);
}

void shuffle_example(vector<int>& v) {
  std::random_device seed_gen;
  std::mt19937_64 engine(seed_gen());
  std::shuffle(v.begin(), v.end(), engine);
}
