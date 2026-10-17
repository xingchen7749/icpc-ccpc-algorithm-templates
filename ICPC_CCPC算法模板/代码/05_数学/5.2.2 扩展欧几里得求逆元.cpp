#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

// 将整数 a 规范为模 m 的非负余数，返回 [0,m-1] 内的值；要求 m>0，时间 O(1)。
i64 norm(i64 a, i64 m) {
    a %= m;
    return a < 0 ? a + m : a;
}
// 扩展欧几里得：返回 gcd(a,b)，并通过引用写入满足 a*x+b*y=gcd(a,b) 的系数。
// 要求 a、b 非负；系数用 i128 保存，递归耗时与空间为对数级。
i64 exgcd(i64 a, i64 b, i128& x, i128& y) {
    if (!b) {
        x = 1;
        y = 0;
        return a;
    }
    i128 u, v;
    i64 g = exgcd(b, a % b, u, v);
    x = v;
    y = u - i128(a / b) * v;
    return g;
}
// 返回 a 在模 m 下的逆元，要求 m>1，支持合数模数和负 a。
// 若 gcd(a,m)!=1 则返回 nullopt，否则返回 [0,m-1] 内的逆元；时间 O(log m)。
optional<i64> inverse(i64 a, i64 m) {
    assert(m > 1);
    i128 x, y;
    if (exgcd(norm(a, m), m, x, y) != 1) return nullopt;
    x %= m;
    if (x < 0) x += m;
    return (i64)x;
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
