#include <bits/stdc++.h>

template <class Ring>
struct GenericMatrix {
  using T = typename Ring::T;
  using Row = std::vector<T>;
  std::vector<Row> data;

  GenericMatrix() = default;
  GenericMatrix(int n, int m) : data(n, Row(m, Ring::zero())) {}
  explicit GenericMatrix(int n) : data(n, Row(n, Ring::zero())){};
  explicit GenericMatrix(std::vector<Row> a) : data(std::move(a)) {}
  GenericMatrix(std::initializer_list<Row> a) : data(std::move(a)) {}

  inline int height() const { return (int)(data.size()); }

  inline int width() const { return (int)(data[0].size()); }

  inline const Row &operator[](int k) const { return data[k]; }

  inline Row &operator[](int k) { return data[k]; }

  static GenericMatrix I(int n) {
    GenericMatrix mat(n);
    for (int i = 0; i < n; i++) mat[i][i] = Ring::one();
    return mat;
  }

  static GenericMatrix row(std::vector<T> v) {
    GenericMatrix mat(1, v.size());
    mat[0] = std::move(v);
    return mat;
  }

  static GenericMatrix column(const std::vector<T> &v) {
    const int n = v.size();
    GenericMatrix mat(n, 1);
    for (int i = 0; i < n; ++i) mat[i][0] = v[i];
    return mat;
  }

  GenericMatrix transpose() const {
    int n = height(), m = width();
    GenericMatrix mat(m, n);
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) {
        mat[j][i] = (*this)[i][j];
      }
    }
    return mat;
  }

  GenericMatrix &operator+=(const GenericMatrix &B) {
    int n = height(), m = width();
    assert(n == B.height() and m == B.width());
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) {
        (*this)[i][j] = Ring::add((*this)[i][j], B[i][j]);
      }
    }
    return *this;
  }

  GenericMatrix &operator*=(const GenericMatrix &B) {
    int n = height(), m = B.width(), p = width();
    assert(p == B.height());
    std::vector<Row> C(n, Row(m, Ring::zero()));
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        for (int k = 0; k < p; k++) {
          C[i][j] = Ring::add(C[i][j], Ring::mul((*this)[i][k], B[k][j]));
        }
      }
    }
    data.swap(C);
    return *this;
  }

  GenericMatrix &operator^=(long long k) {
    auto B = GenericMatrix::I(height());
    while (k > 0) {
      if (k & 1) B *= *this;
      *this *= *this;
      k >>= 1LL;
    }
    data.swap(B.data);
    return *this;
  }

  GenericMatrix operator+(const GenericMatrix &B) const {
    auto r = *this;
    r += B;
    return r;
  }

  GenericMatrix operator*(const GenericMatrix &B) const {
    auto r = *this;
    r *= B;
    return r;
  }

  GenericMatrix operator^(const long long k) const {
    auto r = *this;
    r ^= k;
    return r;
  }

  GenericMatrix pow(const long long k) const { return (*this) ^ k; }

  friend std::ostream &operator<<(std::ostream &os, const GenericMatrix &p) {
    int n = p.height();
    for (int i = 0; i < n; i++) {
      os << p[i] << "\n";
    }
    return os;
  }
};

struct MinMaxRing {
  using T = int;
  static T add(const T &x, const T &y) { return std::min(x, y); }
  static T zero() { return std::numeric_limits<T>::max(); }
  static T mul(const T &x, const T &y) { return std::max(x, y); }
  static T one() { return 0; }
};
