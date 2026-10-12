#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
// #define int long long

struct SuffixArray {
    vector<int> sa, rk, lcp;
    // 构造字符串 s 的后缀排序 sa、逆排名 rk 和相邻后缀公共前缀长度 lcp。
    // 均用 0 下标，lcp[0]=0，支持空串；比较排序倍增耗时 O(n log²n)，空间 O(n)。
    SuffixArray(const string& s) {
        int n = s.size();
        sa.resize(n);
        rk.resize(n);
        lcp.assign(n, 0);
        iota(sa.begin(), sa.end(), 0);
        for (int i = 0; i < n; ++i) {
            rk[i] = (unsigned char)s[i];
        }
        vector<int> tmp(n);
        for (int k = 1; k < n; k *= 2) {
            // 读取当前轮位置 i 的排名，超过字符串末尾时返回 -1，作为缺失后半段的最小排名。
            auto val = [&](int i) { return i < n ? rk[i] : -1; };
            // 比较后缀 a、b 的前后两段排名，每段长 k；供当前倍增轮排序与判断新排名。
            auto cmp = [&](int a, int b) {
                return pair{rk[a], val(a + k)} < pair{rk[b], val(b + k)};
            };
            sort(sa.begin(), sa.end(), cmp);
            tmp[sa[0]] = 0;
            for (int i = 1; i < n; ++i) {
                tmp[sa[i]] = tmp[sa[i - 1]] + cmp(sa[i - 1], sa[i]);
            }
            rk = tmp;
            if (rk[sa.back()] == n - 1) break;
        }
        for (int i = 0; i < n; ++i) {
            rk[sa[i]] = i;
        }
        for (int i = 0, h = 0; i < n; ++i) {
            if (rk[i] == 0) {
                h = 0;
                continue;
            }
            int j = sa[rk[i] - 1];
            while (i + h < n && j + h < n && s[i + h] == s[j + h]) {
                ++h;
            }
            lcp[rk[i]] = h;
            if (h) {
                --h;
            }
        }
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
