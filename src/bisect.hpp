int bisect(int ok, int ng, function<bool(int)> pred) {
  // assert(pred(ok));
  // assert(!pred(ng));
  while (abs(ok - ng) > 1) {
    int mid = (ok + ng) / 2;
    if (pred(mid)) {
      ok = mid;
    } else {
      ng = mid;
    }
  }
  return ok;
}

int bisect_max(int ok, int ng, function<bool(int)> pred) {
  assert(ok < ng);
  return bisect(ok, ng, pred);
}
int bisect_min(int ok, int ng, function<bool(int)> pred) {
  assert(ok > ng);
  return bisect(ok, ng, pred);
}
