#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

// 对增广矩阵 a[1..n][1..m+1] 做实数高斯消元，末列为常数项，0 行/列占位。
// 返回 {状态,x}：0 无解、1 唯一解、2 无穷多解；有解时 x[1..m] 给出自由变量取 0 的一组解。
// 要求 n>0、各行尺寸一致、eps>0；矩阵按值传入，时间 O(n*m*min(n,m))。
pair<int, vector<long double>> gauss(vector<vector<long double>> a, long double eps = 1e-12L) {
    int n = (int)a.size() - 1;
    assert(n > 0);
    int m = (int)a[1].size() - 2, row = 1;
    // where[j]：第 j 个未知数的主元所在行，0 表示没有主元，即自由变量。
    vector<int> where(m + 1, 0);
    for (int col = 1; col <= m && row <= n; ++col) {
        int sel = row;
        for (int i = row; i <= n; ++i) {
            if (fabsl(a[i][col]) > fabsl(a[sel][col])) {
                sel = i;
            }
        }
        if (fabsl(a[sel][col]) < eps) continue;
        swap(a[sel], a[row]);
        where[col] = row;
        long double d = a[row][col];
        for (int j = col; j <= m + 1; ++j) {
            a[row][j] /= d;
        }
        for (int i = 1; i <= n; ++i) {
            if (i != row) {
                long double f = a[i][col];
                for (int j = col; j <= m + 1; ++j) {
                    a[i][j] -= f * a[row][j];
                }
            }
        }
        ++row;
    }
    vector<long double> x(m + 1);
    for (int j = 1; j <= m; ++j) {
        if (where[j]) {
            x[j] = a[where[j]][m + 1];
        }
    }
    for (int i = 1; i <= n; ++i) {
        long double v = 0;
        for (int j = 1; j <= m; ++j) {
            v += a[i][j] * x[j];
        }
        if (fabsl(v - a[i][m + 1]) > eps) return {0, {}};
    }
    return {row == m + 1 ? 1 : 2, x};
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
