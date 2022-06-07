#include <bits/stdc++.h>
using namespace std;
using Int = long long;
using Real = long double;

// compatible with std::complex
template <typename T>
struct Point2d {
  T x;
  T y;

  Point2d() : x(0), y(0) {}
  Point2d(T x, T y) : x(x), y(y) {}
  Point2d(const Point2d &) = default;
  Point2d(Point2d &&) = default;
  Point2d &operator=(const Point2d &) = default;
  Point2d &operator=(Point2d &&) = default;
  const T &real() const { return x; }
  const T &imag() const { return y; }

  // inner product
  friend T dot(const Point2d &a, const Point2d &b) {
    return a.x * b.x + a.y * b.y;
  }

  // outer product
  friend T cross(const Point2d &a, const Point2d &b) {
    return a.x * b.y - a.y * b.x;
  }

  // |a|^2
  friend T norm(const Point2d &a) { return dot(a, a); }

  // |a|
  friend Real abs(const Point2d &a) { return std::sqrt((Real)norm(a)); }

  bool operator==(const Point2d &other) const {
    return x == other.x and y == other.y;
  }
  bool operator!=(const Point2d &other) const {
    return x != other.x or y != other.y;
  }

  Point2d &operator+=(const Point2d &other) {
    x += other.x;
    y += other.y;
    return *this;
  }
  friend Point2d operator+(const Point2d &p1, const Point2d &p2) {
    auto r = p1;
    r += p2;
    return r;
  }

  Point2d &operator-=(const Point2d &other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }
  friend Point2d operator-(const Point2d &p1, const Point2d &p2) {
    auto r = p1;
    r -= p2;
    return r;
  }

  Point2d &operator*=(T scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
  }
  friend Point2d operator*(const Point2d &p, T scalar) {
    auto r = p;
    r *= scalar;
    return r;
  }
  friend Point2d operator*(T scalar, const Point2d &p) {
    auto r = p;
    r *= scalar;
    return r;
  }
  friend ostream &operator<<(ostream &os, const Point2d &p) {
    return os << "(" << p.x << ", " << p.y << ")";
  }
};
using P = Point2d<Int>;
using L = std::pair<P, P>;  // Line
using VP = std::vector<P>;  // Points
using D = Real;

// Compare by argument angle.
// https://ngtkana.hatenablog.com/entry/2021/11/13/202103
struct ArgLess {
  bool operator()(const Point2d<Int> &a, const Point2d<Int> &b) {
    const int low1 = std::pair(a.y, a.x) < std::pair(0LL, 0LL);
    const int low2 = std::pair(b.y, b.x) < std::pair(0LL, 0LL);
    if (low1 != low2) return low1 < low2;
    return cross(a, b) > 0;
  }
};

// relative positions of a->b and a->c.
int ccw(P a, P b, P c) {
  b -= a;
  c -= a;
  if (cross(b, c) > 0) return +1;    // counter clockwise
  if (cross(b, c) < 0) return -1;    // clockwise
  if (dot(b, c) < 0) return +2;      // c--a--b on line
  if (norm(b) < norm(c)) return -2;  // a--b--c on line or a==b
  return 0;                          // a--c--b on line or a==c or b==c
}

// intersection: line and point
bool isecLP(P a1, P a2, P b) {
  return abs(ccw(a1, a2, b)) != 1;  // return EQ(cross(a2-a1, b-a1), 0); と等価
}

// intersection: line and line
bool isecLL(P a1, P a2, P b1, P b2) {
  return !isecLP(a2 - a1, b2 - b1, P(0, 0)) || isecLP(a1, b1, b2);
}

// intersection: line and segment
bool isecLS(P a1, P a2, P b1, P b2) {
  return cross(a2 - a1, b1 - a1) * cross(a2 - a1, b2 - a1) == 0;
}

// intersection: segment and segment
bool isecSS(P a1, P a2, P b1, P b2) {
  return ccw(a1, a2, b1) * ccw(a1, a2, b2) <= 0 &&
         ccw(b1, b2, a1) * ccw(b1, b2, a2) <= 0;
}

// intersection: segment and point
bool isecSP(P a1, P a2, P b) { return !ccw(a1, a2, b); }

// projection: point p onto line (a1, a2)
P proj(P a1, P a2, P p) {
  return a1 + dot(a2 - a1, p - a1) / norm(a2 - a1) * (a2 - a1);
}

// reflection point of p across line (a1, a2)
P reflection(P a1, P a2, P p) { return 2.0 * proj(a1, a2, p) - p; }

// distance between line and point
D distLP(P a1, P a2, P p) { return abs(proj(a1, a2, p) - p); }

// distance between line and line
D distLL(P a1, P a2, P b1, P b2) {
  return isecLL(a1, a2, b1, b2) ? 0.0 : distLP(a1, a2, b1);
}

// distance between line and segment
D distLS(P a1, P a2, P b1, P b2) {
  return isecLS(a1, a2, b1, b2) ? 0.0
                                : min(distLP(a1, a2, b1), distLP(a1, a2, b2));
}

// distance between segment and point
D distSP(P a1, P a2, P p) {
  P r = proj(a1, a2, p);
  if (isecSP(a1, a2, r)) return abs(r - p);
  return min(abs(a1 - p), abs(a2 - p));
}

// distance between segment and segment
D distSS(P a1, P a2, P b1, P b2) {
  if (isecSS(a1, a2, b1, b2)) return 0;
  return min(min(distSP(a1, a2, b1), distSP(a1, a2, b2)),
             min(distSP(b1, b2, a1), distSP(b1, b2, a2)));
}

// Convex Hull by Monotone Chain
// Returns {lower_hull, upper_hull}.
std::pair<std::vector<P>, std::vector<P>> scan_convex_hull(std::vector<P> ps) {
  std::sort(ps.begin(), ps.end(), [](const P &a, const P &b) {
    return std::tie(a.real(), a.imag()) < std::tie(b.real(), b.imag());
  });
  std::vector<P> lower, upper;
  for (int i = 0; i < (int)ps.size(); ++i) {
    auto ax = ps[i].x, ay = ps[i].y;
    P now{ax, ay};
    while (lower.size() >= 2) {
      P &p2 = lower[lower.size() - 2];
      P v1 = lower.back() - p2;
      P v2 = now - p2;
      if (cross(v1, v2) > 0) break;
      lower.pop_back();
    }
    lower.push_back(now);
  }
  for (int i = (int)ps.size() - 1; i >= 0; --i) {
    auto ax = ps[i].x, ay = ps[i].y;
    P now{ax, ay};
    while (upper.size() >= 2) {
      P &p2 = upper[upper.size() - 2];
      P v1 = upper.back() - p2;
      P v2 = now - p2;
      if (cross(v1, v2) > 0) break;
      upper.pop_back();
    }
    upper.push_back(move(now));
  }
  reverse(upper.begin(), upper.end());
  return {lower, upper};
}
