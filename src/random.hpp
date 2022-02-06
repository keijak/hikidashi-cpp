#include <bits/stdc++.h>
using namespace std;
using u64 = unsigned long long;

// Time seed
// std::chrono::steady_clock::now().time_since_epoch().count()

std::mt19937_64& PRNG() {
  static std::mt19937_64 engine(std::random_device{}());
  return engine;
}

// Generates values in [lo, hi].
int rand_int(int lo, int hi) {
  std::uniform_int_distribution<int> rand(lo, hi);
  return rand(PRNG());
}

uint64_t rand_u64() {
  static std::uniform_int_distribution<uint64_t> dist(
      1000, std::numeric_limits<uint64_t>::max());
  return dist(PRNG());
}

void shuffle_example(vector<int>& v) {
  std::shuffle(v.begin(), v.end(), PRNG());
}

bool trial_loop() {
  const auto start_time = chrono::system_clock::now();
  const unsigned time_limit_check_mask = 31;  // check per 32 trials
  const unsigned time_limit_ms = 1800;
  for (unsigned trial = 0;; ++trial) {
    if ((trial & time_limit_check_mask) == 0) {
      const auto now = chrono::system_clock::now();
      const auto dur_ms =
          chrono::duration_cast<chrono::milliseconds>(now - start_time);
      if (dur_ms.count() >= time_limit_ms) break;
    }
    //
    // Compute here.
    //
  }
  return false;
}

// Custom uint64 hash function for unordered_map, etc.
// https://codeforces.com/blog/entry/62393
struct splitmix64 {
  // http://xorshift.di.unimi.it/splitmix64.c
  size_t operator()(std::uint64_t x) const {
    static const std::uint64_t FIXED_RANDOM = std::random_device{}();
    x += FIXED_RANDOM;
    x += 0x9e3779b97f4a7c15;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
    x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
    return x ^ (x >> 31);
  }
};
