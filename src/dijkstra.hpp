template <class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

struct Edge {
  i64 cost;
  int to;
};

struct State {
  i64 cost;
  int node;
  //...
};
bool operator>(const State &x, const State &y) { return x.cost > y.cost; }

void dijkstra(int n, const V<V<tuple<int, int>>> &g) {
  const i64 INF = 1e18;
  V<i64> mincost(n, INF);
  MinHeap<State> heap;
  heap.push({0, 0});
  while (not heap.empty()) {
    State cur = move(heap.top());
    heap.pop();
    if (cur.cost > mincost[cur.node]) continue;
    for (auto [cost, to] : g[cur.node]) {
      i64 new_cost = cur.cost + cost;
      if (mincost[to] > new_cost) {
        mincost[to] = new_cost;
        heap.push({new_cost, to});
      }
    }
  }
}
