#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

// 对 g[1..n] 做拓扑排序，返回无占位的顶点顺序列表；不修改图。
// 返回长度不足 n 表示存在有向环，此时列表并非全图拓扑序；时间 O(n+m)。
// DAG（Directed Acyclic Graph）：有向无环图。
// DP（Dynamic Programming）：动态规划，可按 DAG 的拓扑序转移。
vector<int> topo_sort(const vector<vector<int>>& g) {
    int n = (int)g.size() - 1;
    // deg[u]：尚未删除的入边数量；ans：依次取出的拓扑序顶点。
    vector<int> deg(n + 1), ans;
    queue<int> q;
    for (auto& es : g) {
        for (int v : es) ++deg[v];
    }
    for (int i = 1; i <= n; ++i) {
        if (!deg[i]) q.push(i);
    }
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        ans.push_back(u);
        for (int v : g[u]) {
            if (--deg[v] == 0) q.push(v);
        }
    }
    return ans;
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
