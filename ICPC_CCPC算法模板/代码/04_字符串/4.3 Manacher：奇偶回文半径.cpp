#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

// 返回 {奇回文半径 d1,偶回文半径 d2}，均为 0 下标。
// d1[i] 含中心 i，回文长度为 2*d1[i]-1；d2[i] 的中心在 i-1 与 i 之间，长度为 2*d2[i]。
// 空串返回两个空数组；时间和空间 O(|s|)。
pair<vector<int>, vector<int>> manacher(const string& s) {
    int n = s.size();
    // d1[i]：以 i 为中心的奇回文半径，包含中心。
    // d2[i]：以 i-1 与 i 之间为中心的偶回文半径，不含虚拟中心。
    vector<int> d1(n), d2(n);
    for (int i = 0, l = 0, r = -1; i < n; ++i) {
        int k = i > r ? 1 : min(d1[l + r - i], r - i + 1);
        while (i - k >= 0 && i + k < n && s[i - k] == s[i + k]) {
            ++k;
        }
        d1[i] = k--;
        if (i + k > r) {
            l = i - k;
            r = i + k;
        }
    }
    for (int i = 0, l = 0, r = -1; i < n; ++i) {
        int k = i > r ? 0 : min(d2[l + r - i + 1], r - i + 1);
        while (i - k - 1 >= 0 && i + k < n && s[i - k - 1] == s[i + k]) {
            ++k;
        }
        d2[i] = k--;
        if (i + k > r) {
            l = i - k - 1;
            r = i + k;
        }
    }
    return {d1, d2};
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
