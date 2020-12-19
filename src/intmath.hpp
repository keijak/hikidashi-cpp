// Integer math operations that take negative numbers into account.

// Returns ceil(x / y).
i64 ceil_div(i64 x, i64 y) {
  int sign = (x < 0 ? -1 : 1) * (y < 0 ? -1 : 1);
  if (sign == 1) {
    auto d = std::abs(y);
    return (std::abs(x) + d - 1) / d;
  } else {
    return -(std::abs(x) / std::abs(y));
  }
}

// Returns floor(x / y).
i64 floor_div(i64 x, i64 y) {
  int sign = (x < 0 ? -1 : 1) * (y < 0 ? -1 : 1);
  if (sign == 1) {
    return std::abs(x) / std::abs(y);
  } else {
    auto d = std::abs(y);
    return -((std::abs(x) + d - 1) / d);
  }
}

i64 floor_mod(i64 x, i64 y) {
  i64 q = floor_div(x, y);
  return x - q * y;
}
