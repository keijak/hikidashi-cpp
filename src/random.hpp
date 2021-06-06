#include <bits/stdc++.h>
using namespace std;
using u64 = unsigned long long;

std::mt19937_64& PRNG() {
  static std::random_device seed_gen;
  static std::mt19937_64 engine(seed_gen());  // non-deterministic
  return engine;
}

int randint_example(int lo, int hi) {
  std::uniform_int_distribution<int> rand(lo, hi);  // values in [lo, hi].
  return rand(PRNG());
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
