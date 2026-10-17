#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

// 判断正常 Nim 先手是否必胜：a[1..n] 为各堆石子数，忽略 a[0]。
// 所有堆大小异或非零返回 true，空局面返回 false；时间 O(n)。
bool nim_win(const vector<u64>& a) {
    u64 x = 0;
    for (int i = 1; i < (int)a.size(); ++i) {
        x ^= a[i];
    }
    return x != 0;
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
