#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

const i64 INF = (1LL << 62);

// 计算一层 cur[i]=min(0<=j<i){prev[j]+cost(j,i)}，返回 cur[0..n]，cur[0]=INF。
// prev 的下标是前缀长度，cost(j,i) 对应原数组 [j+1,i]；要求最优决策位置单调不减。
// 不可达 prev 用 INF，有限转移结果须在 (-INF,INF)；cost 为 O(1) 时耗时 O(n log n)。
template <class Cost>
// DP（Dynamic Programming）：动态规划；利用最优决策下标的单调性分治计算一层。
vector<i64> divide_dp_layer(const vector<i64>& prev, Cost cost) {
    int n = prev.size() - 1;
    // prev：上一层 DP 值；cur：正在计算的当前层 DP 值，INF 表示尚无有效转移。
    vector<i64> cur(n + 1, INF);
    // 内部决策分治：求 cur[l..r]，最优决策只在 [optL,optR] 中搜索。
    // 先计算中点最优决策，再收窄左右搜索范围；只有决策单调性成立时才正确。
    function<void(int, int, int, int)> go = [&](int l, int r, int optL, int optR) {
        if (l > r) return;
        int mid = (l + r) / 2, best = optL;
        for (int j = optL; j <= min(mid - 1, optR); ++j) {
            if (prev[j] == INF) continue;
            i128 v = i128(prev[j]) + cost(j, mid);
            if (v < cur[mid]) {
                cur[mid] = (i64)v;
                best = j;
            }
        }
        go(l, mid - 1, optL, best);
        go(mid + 1, r, best, optR);
    };
    if (n) go(1, n, 0, n - 1);
    return cur;
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
