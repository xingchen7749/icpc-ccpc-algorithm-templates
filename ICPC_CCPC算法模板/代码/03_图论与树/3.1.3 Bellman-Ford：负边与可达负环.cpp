#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

struct Edge {
    int u, v;
    i64 w;
};
// 在 n 个点、边列表 e 中求 s 出发的最短路，并检查可达负环；点编号 1..n。
// 返回 {有无可达负环,距离}，有负环时距离无效，不可达为 i128(1)<<120。
// 允许负权，不修改输入；时间 O(nm)，空间 O(n)。
pair<bool, vector<i128>> bellman_ford(int n, const vector<Edge>& e, int s) {
    const i128 INF = (i128(1) << 120);
    vector<i128> dist(n + 1, INF);
    dist[s] = 0;
    for (int it = 0; it < n; ++it) {
        bool changed = false;
        for (auto [u, v, w] : e) {
            if (dist[u] != INF && dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                changed = true;
            }
        }
        if (!changed) return {false, dist};
        if (it == n - 1) return {true, dist};
    }
    return {false, dist};
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
