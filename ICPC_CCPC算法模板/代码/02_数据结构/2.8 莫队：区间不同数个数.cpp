#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

// 离线统计 ranges 中各闭区间的不同值个数，答案按查询输入顺序返回，不带占位。
// a 使用 1 下标且值在 1..m；非空查询满足 1<=l<=r<=n，不支持修改。
vector<int> mo_distinct(const vector<int>& a, int m, const vector<pair<int, int>>& ranges) {
    int n = (int)a.size() - 1, q = ranges.size(), B = max(1, (int)sqrt(n));
    // ord：查询的处理顺序；ans：按原查询编号保存答案；cnt[x]：当前区间中值 x 的频数。
    vector<int> ord(q), ans(q), cnt(m + 1);
    iota(ord.begin(), ord.end(), 0);
    // 按左端点分块，块内右端点交替升降序排序，减少窗口指针移动；x、y 是查询编号。
    sort(ord.begin(), ord.end(), [&](int x, int y) {
        auto [l, r] = ranges[x];
        auto [L, R] = ranges[y];
        if (l / B != L / B) return l / B < L / B;
        return (l / B & 1) ? r > R : r < R;
    });
    // 当前窗口为 [l,r]，初始为空；cur：窗口内不同数值的数量。
    int l = 1, r = 0, cur = 0;
    // 把位置 i 纳入当前窗口，更新其频数；该值首次出现时增加不同值计数 cur。
    auto add = [&](int i) {
        if (cnt[a[i]]++ == 0) ++cur;
    };
    // 把当前窗口内的位置 i 移出，更新其频数；该值不再出现时减少 cur。
    auto del = [&](int i) {
        if (--cnt[a[i]] == 0) --cur;
    };
    for (int id : ord) {
        auto [L, R] = ranges[id];
        while (l > L) add(--l);
        while (r < R) add(++r);
        while (l < L) del(l++);
        while (r > R) del(r--);
        ans[id] = cur;
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
