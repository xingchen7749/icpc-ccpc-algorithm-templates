#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

// 判断取最后一颗石子者输的反 Nim 先手胜负，堆数组为 a[1..n]。
// 存在大于 1 的堆时看异或和，否则看 1 石子堆是否为偶数；本实现空局面返回 true。
bool misere_nim_win(const vector<u64>& a) {
    u64 x = 0;
    int ones = 0;
    bool big = false;
    for (int i = 1; i < (int)a.size(); ++i) {
        u64 v = a[i];
        x ^= v;
        ones += v == 1;
        big |= v > 1;
    }
    return big ? x != 0 : ones % 2 == 0;
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
