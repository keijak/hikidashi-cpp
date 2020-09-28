const double EPS = 1e-6;
const double PI = std::cos(-1);

using P = std::complex<double>;

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

template <typename T>  // T: int, double, etc.
struct Point2d {
  T x;
  T y;

  Point2d() : x(0), y(0) {}
  Point2d(T x, T y) : x(x), y(y) {}
  Point2d(const Point2d&) = default;
  Point2d(Point2d&&) = default;
  Point2d& operator=(const Point2d&) = default;
  Point2d& operator=(Point2d&&) = default;

  // inner product
  T dot(const Point2d& other) const { return x * other.x + y * other.y; }

  // outer product
  T det(const Point2d& other) const { return y * other.x - x * other.y; }

  T abs2() const { return this->dot(this); }                // |x|^2
  double abs() const { return std::sqrt((double)abs2()); }  // |x|

  Point2d& operator+=(const Point2d& other) {
    x += other.x;
    y += other.y;
    return *this;
  }
  friend Point2d operator+(const Point2d& p1, const Point2d& p2) {
    return (Point2d(p1) += p2);
  }

  Point2d& operator-=(const Point2d& other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }
  friend Point2d operator-(const Point2d& p1, const Point2d& p2) {
    return (Point2d(p1) -= p2);
  }

  Point2d& operator*=(T scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
  }
  friend Point2d operator*(const Point2d& p, T scalar) {
    return (Point2d(p) *= scalar);
  }
  friend Point2d operator*(T scalar, const Point2d& p) {
    return (Point2d(p) *= scalar);
  }
};
// using P = Point2d<long long>;

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
