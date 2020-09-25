struct Min {
  using T = long long;
  static T op(const T &x, const T &y) { return std::min(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::max(); }
};

struct Max {
  using T = long long;
  static T op(const T &x, const T &y) { return std::max(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::min(); }
};

struct Sum {
  using T = long long;
  static T op(const T &x, const T &y) { return x + y; }
  static constexpr T id() { return 0; }
};

struct Prod {
  using T = long long;
  static T op(const T &x, const T &y) { return x * y; }
  static constexpr T id() { return 1; }
};

// Keeps the value with the latest time.
struct Assign {
  // Value with a timestamp.
  struct T {
    int time;
    long long value;
  };
  static T op(const T &x, const T &y) { return (x.time > y.time) ? x : y; }
  static T id() { return {std::numeric_limits<int>::min(), 0}; }
};

struct GCD {
  using T = long long;
  static T op(const T &x, const T &y) {
    if (x == id()) return y;
    if (y == id()) return x;
    return std::gcd(x, y);
  }
  static constexpr T id() { return -1; }
};

struct AddMin {
  using T = long long;
  using F = long long;

  // Fold: Min
  static T op(const T &x, const T &y) { return std::min(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::max(); }

  // Update: Add
  static T f_apply(const F &f, const T &x) { return f + x; }
  static F f_compose(const F &f, const F &g) { return f + g; }
  static constexpr F f_id() { return 0; }
};

struct AddMax {
  using T = long long;
  using F = long long;

  // Fold: Max
  static T op(const T &x, const T &y) { return std::max(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::min(); }

  // Update: Add
  static T f_apply(const F &f, const T &x) { return f + x; }
  static F f_compose(const F &f, const F &g) { return f + g; }
  static constexpr F f_id() { return 0; }
};

struct AddSum {
  struct T {
    long long sum;
    int width;
  };
  using F = long long;

  // Fold: Sum
  static T op(const T &x, const T &y) {
    return {x.sum + y.sum, x.width + y.width};
  }
  static constexpr T id() { return {0, 0}; }

  // Update: Add
  static T f_apply(const F &f, const T &x) {
    return {x.sum + f * x.width, x.width};
  }
  static F f_compose(const F &f, const F &g) { return f + g; }
  static constexpr F f_id() { return 0; }
};

struct AssignMin {
  using T = long long;
  using F = std::optional<long long>;

  // Fold: Min
  static T op(const T &x, const T &y) { return std::min(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::max(); }

  // Update: Assign
  static T f_apply(const F &f, const T &x) {
    return f.has_value() ? f.value() : x;
  }
  static F f_compose(const F &f, const F &g) { return f.has_value() ? f : g; }
  static constexpr F f_id() { return std::nullopt; }
};

struct AssignMax {
  using T = long long;
  using F = std::optional<long long>;

  // Fold: Max
  static T op(const T &x, const T &y) { return std::max(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::min(); }

  // Update: Assign
  static T f_apply(const F &f, const T &x) {
    return f.has_value() ? f.value() : x;
  }
  static F f_compose(const F &f, const F &g) { return f.has_value() ? f : g; }
  static constexpr F f_id() { return std::nullopt; }
};

struct AssignSum {
  struct T {
    long long sum;
    int width;
  };
  using F = std::optional<long long>;

  // Fold: Sum
  static T op(const T &x, const T &y) {
    return {x.sum + y.sum, x.width + y.width};
  }
  static constexpr T id() { return {0, 0}; }

  // Update: Assign
  static T f_apply(const F &f, const T &x) {
    return f.has_value() ? {f.value() * x.width, x.width} : x;
  }
  static F f_compose(const F &f, const F &g) { return f.has_value() ? f : g; }
  static constexpr F f_id() { return std::nullopt; }
};

struct MinMin {
  using T = long long;
  using F = long long;

  // Fold: Min
  static T op(const T &x, const T &y) { return std::min(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::max(); }

  // Update: Min
  static T f_apply(const F &f, const T &x) { return std::min(f, x); }
  static F f_compose(const F &f, const F &g) { return std::min(f, g); }
  static constexpr F f_id() { return std::numeric_limits<T>::max(); }
};

struct MaxMax {
  using T = long long;
  using F = long long;

  // Fold: Max
  static T op(const T &x, const T &y) { return std::max(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::min(); }

  // Update: Max
  static T f_apply(const F &f, const T &x) { return std::max(f, x); }
  static F f_compose(const F &f, const F &g) { return std::max(f, g); }
  static constexpr F f_id() { return std::numeric_limits<T>::min(); }
};
