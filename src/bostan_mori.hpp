// Returns [x^k] P(x)/Q(x).
template <typename FPS>
typename FPS::T bostan_mori(long long k, FPS P, FPS Q) {
  typename FPS::T ret = 0;
  P.coeff_.resize(Q.size() - 1);
  while (k) {
    auto Q2 = Q;
    for (int i = 1; i < Q2.size(); i += 2) {
      Q2.coeff_[i] = -Q2[i];
    }
    const auto S = P * Q2;
    const auto T = Q * Q2;
    for (int i = int(k & 1); i < S.size(); i += 2) {
      P.coeff_[i >> 1] = S[i];
    }
    for (int i = 0; i < T.size(); i += 2) {
      Q.coeff_[i >> 1] = T[i];
    }
    k >>= 1;
  }
  ret += P[0];
  return ret;
}
