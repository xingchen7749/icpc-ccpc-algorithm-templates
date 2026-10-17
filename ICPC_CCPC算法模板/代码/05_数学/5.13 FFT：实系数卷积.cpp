#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

using cd = complex<double>;
// 对复数数组 a 原地做 FFT；inv=false 为正变换，true 为逆变换并除以长度。
// 要求长度为正二次幂；浮点计算存在误差，时间 O(n log n)。
// FFT（Fast Fourier Transform）：快速傅里叶变换。
// cd（complex double）：复数类型别名；inv（inverse）：是否执行逆变换。
void fft(vector<cd>& a, bool inv) {
    int n = a.size();
    assert(n > 0 && (n & (n - 1)) == 0);
    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) {
            j ^= bit;
        }
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    const double PI = acos(-1.0);
    for (int len = 2; len <= n; len <<= 1) {
        cd step = polar(1.0, 2 * PI / len * (inv ? -1 : 1));
        for (int i = 0; i < n; i += len) {
            cd w = 1;
            for (int j = 0; j < len / 2; ++j) {
                cd u = a[i + j], v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= step;
            }
        }
    }
    if (inv) {
        for (cd& x : a) x /= n;
    }
}
// 返回实系数多项式 a、b 的近似卷积，系数按次数从 0 编号；空输入返回空数组。
// 输出长度 |a|+|b|-1，不修改输入；变换长度 L 下时间 O(L log L)、空间 O(L)。
// 用于整数卷积时须先验证误差再舍入，不能直接当作精确大整数卷积。
vector<double> convolution(const vector<double>& a, const vector<double>& b) {
    if (a.empty() || b.empty()) return {};
    int need = a.size() + b.size() - 1, n = 1;
    while (n < need) n <<= 1;
    vector<cd> x(n), y(n);
    copy(a.begin(), a.end(), x.begin());
    copy(b.begin(), b.end(), y.begin());
    fft(x, false);
    fft(y, false);
    for (int i = 0; i < n; ++i) {
        x[i] *= y[i];
    }
    fft(x, true);
    vector<double> ans(need);
    for (int i = 0; i < need; ++i) ans[i] = x[i].real();
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
