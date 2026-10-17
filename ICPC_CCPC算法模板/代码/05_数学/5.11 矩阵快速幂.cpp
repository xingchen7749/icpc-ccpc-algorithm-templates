#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

using Matrix = vector<vector<i64>>;
// 返回同阶方阵 a*b 对 mod 取模的结果；行列为 1..n，0 行/列占位。
// 要求 mod>0、元素在 [0,mod)，输入尺寸一致；不修改输入，时间 O(n³)，输出空间 O(n²)。
Matrix multiply(const Matrix& a, const Matrix& b, i64 mod) {
    int n = (int)a.size() - 1;
    Matrix c(n + 1, vector<i64>(n + 1));
    for (int i = 1; i <= n; ++i) {
        for (int k = 1; k <= n; ++k) {
            if (a[i][k]) {
                for (int j = 1; j <= n; ++j) {
                    c[i][j] = (i128(c[i][j]) + i128(a[i][k]) * b[k][j]) % mod;
                }
            }
        }
    }
    return c;
}
// 返回方阵 a 的非负整数 e 次幂模 mod；行列使用 1 下标，元素需在 [0,mod)。
// 要求 mod>0；e=0 返回模意义的单位矩阵，时间 O(n³log(e+1))，空间 O(n²)。
Matrix matrix_power(Matrix a, u64 e, i64 mod) {
    assert(mod > 0);
    int n = (int)a.size() - 1;
    Matrix r(n + 1, vector<i64>(n + 1));
    for (int i = 1; i <= n; ++i) r[i][i] = 1 % mod;
    for (; e; e >>= 1, a = multiply(a, a, mod)) {
        if (e & 1) {
            r = multiply(r, a, mod);
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
