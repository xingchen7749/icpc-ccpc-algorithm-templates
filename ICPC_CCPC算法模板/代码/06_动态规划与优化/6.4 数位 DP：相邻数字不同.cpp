#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
// #define int long long

// 统计闭区间 [0,N] 中十进制相邻数字不同的整数个数，计入 0；N<0 返回 0。
// 前导零不参与比较；可用 count(R)-count(L-1) 求区间答案，时间与十进制位数成正比。
u64 count_no_equal_adjacent(i64 N) {
    if (N < 0) return 0;
    string s = to_string(N);
    u64 memo[20][11]{};
    bool vis[20][11]{};
    // 内部数位搜索：pos 为当前 0 下标位，prev 为上一位数字，10 表示尚未开始。
    // tight 表示前缀是否贴住上界；返回当前状态的合法补全数量，仅缓存不贴上界的状态。
    function<u64(int, int, bool)> dfs = [&](int pos, int prev, bool tight) -> u64 {
        if (pos == (int)s.size()) return 1;
        if (!tight && vis[pos][prev]) return memo[pos][prev];
        int lim = tight ? s[pos] - '0' : 9;
        u64 ans = 0;
        for (int d = 0; d <= lim; ++d) {
            int next = (prev == 10 && d == 0) ? 10 : d;
            if (prev != 10 && d == prev) continue;
            ans += dfs(pos + 1, next, tight && d == lim);
        }
        if (!tight) {
            vis[pos][prev] = true;
            memo[pos][prev] = ans;
        }
        return ans;
    };
    return dfs(0, 10, true);
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
