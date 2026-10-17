#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

// SCC（Strongly Connected Component）：强连通分量；此处采用 Kosaraju 算法。
// DFS（Depth-First Search）：深度优先搜索，本实现用显式栈模拟。
struct SCC {
    // cc：强连通分量数量，已分配的分量编号为 0..cc-1。
    int n, cc = 0;
    // g：原图；rg（reverse graph）：将全部有向边反转后的图。
    vector<vector<int>> g, rg;
    // comp[u]：顶点 u 所属分量的编号，-1 表示尚未分配。
    vector<int> comp;
    // 初始化编号 1..n 的有向图和反图；时间和空间 O(n)。
    SCC(int n) : n(n), g(n + 1), rg(n + 1) {}
    // 添加有向边 u→v，同时在反图加入 v→u；要求端点在 1..n，均摊 O(1)。
    void add_edge(int u, int v) {
        g[u].push_back(v);
        rg[v].push_back(u);
    }
    // 用迭代 Kosaraju 重算强连通分量，将数量写入 cc、编号写入 comp[1..n]。
    // 编号为 0..cc-1，跨分量边 u→v 满足 comp[u]<comp[v]；可加边后重新调用。
    // 时间 O(n+m)，无需递归调用栈。
    void build() {
        cc = 0;
        vector<char> vis(n + 1);
        // order：第一遍 DFS 的完成顺序，反转后用于第二遍遍历。
        vector<int> order;
        for (int s = 1; s <= n; ++s) {
            if (!vis[s]) {
                // st：模拟递归的栈；每项保存顶点和下一条待处理邻边的下标。
                vector<pair<int, int>> st{{s, 0}};
                vis[s] = 1;
                while (!st.empty()) {
                    int u = st.back().first;
                    int& i = st.back().second;
                    if (i == (int)g[u].size()) {
                        order.push_back(u);
                        st.pop_back();
                    }
                    else {
                        int v = g[u][i++];
                        if (!vis[v]) {
                            vis[v] = 1;
                            st.push_back({v, 0});
                        }
                    }
                }
            }
        }
        comp.assign(n + 1, -1);
        reverse(order.begin(), order.end());
        for (int s : order) {
            if (comp[s] == -1) {
                vector<int> st{s};
                comp[s] = cc;
                while (!st.empty()) {
                    int u = st.back();
                    st.pop_back();
                    for (int v : rg[u]) {
                        if (comp[v] == -1) {
                            comp[v] = cc;
                            st.push_back(v);
                        }
                    }
                }
                ++cc;
            }
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
