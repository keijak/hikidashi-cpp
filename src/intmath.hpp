// Integer math operations that handle negative numbers.

// Returns ceil(x / y).
i64 ceil_div(i64 x, i64 y) {
  int sign = (x < 0 ? -1 : 1) * (y < 0 ? -1 : 1);
  if (sign == 1) {
    i64 d = abs(y);
    return (abs(x) + d - 1) / d;
  } else {
    i64 q = abs(x) / abs(y);
    return -q;
  }
}

// Returns floor(x / y).
i64 floor_div(i64 x, i64 y) {
  int sign = (x < 0 ? -1 : 1) * (y < 0 ? -1 : 1);
  if (sign == 1) {
    return abs(x) / abs(y);
  } else {
    i64 d = abs(y);
    i64 q = (abs(x) + d - 1) / d;
    return -q;
  }
}

i64 floor_mod(i64 x, i64 y) {
  i64 q = floor_div(x, y);
  return x - q * y;
}
