#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

struct StringHash {
    static constexpr i64 M1 = 1000000007, M2 = 1000000009, B = 911382323;
    // pre1[i]、pre2[i]：前 i 个字符在两组模数下的哈希，初值均为 0。
    // p1[i]、p2[i]：对应模数下底数的 i 次幂。
    vector<i64> pre1, pre2, p1, p2;
    // 预处理字符串 s 的双模前缀哈希和幂表，字符串位置为 0 下标；时间和空间 O(|s|)。
    StringHash(const string& s)
        : pre1(s.size() + 1), pre2(s.size() + 1), p1(s.size() + 1, 1), p2(s.size() + 1, 1) {
        for (int i = 0; i < (int)s.size(); ++i) {
            int x = (unsigned char)s[i] + 1;
            pre1[i + 1] = (pre1[i] * B + x) % M1;
            pre2[i + 1] = (pre2[i] * B + x) % M2;
            p1[i + 1] = p1[i] * B % M1;
            p2[i + 1] = p2[i] * B % M2;
        }
    }
    // 返回子串 s[l..r) 的两个模数下哈希，要求 0<=l<=r<=|s|；时间 O(1)。
    // 比较子串时需同时比较长度；即使双哈希相等，也仍存在碰撞可能。
    pair<i64, i64> get(int l, int r) const {
        return {(pre1[r] - pre1[l] * p1[r - l] % M1 + M1) % M1,
                (pre2[r] - pre2[l] * p2[r - l] % M2 + M2) % M2};
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
