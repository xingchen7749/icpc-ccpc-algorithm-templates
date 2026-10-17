#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

// 返回 a[1..n] 每个长度为 k 的窗口最小值，ans[j] 对应 [j,j+k-1]。
// 输入和输出的 0 号项均占位，要求 1<=k<=n；时间 O(n)，队列空间 O(k)。
vector<i64> window_min(const vector<i64>& a, int k) {
    assert(1 <= k && k < (int)a.size());
    deque<int> q;
    vector<i64> ans(1);
    for (int i = 1; i < (int)a.size(); ++i) {
        while (!q.empty() && q.front() <= i - k) {
            q.pop_front();
        }
        while (!q.empty() && a[q.back()] >= a[i]) {
            q.pop_back();
        }
        q.push_back(i);
        if (i >= k) ans.push_back(a[q.front()]);
    }
    return ans;
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
