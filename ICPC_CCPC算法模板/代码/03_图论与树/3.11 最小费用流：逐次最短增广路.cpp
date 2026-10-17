#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

struct MinCostFlow {
    struct E {
        // E（edge）：残量边；v：终点；rev：反向边在 g[v] 中的下标。
        int v, rev;
        // cap（capacity）：剩余容量；cost：单位流量费用，反向边费用取反。
        i64 cap, cost;
    };
    int n;
    vector<vector<E>> g;
    // 创建 n 个顶点的空费用残量网络，顶点编号为 1..n。
    MinCostFlow(int n) : n(n), g(n + 1) {}
    // 添加容量 cap、单位费用 cost 的有向边，并建立费用取反的残量边。
    // 要求端点有效且不同、cap>=0、cost!=LLONG_MIN；允许负费用。
    void add_edge(int u, int v, i64 cap, i64 cost) {
        assert(u != v && cap >= 0 && cost != LLONG_MIN);
        int a = g[u].size(), b = g[v].size();
        g[u].push_back({v, b, cap, cost});
        g[v].push_back({u, a, 0, -cost});
    }
    // 从 s 向 t 增广至多 limit 单位流，返回 {本次流量,本次总费用}，并修改残量网络。
    // 要求 s!=t、limit>=0，初始网络不能有正容量负费用环；精确需求必须检查流量等于 limit。
    // 每轮用 Bellman-Ford，若增广 A 次，时间 O(A*n*m)，费用用 i128。
    pair<i64, i128> flow(int s, int t, i64 limit) {
        assert(s != t);
        i64 f = 0;
        i128 cost = 0, INF = i128(1) << 120;
        while (f < limit) {
            vector<i128> dist(n + 1, INF);
            // pv[v]：增广路径上 v 的前驱顶点，-1 表示尚未记录。
            // pe[v]：前驱边在 g[pv[v]] 中的下标，仅在 pv[v] 有效时使用。
            vector<int> pv(n + 1, -1), pe(n + 1, -1);
            dist[s] = 0;
            for (int z = 0; z < n - 1; ++z) {
                bool changed = false;
                for (int u = 1; u <= n; ++u) {
                    if (dist[u] != INF) {
                        for (int i = 0; i < (int)g[u].size(); ++i) {
                            auto e = g[u][i];
                            if (e.cap > 0 && dist[e.v] > dist[u] + e.cost) {
                                dist[e.v] = dist[u] + e.cost;
                                pv[e.v] = u;
                                pe[e.v] = i;
                                changed = true;
                            }
                        }
                    }
                }
                if (!changed) break;
            }
            if (pv[t] == -1) break;
            i64 add = limit - f;
            for (int v = t; v != s; v = pv[v]) {
                add = min(add, g[pv[v]][pe[v]].cap);
            }
            for (int v = t; v != s; v = pv[v]) {
                E& e = g[pv[v]][pe[v]];
                // 正向剩余容量减少，反向剩余容量增加，使后续增广可以撤回已有流量。
                e.cap -= add;
                g[v][e.rev].cap += add;
            }
            f += add;
            cost += i128(add) * dist[t];
        }
        return {f, cost};
    }
};

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
