#include <bits/stdc++.h>

// Monotonic key-value map.
struct MonotonicMap {
  // Minimum necessary `cost` to get `value`.
  std::map<Int, Int> value_to_cost;

  // Maximum achievable `value` by paying `cost`.
  std::map<Int, Int> cost_to_value;

  void add(Int cost, Int value) {
    minimize_cost(value, cost);
    maximize_value(cost, value);
  }

 private:
  void minimize_cost(Int value, Int cost) {
    auto it = value_to_cost.lower_bound(value);
    if (it != value_to_cost.end() and it->second <= cost) {
      return;
    }
    value_to_cost[value] = cost;
    it = value_to_cost.find(value);
    while (it != value_to_cost.begin()) {
      --it;
      if (it->second < cost) break;
      it = value_to_cost.erase(it);
    }
  }

  void maximize_value(Int cost, Int value) {
    auto it = cost_to_value.upper_bound(cost);
    if (it != cost_to_value.begin() and prev(it)->second >= value) {
      return;
    }
    cost_to_value[cost] = value;
    it = cost_to_value.find(cost);
    ++it;
    while (it != cost_to_value.end()) {
      if (it->second > value) break;
      it = cost_to_value.erase(it);
    }
  }
};
