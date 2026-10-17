#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

// SCC（Strongly Connected Component）：强连通分量；此处采用 Tarjan 算法。
// DFS（Depth-First Search）：深度优先搜索。
struct TarjanSCC {
    int n, cc = 0, timer = 0;
    vector<vector<int>> g;
    // dfn[u]：首次访问时间戳，0 表示未访问；low[u]：按 Tarjan 转移维护的可达最小时间戳。
    // st：尚未归属分量的顶点栈；comp[u]：所属强连通分量编号。
    // cc：分量数量；timer：DFS 时间戳计数器。
    vector<int> dfn, low, st, comp;
    // in_stack[u]：u 是否仍在分量栈中；已出栈顶点不能用于降低 low。
    vector<char> in_stack;
    // 初始化编号 1..n 的有向图；遍历数组在 build 中初始化。
    TarjanSCC(int n) : n(n), g(n + 1) {}
    // 添加有向边 u→v，端点需在 1..n；均摊 O(1)。
    void add_edge(int u, int v) {
        g[u].push_back(v);
    }
    // 内部 Tarjan 遍历：访问尚未访问的 u，维护 dfn、low 和栈，并弹出完整强连通分量。
    // 由 build 初始化状态后调用，可能递归至 O(n) 深度。
    void dfs(int u) {
        dfn[u] = low[u] = ++timer;
        st.push_back(u);
        in_stack[u] = 1;
        for (int v : g[u]) {
            if (!dfn[v]) {
                dfs(v);
                low[u] = min(low[u], low[v]);
            }
            else if (in_stack[v]) {
                // 只有仍在分量栈中的顶点才能用于降低 low[u]，避免合并已完成的分量。
                low[u] = min(low[u], dfn[v]);
            }
        }
        if (low[u] == dfn[u]) {
            while (true) {
                int v = st.back();
                st.pop_back();
                in_stack[v] = 0;
                comp[v] = cc;
                if (v == u) break;
            }
            ++cc;
        }
    }
    // 清空遍历状态并重算全部强连通分量，写入 comp[1..n] 与数量 cc。
    // 编号为 0..cc-1，跨分量边 u→v 满足 comp[u]>comp[v]；时间 O(n+m)。
    void build() {
        cc = timer = 0;
        st.clear();
        dfn.assign(n + 1, 0);
        low.assign(n + 1, 0);
        comp.assign(n + 1, -1);
        in_stack.assign(n + 1, 0);
        for (int u = 1; u <= n; ++u) {
            if (!dfn[u]) dfs(u);
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
