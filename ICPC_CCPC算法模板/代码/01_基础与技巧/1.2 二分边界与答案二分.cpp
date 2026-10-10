#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
// #define int long long

// 在候选区间 [lo,hi) 中寻找第一个使 ok(x) 为真的值，不存在则返回原 hi。
// 要求谓词先假后真，且 hi-lo 不溢出；调用 ok 的次数为 O(log(hi-lo+1))。
template<class Pred>
i64 first_true(i64 lo, i64 hi, Pred ok) {
    while (lo < hi) {
        i64 mid = lo + (hi - lo) / 2;
        if (ok(mid)) hi = mid;
        else lo = mid + 1;
    }
    return lo;
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
