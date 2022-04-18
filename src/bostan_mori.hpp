// Returns [x^n] P(x)/Q(x).
template <typename FPS>
typename FPS::T bostan_mori(FPS P, FPS Q, long long n) {
  typename FPS::T ret = 0;
  P.coeff_.resize(Q.size() - 1);
  while (n) {
    auto Q2 = Q;
    for (int i = 1; i < Q2.size(); i += 2) {
      Q2.coeff_[i] = -Q2[i];
    }
    const auto S = P * Q2;
    const auto T = Q * Q2;
    for (int i = int(n & 1); i < S.size(); i += 2) {
      P.coeff_[i >> 1] = S[i];
    }
    for (int i = 0; i < T.size(); i += 2) {
      Q.coeff_[i >> 1] = T[i];
    }
    n >>= 1;
  }
  ret += P[0];
  return ret;
}

// Returns the n-th term (0-indexed) of a linear recurrence.
// - initial k terms: a[0], a[1], ..., a[k-1].
// - a[n] = c[0]*a[n-1] + c[1]*a[n-2] + ... + c[k-1]*a[n-k]
//
// Calculates [x^n]P(x)/Q(x) where
// - Q(x) = 1 - c[0]*x - c[1]*x^2 - ... - c[k-1]*x^k
// - P(x) = Q(x) * (a[0] + a[1]*x + ... + a[k-1]*x^(k-1))
template <typename FPS>
Mint linear_recurrence(const vector<Mint> &a, const vector<Mint> &c,
                       long long n) {
  assert(n >= 0);
  vector<Mint> q(c.size() + 1);
  q[0] = 1;
  for (int i = 0; i < (int)c.size(); ++i) {
    q[i + 1] = -c[i];
  }
  FPS Q(std::move(q));
  FPS P(a);
  P *= Q;
  P.coeff_.resize(Q.size() - 1);
  return bostan_mori(P, Q, n);
}
