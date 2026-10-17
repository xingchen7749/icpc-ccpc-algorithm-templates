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

// 依赖手册 3.4.1「迭代 Kosaraju」，以上已展开其实现。
// Dependency: SCC from the previous entry.
// 2-SAT（2-Satisfiability）：每个子句至多包含两个文字的布尔可满足性问题。
struct TwoSAT {
    int n;
    SCC s;
    // value[i]：求解成功后第 i 个布尔变量的取值，0 为假、1 为真。
    vector<int> value;
    // 创建 n 个布尔变量的蕴含图，变量编号 1..n，每个变量对应两个图节点。
    TwoSAT(int n) : n(n), s(2 * n) {}
    // 加入子句 (变量 a 等于 av) 或 (变量 b 等于 bv)，转成两条蕴含边。
    // 变量编号为 1..n；clause(x,v,x,v) 可强制 x=v，均摊 O(1)。
    void clause(int a, bool av, int b, bool bv) {
        int x = 2 * a - 1 + av, y = 2 * b - 1 + bv;
        s.add_edge((x % 2 ? x + 1 : x - 1), y);
        s.add_edge((y % 2 ? y + 1 : y - 1), x);
    }
    // 判断全部子句是否可同时满足；可满足返回 true，并把一组赋值写入 value[1..n]。
    // 返回 false 时不能使用 value 作为解；此处赋值比较配合 Kosaraju 编号，时间 O(n+m)。
    bool solve() {
        s.build();
        value.resize(n + 1);
        for (int i = 1; i <= n; ++i) {
            if (s.comp[2 * i - 1] == s.comp[2 * i]) return false;
            value[i] = s.comp[2 * i - 1] < s.comp[2 * i];
        }
        return true;
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
