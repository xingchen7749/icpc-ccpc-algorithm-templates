#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

// Kuhn 算法（以 Harold W. Kuhn 命名）：国内竞赛中通常称为匈牙利算法。
// 使用 DFS（Depth-First Search，深度优先搜索）寻找增广路，求无权二分图最大匹配。
struct BipartiteMatch {
    int nL, nR, timer = 0;
    vector<vector<int>> g;
    // mr[v]：右侧顶点 v 匹配的左侧顶点，-1 表示未匹配。
    // seen[v]：右侧顶点最近被搜索的轮次；timer：当前搜索轮次。
    // nL、nR：左右两侧的顶点数量，各自从 1 开始编号。
    vector<int> mr, seen;
    // 初始化左部 l 个点、右部 r 个点的二分图，两侧各从 1 编号；mr 初值为 -1。
    BipartiteMatch(int l, int r) : nL(l), nR(r), g(l + 1), mr(r + 1, -1), seen(r + 1) {}
    // 内部增广搜索：从左点 u 尝试找到可用右点或重排旧匹配，成功返回 true 并更新 mr。
    // 使用当前 timer 防止本轮重复访问右点，由 solve 为每轮设置新时间戳。
    bool dfs(int u) {
        for (int v : g[u]) {
            if (seen[v] != timer) {
                seen[v] = timer;
                if (mr[v] == -1 || dfs(mr[v])) {
                    mr[v] = u;
                    return true;
                }
            }
        }
        return false;
    }
    // 重置并求最大匹配，返回匹配边数，mr[v] 保存右点 v 匹配的左点，-1 表示未匹配。
    // 调用前把左到右的边放入 g；时间 O(nL*m)。
    int solve() {
        fill(mr.begin(), mr.end(), -1);
        int ans = 0;
        for (int u = 1; u <= nL; ++u) {
            ++timer;
            ans += dfs(u);
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
