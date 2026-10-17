#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

// 在开区间 (l,r) 中寻找第一个使 ok(x) 为真的值，不存在则返回初始 r。
// l 为假边界，r 为真边界；两端可为虚拟哨兵，不会调用 ok。
// 要求 l<r、r-l 不溢出，条件先假后真；判定次数为 O(log(r-l))。
// 对 arr[1..n]，传入 l=0、r=n+1；返回 n+1 表示不存在。
template <class Pred>
i64 first_true(i64 l, i64 r, Pred ok) {
    while (l + 1 < r) {
        i64 mid = l + (r - l) / 2;
        if (ok(mid)) {
            r = mid;
        }
        else {
            l = mid;
        }
    }
    return r;
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
