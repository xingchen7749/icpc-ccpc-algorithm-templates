#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

// HLD（Heavy-Light Decomposition）：树链剖分。
// DFS（Depth-First Search）：深度优先搜索；DFS 序使子树对应连续区间。
struct HLD {
    int n;
    vector<vector<int>> g;
    // father[u]：父节点，根的父节点是自身；dep（depth）：深度，根为 0。
    // sz（size）：子树大小，包含自身；heavy[u]：重儿子，-1 表示没有。
    // head[u]：所在重链的链头；pos[u]：从 1 开始的 DFS 序编号。
    // rev[i]：DFS 序编号为 i 的顶点，rev[pos[u]] = u。
    vector<int> father, dep, sz, heavy, head, pos, rev;
    // 以 root 为根对非空连通无向树 g[1..n] 做迭代树链剖分，构造父亲、深度和 dfs 序。
    // pos/rev 使用 1 下标，子树区间为 [pos[u],pos[u]+sz[u]-1]；时间和空间 O(n)。
    HLD(vector<vector<int>> g, int root = 1)
        : n((int)g.size() - 1), g(move(g)), father(n + 1, -1), dep(n + 1), sz(n + 1, 1),
          heavy(n + 1, -1), head(n + 1), pos(n + 1), rev(n + 1) {
        // ord（order）：父节点先于子节点的遍历顺序，逆序处理即可汇总子树大小。
        vector<int> ord{root};
        father[root] = root;
        for (int i = 0; i < n; ++i) {
            int u = ord[i];
            for (int v : this->g[u]) {
                if (v != father[u]) {
                    father[v] = u;
                    dep[v] = dep[u] + 1;
                    ord.push_back(v);
                }
            }
        }
        for (int i = n - 1; i > 0; --i) {
            int u = ord[i], p = father[u];
            sz[p] += sz[u];
            if (heavy[p] == -1 || sz[u] > sz[heavy[p]]) {
                heavy[p] = u;
            }
        }
        int timer = 1;
        vector<pair<int, int>> st{{root, root}};
        while (!st.empty()) {
            auto [u, h] = st.back();
            st.pop_back();
            for (; u != -1; u = heavy[u]) {
                head[u] = h;
                pos[u] = timer;
                rev[timer++] = u;
                for (int v : this->g[u]) {
                    if (v != father[u] && v != heavy[u]) {
                        st.push_back({v, v});
                    }
                }
            }
        }
    }
    // 返回树上顶点 u、v 的最近公共祖先，要求顶点有效且已完成构造；时间 O(log n)。
    // LCA（Lowest Common Ancestor）：最近公共祖先。
    int lca(int u, int v) {
        while (head[u] != head[v]) {
            if (dep[head[u]] < dep[head[v]]) swap(u, v);
            u = father[head[u]];
        }
        return dep[u] < dep[v] ? u : v;
    }
    // 将 u 到 v 的点路径拆成若干 dfs 序闭区间，逐段调用 f(l,r)，包含两端与 LCA。
    // 共 O(log n) 段，但段间顺序和方向无保证；适合求和等可交换运算。
    template <class F>
    void path(int u, int v, F f) {
        while (head[u] != head[v]) {
            if (dep[head[u]] < dep[head[v]]) swap(u, v);
            f(pos[head[u]], pos[u]);
            u = father[head[u]];
        }
        if (pos[u] > pos[v]) swap(u, v);
        f(pos[u], pos[v]);
    }
};

// 依赖手册 3.7「树链剖分、LCA 与子树区间」，以上已展开其实现。
// 依赖手册 3.7 的 HLD（树链剖分、LCA 与子树区间），独立文件已展开。
class TreePointDiff {
private:
    HLD h;
    vector<i64> diff;

public:
    // 为非空连通无向树 g[1..n] 初始化点差分，根为 root，初始增量全为 0。
    // 要求 1<=root<=n；使用迭代预处理，时间和空间 O(n)。
    TreePointDiff(const vector<vector<int>>& g, int root = 1) : h(g, root), diff(g.size(), 0) {}

    // 将 u 到 v 路径上的所有点加 val，包含两端；时间 O(log n)。
    void add(int u, int v, i64 val) {
        assert(1 <= u && u <= h.n && 1 <= v && v <= h.n);
        int p = h.lca(u, v);
        diff[u] += val;
        diff[v] += val;
        diff[p] -= val;
        if (h.father[p] != p) {
            diff[h.father[p]] -= val;
        }
    }

    // 返回各点累计增量 ans[1..n]，ans[0]=0；时间和空间 O(n)。
    // 按 dfs 序逆序向父亲累加，不修改 diff，可重复调用并继续添加操作。
    vector<i64> result() const {
        vector<i64> ans = diff;
        for (int i = h.n; i > 1; --i) {
            int u = h.rev[i];
            ans[h.father[u]] += ans[u];
        }
        return ans;
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
