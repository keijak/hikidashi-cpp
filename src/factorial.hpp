long long factorial(int x) {
  static std::vector<long long> facts = {1, 1, 2};
  facts.reserve(x + 1);
  while (facts.size() <= x) {
    facts.push_back(facts.back() * facts.size());
  }
  return facts[x];
}
