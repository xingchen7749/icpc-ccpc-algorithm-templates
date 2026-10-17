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

// 依赖手册 5.2.2「扩展欧几里得求逆元」，以上已展开其实现。
// 依赖 exgcd：见 5.2.2「扩展欧几里得求逆元」（代码/05_数学/5.2.2 扩展欧几里得求逆元.cpp）。
// CRT（Chinese Remainder Theorem）：中国剩余定理，此处支持非互质模数的扩展形式。
struct CRTResult {
    // status：0 无解、1 成功、2 合并模数溢出；只有成功时才使用 r、m。
    int status;
    // r：规范化余数；m：合并后的模数，成功时解为 x ≡ r (mod m)。
    i64 r, m;
};
// 合并 x≡a(mod m) 与 x≡b(mod n)，要求 m,n>0，允许模数不互质。
// 返回 {status,r,m}：0 表示无解，1 表示成功，2 表示合并模数超出 i64。
// 成功时解为 x≡r(mod 返回模数)，且 0<=r<返回模数；依赖 exgcd，耗时为对数级。
CRTResult crt_merge(i64 a, i64 m, i64 b, i64 n) {
    assert(m > 0 && n > 0);
    a %= m;
    if (a < 0) a += m;
    b %= n;
    if (b < 0) b += n;
    i128 x, y;
    i64 g = exgcd(m, n, x, y);
    i128 diff = i128(b) - a;
    if (diff % g) return {0, 0, 0};
    i128 mod = n / g;
    i128 t = (diff / g * x) % mod;
    if (t < 0) t += mod;
    i128 lcm = i128(m / g) * n;
    if (lcm > LLONG_MAX) return {2, 0, 0};
    i128 r = (i128(a) + i128(m) * t) % lcm;
    return {1, (i64)r, (i64)lcm};
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
