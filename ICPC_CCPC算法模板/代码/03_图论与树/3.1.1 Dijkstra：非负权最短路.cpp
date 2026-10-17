#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

const i64 INF = (1LL << 62);

// 求源点 s 到各点的最短距离，g[u] 存 {终点,边权}，顶点使用 1..n。
// 要求边权非负，返回 dist[1..n]，不可达为 INF，有限答案须小于 INF；不修改图。
// 堆实现时间 O((n+m)log(n+m))，空间 O(n+m)。
vector<i64> dijkstra(const vector<vector<pair<int, i64>>>& g, int s) {
    vector<i64> dist(g.size(), INF);
    dist[s] = 0;
    priority_queue<pair<i64, int>, vector<pair<i64, int>>, greater<pair<i64, int>>> q;
    q.push({0, s});
    while (!q.empty()) {
        auto [du, u] = q.top();
        q.pop();
        if (du != dist[u]) continue;
        for (auto [v, w] : g[u]) {
            assert(w >= 0);
            if (w >= INF - du) continue;
            if (dist[v] > du + w) {
                dist[v] = du + w;
                q.push({dist[v], v});
            }
        }
    }
    return dist;
}

void solve() {
    // Read one test case. Arrays and vertices use 1..n.
}
signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T = 1;
    cin >> T; // Remove for a single test case.
    while (T--) solve();
}
