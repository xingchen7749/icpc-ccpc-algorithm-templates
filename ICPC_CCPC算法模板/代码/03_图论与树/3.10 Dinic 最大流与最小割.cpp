#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

const i64 INF = (1LL << 62);

struct Dinic {
    struct E {
        // E（edge）：残量边；v：终点；rev：反向边在 g[v] 中的下标。
        int v, rev;
        // cap（capacity）：当前剩余容量，增广时会改变。
        i64 cap;
    };
    int n;
    vector<vector<E>> g;
    // level[u]：层次图中的层数，-1 表示不可达；it[u]：当前弧的邻接表下标。
    vector<int> level, it;
    // 创建 n 个顶点的空残量网络及层次、当前弧数组，顶点编号为 1..n。
    Dinic(int n) : n(n), g(n + 1), level(n + 1), it(n + 1) {}
    // 添加容量为 c 的有向边 u→v 和容量为 0 的反向残量边；c>=0，自环直接忽略。
    void add_edge(int u, int v, i64 c) {
        if (u == v) return;
        assert(c >= 0);
        int a = g[u].size(), b = g[v].size();
        g[u].push_back({v, b, c});
        g[v].push_back({u, a, 0});
    }
    // 内部建层：沿正残量边从 s 广搜，更新 level，返回汇点 t 是否可达；时间 O(n+m)。
    // BFS（Breadth-First Search）：广度优先搜索，用于构造层次图。
    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto e : g[u]) {
                if (e.cap > 0 && level[e.v] < 0) {
                    level[e.v] = level[u] + 1;
                    q.push(e.v);
                }
            }
        }
        return level[t] >= 0;
    }
    // 在层次图中从 u 向汇点 t 尝试推送至多 f 流量，返回实际流量并更新正反残量。
    // 使用当前弧 it 跳过已尝试边；要求先 bfs 建层并初始化 it。
    // DFS（Depth-First Search）：深度优先搜索，用于沿层次图增广。
    i64 dfs(int u, int t, i64 f) {
        if (u == t) return f;
        for (int& i = it[u]; i < (int)g[u].size(); ++i) {
            E& e = g[u][i];
            if (e.cap > 0 && level[e.v] == level[u] + 1) {
                i64 z = dfs(e.v, t, min(f, e.cap));
                if (z) {
                    e.cap -= z;
                    g[e.v][e.rev].cap += z;
                    return z;
                }
            }
        }
        return 0;
    }
    // 在当前残量网络上增加 s→t 流，返回本次新增流量，最多 limit；要求 s!=t、limit>=0。
    // 会修改残量网络，重复调用只返回新增部分；未被 limit 截断时可用残量可达点求最小割。
    i64 flow(int s, int t, i64 limit = INF) {
        assert(s != t);
        i64 ans = 0;
        while (ans < limit && bfs(s, t)) {
            fill(it.begin(), it.end(), 0);
            while (i64 f = dfs(s, t, limit - ans)) {
                ans += f;
                if (ans == limit) break;
            }
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
