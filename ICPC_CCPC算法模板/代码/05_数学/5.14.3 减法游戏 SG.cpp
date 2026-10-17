#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

// 计算减法游戏状态 0..N 的 SG 值；moves 是每步可取走数量的无占位列表。
// 要求 N>=0、每个动作 d>0；返回 sg[0]=0 的数组，时间 O(N*(|moves|+1))。
// SG（Sprague–Grundy）：斯普拉格–格兰迪函数。
// mex（minimum excluded value）：集合中未出现的最小非负整数。
vector<int> subtraction_sg(int N, const vector<int>& moves) {
    int K = moves.size();
    // sg[x]：剩余 x 时的 SG 值；seen[v]：SG 值 v 最近在哪个状态的后继中出现。
    vector<int> sg(N + 1), seen(K + 1, -1);
    for (int x = 1; x <= N; ++x) {
        for (int d : moves) {
            assert(d > 0);
            if (d <= x && sg[x - d] <= K) {
                seen[sg[x - d]] = x;
            }
        }
        int g = 0;
        while (g <= K && seen[g] == x) {
            ++g;
        }
        sg[x] = g;
    }
    return sg;
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
