// https://tjkendev.github.io/procon-library/cpp/range_query/kd-tree.html
#include <algorithm>
#include <vector>
using namespace std;
using ll = long long;

#define N 500007

struct Point {
  ll x, y;
  int i;
};

int n;
Point p[N];

bool comp_x(const Point &p1, const Point &p2) { return p1.x < p2.x; }
bool comp_y(const Point &p1, const Point &p2) { return p1.y < p2.y; }

struct Node {
  Node *left, *right;
  Point p;

  Node(Node *left, Node *right, Point p) : left(left), right(right), p(p) {}
};

Node *root;

Node *make(int l, int r, int depth) {
  if (!(l < r)) {
    return nullptr;
  }

  int mid = (l + r) >> 1;
  if (depth % 2) {
    sort(p + l, p + r, comp_x);
  } else {
    sort(p + l, p + r, comp_y);
  }

  return new Node(make(l, mid, depth + 1), make(mid + 1, r, depth + 1), p[mid]);
}

// find nearest neighber
ll find(Node *nd, ll x, ll y, int depth, ll r) {
  if (nd == nullptr) return r;
  Point &p = nd->p;
  ll d = (x - p.x) * (x - p.x) + (y - p.y) * (y - p.y);
  if (r == -1 || d < r) r = d;

  if (depth % 2) {
    if (nd->left != nullptr && x - r <= p.x) {
      r = find(nd->left, x, y, depth + 1, r);
    }
    if (nd->right != nullptr && p.x <= x + r) {
      r = find(nd->right, x, y, depth + 1, r);
    }
  } else {
    if (nd->left != nullptr && y - r <= p.y) {
      r = find(nd->left, x, y, depth + 1, r);
    }
    if (nd->right != nullptr && p.y <= y + r) {
      r = find(nd->right, x, y, depth + 1, r);
    }
  }
  return r;
}

// find nodes in [tl.x, tl.y]×[br.x, br.y] (Range Search)
void find(Node *nd, vector<int> &result, ll tlx, ll tly, ll brx, ll bry,
          int depth) {
  Point &p = nd->p;
  if (tlx <= p.x && p.x <= brx && tly <= p.y && p.y <= bry) {
    result.push_back(p.i);
  }

  if (depth % 2) {
    if (nd->left != nullptr && tlx <= p.x) {
      find(nd->left, result, tlx, brx, tly, bry, depth + 1);
    }
    if (nd->right != nullptr && p.x <= brx) {
      find(nd->right, result, tlx, brx, tly, bry, depth + 1);
    }
  } else {
    if (nd->left != nullptr && tly <= p.y) {
      find(nd->left, result, tlx, brx, tly, bry, depth + 1);
    }
    if (nd->right != nullptr && p.y <= bry) {
      find(nd->right, result, tlx, brx, tly, bry, depth + 1);
    }
  }
}
