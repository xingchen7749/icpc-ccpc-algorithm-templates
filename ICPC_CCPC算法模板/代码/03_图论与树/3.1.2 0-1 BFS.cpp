#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

// 求源点 s 的 0/1 边权最短路；g 使用 1 下标，边记录为 {终点,0或1}。
// 返回各点距离，不可达为 1LL<<62；不修改图，时间 O(n+m)。
// BFS（Breadth-First Search）：广度优先搜索；0-1 BFS 用双端队列处理 0/1 边权。
vector<i64> bfs01(const vector<vector<pair<int, int>>>& g, int s) {
    const i64 INF = (1LL << 62);
    vector<i64> dist(g.size(), INF);
    // done[u]：顶点 u 是否已经确定最短距离并展开，防止重复处理。
    vector<char> done(g.size());
    deque<int> q;
    dist[s] = 0;
    q.push_front(s);
    while (!q.empty()) {
        int u = q.front();
        q.pop_front();
        if (done[u]) continue;
        done[u] = 1;
        for (auto [v, w] : g[u]) {
            assert(w == 0 || w == 1);
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                if (w) {
                    q.push_back(v);
                }
                else {
                    q.push_front(v);
                }
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
