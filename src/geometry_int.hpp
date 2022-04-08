#include <bits/stdc++.h>
using namespace std;
using Int = long long;
using Real = long double;

// compatible with std::complex
template <typename T>  // T: int, double, etc.
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
  friend Real abs(const Point2d &a) { return std::sqrt((Real)norm()); }

  bool operator==(const Point2d &other) {
    return x == other.x and y == other.y;
  }
  bool operator!=(const Point2d &other) { return x != other.x or y != other.y; }

  Point2d &operator+=(const Point2d &other) {
    x += other.x;
    y += other.y;
    return *this;
  }
  friend Point2d operator+(const Point2d &p1, const Point2d &p2) {
    return (Point2d(p1) += p2);
  }

  Point2d &operator-=(const Point2d &other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }
  friend Point2d operator-(const Point2d &p1, const Point2d &p2) {
    return (Point2d(p1) -= p2);
  }

  Point2d &operator*=(T scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
  }
  friend Point2d operator*(const Point2d &p, T scalar) {
    return (Point2d(p) *= scalar);
  }
  friend Point2d operator*(T scalar, const Point2d &p) {
    return (Point2d(p) *= scalar);
  }
};
using P = Point2d<Int>;
using L = std::pair<P, P>;  // Line
using VP = std::vector<P>;  // Points

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
  return !isecLP(a2 - a1, b2 - b1, 0) || isecLP(a1, b1, b2);
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
  return isecLL(a1, a2, b1, b2) ? 0 : distLP(a1, a2, b1);
}

