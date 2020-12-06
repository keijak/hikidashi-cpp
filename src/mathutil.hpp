
// Returns floor(x / y).
i64 floordiv(i64 x, i64 y) {
  int sign = (x < 0 ? -1 : 1) * (y < 0 ? -1 : 1);
  if (sign == 1) {
    return abs(x) / abs(y);
  } else {
    i64 d = abs(y);
    i64 cd = (abs(x) + d - 1) / d;
    return -cd;
  }
}

i64 floormod(i64 x, i64 y) {
  i64 a = floordiv(x, y);
  return x - a * y;
}

// Returns ceil(x / y).
i64 ceildiv(i64 x, i64 y) {
  int sign = (x < 0 ? -1 : 1) * (y < 0 ? -1 : 1);
  if (sign == 1) {
    i64 d = abs(y);
    return (abs(x) + d - 1) / d;
  } else {
    i64 fd = abs(x) / abs(y);
    return -fd;
  }
}
