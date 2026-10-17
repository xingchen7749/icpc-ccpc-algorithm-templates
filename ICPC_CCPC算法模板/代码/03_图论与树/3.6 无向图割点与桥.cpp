#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

struct LowLink {
    int n, timer = 0;
    vector<vector<pair<int, int>>> g;
    // DFS（Depth-First Search）：深度优先搜索。
    // dfn[u]：首次访问时间戳；low[u]：从子树经树边及至多一条返祖边可达的最小时间戳。
    // bridges：桥的边编号列表；timer：访问时间戳计数器。
    vector<int> dfn, low, bridges;
    // cut[u]：顶点 u 是否为割点。
    vector<char> cut;
    // 创建 n 个顶点的无向图，初始化访问时间、low 值和割点标记；点编号为 1..n。
    LowLink(int n) : n(n), g(n + 1), dfn(n + 1), low(n + 1), cut(n + 1) {}
    // 添加无向边 u-v，两个方向使用同一 id；每条物理边需有独立非负 id，以支持重边。
    void add_edge(int u, int v, int id) {
        g[u].push_back({v, id});
        g[v].push_back({u, id});
    }
    // 内部割点/桥搜索：从未访问顶点 u 出发，pe 为父边编号，根使用 -1。
    // 更新 low、cut 和 bridges，按父边 id 跳过反向边；递归深度最多 O(n)。
    void dfs(int u, int pe) {
        dfn[u] = low[u] = ++timer;
        int child = 0;
        for (auto [v, id] : g[u]) {
            if (id == pe) continue;
            if (!dfn[v]) {
                ++child;
                dfs(v, id);
                low[u] = min(low[u], low[v]);
                if (low[v] > dfn[u]) {
                    bridges.push_back(id);
                }
                if (pe != -1 && low[v] >= dfn[u]) {
                    cut[u] = 1;
                }
            }
            else {
                low[u] = min(low[u], dfn[v]);
            }
        }
        if (pe == -1 && child > 1) {
            cut[u] = 1;
        }
    }
    // 遍历所有连通块，写入割点标记 cut[1..n] 和桥的边编号列表 bridges，时间 O(n+m)。
    // 应在全部边添加完后调用一次；本实现不清空旧状态，修改图后需重建对象。
    void build() {
        for (int i = 1; i <= n; ++i) {
            if (!dfn[i]) dfs(i, -1);
        }
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
