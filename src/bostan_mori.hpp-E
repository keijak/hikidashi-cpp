template <typename Mint, typename FPS>
Mint bostan_mori(long long k, FPS Q, FPS P) {
  Mint ret = 0;
  P.coeff_.resize(Q.size() - 1);
  while (k) {
    auto Q2 = Q;
    for (int i = 1; i < Q2.size(); i += 2) Q2.coeff_[i] = -Q2[i];
    auto S = P * Q2;
    auto T = Q * Q2;
    if (k & 1) {
      for (int i = 1; i < S.size(); i += 2) P.coeff_[i >> 1] = S[i];
      for (int i = 0; i < T.size(); i += 2) Q.coeff_[i >> 1] = T[i];
    } else {
      for (int i = 0; i < S.size(); i += 2) P.coeff_[i >> 1] = S[i];
      for (int i = 0; i < T.size(); i += 2) Q.coeff_[i >> 1] = T[i];
    }
    k >>= 1;
  }
  return ret + P[0];
}
