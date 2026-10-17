#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

struct Fenwick {
    int n;
    // t[i]：树状数组节点，维护原数组区间 [i-lowbit(i)+1,i] 的和。
    // lowbit(i) = i & -i，表示 i 的最低有效二进制位对应的值。
    vector<i64> t;
    // 初始化长度为 n、下标为 1..n 的全零树状数组，时间和空间 O(n)。
    Fenwick(int n) : n(n), t(n + 1) {}
    // 将第 x 项增加 v，并更新树状数组；要求 1<=x<=n，时间 O(log n)。
    void add(int x, i64 v) {
        assert(1 <= x && x <= n);
        for (; x <= n; x += x & -x) {
            t[x] += v;
        }
    }
    // 返回闭区间 [1,r] 的元素和；要求 0<=r<=n，r=0 返回 0，时间 O(log n)。
    i64 sum(int r) const {
        i64 s = 0;
        for (; r; r -= r & -r) {
            s += t[r];
        }
        return s;
    }
    // 返回闭区间 [l,r] 的元素和；非空查询要求 1<=l<=r<=n，l>r 返回 0。
    // 不修改数组，时间 O(log n)。
    i64 query(int l, int r) const { return l > r ? 0 : sum(r) - sum(l - 1); }
    // 返回最小的下标 x，使前缀和 sum(x)>=k；将单点值看作频数时即第 k 小的位置。
    // 要求所有单点频数非负且 1<=k<=sum(n)，返回 1 下标，时间 O(log n)。
    int kth(i64 k) const {
        assert(1 <= k && k <= sum(n));
        int x = 0, step = 1;
        while (step <= n / 2) step <<= 1;
        for (; step; step >>= 1) {
            int y = x + step;
            if (y <= n && t[y] < k) {
                x = y;
                k -= t[y];
            }
        }
        return x + 1;
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
