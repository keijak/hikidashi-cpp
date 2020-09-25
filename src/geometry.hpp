using P = complex<double>;
const double EPS = 1e-6;
const double PI = cos(-1);

// inner product
// Checks if two vectors are orthogonal.
double dot(const P& p1, const P& p2) {
  return real(p1) * real(p2) + imag(p1) * imag(p2);
}

// outer product
// Checks if two vectors are parallel.
double det(const P& p1, const P& p2) {
  return imag(p1) * real(p2) - real(p1) * imag(p2);
}

// Monotone Chain
void scan_convex_hull(const vector<P>& ps, vector<P>& upper, vector<P>& lower) {
  for (int i = 0; i < (int)ps.size(); ++i) {
    double ax = ps[i].real(), ay = ps[i].imag();
    P now{ax, ay};
    while (upper.size() >= 2) {
      P& p2 = upper[upper.size() - 2];
      P v1 = upper.back() - p2;
      P v2 = now - p2;
      if (det(v1, v2) > EPS) {
        break;
      }
      upper.pop_back();
    }
    upper.push_back(move(now));
  }
  for (int i = ps.size() - 1; i >= 0; --i) {
    double ax = ps[i].real(), ay = ps[i].imag();
    P now{ax, ay};
    while (lower.size() >= 2) {
      P& p2 = lower[lower.size() - 2];
      P v1 = lower.back() - p2;
      P v2 = now - p2;
      if (det(v1, v2) > EPS) {
        break;
      }
      lower.pop_back();
    }
    lower.push_back(move(now));
  }
  reverse(lower.begin(), lower.end());
}