// distance between line and segment
D distLS(P a1, P a2, P b1, P b2) {
  return isecLS(a1, a2, b1, b2) ? 0
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

// cross point of two lines
std::optional<P> crosspointLL(P a1, P a2, P b1, P b2) {
  D d1 = cross(b2 - b1, b1 - a1);
  D d2 = cross(b2 - b1, a2 - a1);
  if (EQ(d1, 0.0) && EQ(d2, 0.0)) return a1;  // same line
  if (EQ(d2, 0.0)) return std::nullopt;       // parallel
  return a1 + d1 / d2 * (a2 - a1);
}

/*** Circle ***/

// distance between line and circle
D distLC(P a1, P a2, P c, D r) {
  return std::max<D>(distLP(a1, a2, c) - r, 0.0);
}

// distance between segment and circle
D distSC(P a1, P a2, P c, D r) {
  D dSqr1 = std::norm(c - a1), dSqr2 = std::norm(c - a2), rSqr = r * r;
  // The circle intersects with the segment.
  if ((dSqr1 < rSqr) ^ (dSqr2 < rSqr)) {
    return 0;
  }
  // The circle contains the segment.
  if (dSqr1 < rSqr && dSqr2 < rSqr) {
    // Alternatively, it could be defined as 0.
    return r - std::sqrt(std::max(dSqr1, dSqr2));
  }
  return std::max<D>(distSP(a1, a2, c) - r, 0.0);
}

// cross points of a line and a circle
VP crosspointLC(P a1, P a2, P c, D r) {
  VP ps;
  P ft = proj(a1, a2, c);
  if (!GE(r * r, norm(ft - c))) return ps;

  P dir = sqrt(max<D>(r * r - norm(ft - c), 0.0)) / abs(a2 - a1) * (a2 - a1);
  ps.push_back(ft + dir);
  if (!EQ(r * r, norm(ft - c))) ps.push_back(ft - dir);
  return ps;
}

// distance between two circles
D distCC(P a, D ar, P b, D br) {
  D d = abs(a - b);
  return GE(d, abs(ar - br)) ? max<D>(d - ar - br, 0.0) : abs(ar - br) - d;
}

// cross points of two circles
VP crosspointCC(P a, D ar, P b, D br) {
  VP ps;
  P ab = b - a;
  D d = abs(ab);
  D crL = (norm(ab) + ar * ar - br * br) / (2 * d);
  if (EQ(d, 0) || ar < abs(crL)) return ps;

  P abN = ab * P(0, sqrt(ar * ar - crL * crL) / d);
  P crossp = a + crL / d * ab;
  ps.push_back(crossp + abN);
  if (!EQ(norm(abN), 0)) ps.push_back(crossp - abN);
  return ps;
}

// 点pから円aへの接線の接点
VP tangentPoints(P a, D ar, P p) {
  VP ps;
  D sinx = ar / abs(p - a);
  if (!LE(sinx, 1)) return ps;  // NaN is handled here.
  D t = PI * 0.5 - asin(min<D>(sinx, 1.0));
  ps.push_back(a + (p - a) * polar(sinx, t));
  if (!EQ(sinx, 1)) ps.push_back(a + (p - a) * polar(sinx, -t));
  return ps;
}

// 2円の共通接線。返される各直線に含まれる頂点は円との接点となる
vector<L> tangentLines(P a, D ar, P b, D br) {
  vector<L> ls;
  D d = abs(b - a);
  for (int i = 0; i < 2; ++i) {
    D sinx = (ar - (1 - i * 2) * br) / d;
    if (!LE(sinx * sinx, 1)) break;
    D cosx = sqrt(max<D>(1 - sinx * sinx, 0.0));
    for (int j = 0; j < 2; ++j) {
      P n = (b - a) * P(sinx, (1 - j * 2) * cosx) / d;
      ls.push_back(L(a + ar * n, b + (1 - i * 2) * br * n));
      if (cosx < EPS)
        break;  // 重複する接線を無視（重複していいならこの行不要）
    }
  }
  return ls;
}

// 三角形の外心。点a,b,cは同一線上にあってはならない
P circumcenter(P a, P b, P c) {
  a = (a - c) * D(0.5);
  b = (b - c) * D(0.5);
  auto crossp = crosspointLL(a, a * P(1, 1), b, b * P(1, 1));
  assert(crossp.has_value());
  return c + crossp.value();
}

// 点aと点bを通り、半径がrの円の中心を返す
VP circlesPointsRadius(P a, P b, D r) {
  VP cs;
  P abH = (b - a) * D(0.5);
  D d = abs(abH);
  if (d == 0 || d > r)
    return cs;  // 必要なら !LE(d,r) として円1つになる側へ丸める
  D dN = sqrt(r * r - d * d);  // 必要なら max(r*r - d*d, 0) とする
  P n = abH * P(0, 1) * (dN / d);
  cs.push_back(a + abH + n);
  if (dN > 0) cs.push_back(a + abH - n);
  return cs;
}

// 点aと点bを通り、直線lに接する円の中心
VP circlesPointsTangent(P a, P b, P l1, P l2) {
  P n = (l2 - l1) * P(0, 1);
  P m = (b - a) * P(0, 0.5);
  D rC = dot((a + b) * D(0.5) - l1, n);
  D qa = norm(n) * norm(m) - dot(n, m) * dot(n, m);
  D qb = -rC * dot(n, m);
  D qc = norm(n) * norm(m) - rC * rC;
  D qd = qb * qb - qa * qc;  // qa*k^2 + 2*qb*k + qc = 0

  VP cs;
  if (qd < -EPS) return cs;
  if (EQ(qa, 0)) {
    if (!EQ(qb, 0)) cs.push_back((a + b) * D(0.5) - m * (qc / qb / 2));
    return cs;
  }
  D t = -qb / qa;
  cs.push_back((a + b) * D(0.5) + m * (t + sqrt(max<D>(qd, 0.0)) / qa));
  if (qd > EPS) {
    cs.push_back((a + b) * D(0.5) + m * (t - sqrt(max<D>(qd, 0.0)) / qa));
  }
  return cs;
}

// 点集合を含む最小の円の中心
P minEnclosingCircle(const VP &ps) {
  P c = {0, 0};
  for (const P &p : ps) c += p;
  c /= D(ps.size());

  D move_delta = 0.5;
  for (int i = 0; i < 39; ++i) {  // 2^(-39-1) \approx 0.9e-12
    for (int t = 0; t < 50; ++t) {
      D d_max = 0;
      int k = 0;  // the most distant point
      for (int j = 0; j < (int)ps.size(); ++j) {
        D d = norm(ps[j] - c);
        if (d_max < d) {
          d_max = d;
          k = j;
        }
      }
      c += (ps[k] - c) * move_delta;
    }
    move_delta *= 0.5;
  }
  return c;
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
      if (cross(v1, v2) > EPS) break;
      lower.pop_back();
    }
    lower.push_back(move(now));
  }
  for (int i = ps.size() - 1; i >= 0; --i) {
    auto ax = ps[i].x, ay = ps[i].y;
    P now{ax, ay};
    while (upper.size() >= 2) {
      P &p2 = upper[upper.size() - 2];
      P v1 = upper.back() - p2;
      P v2 = now - p2;
      if (cross(v1, v2) > EPS) break;
      upper.pop_back();
    }
    upper.push_back(move(now));
  }
  reverse(upper.begin(), upper.end());
  return {lower, upper};
}
