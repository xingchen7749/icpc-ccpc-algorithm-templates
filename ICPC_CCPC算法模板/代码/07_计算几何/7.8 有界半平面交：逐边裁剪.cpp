#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long


// ld（long double）：扩展精度浮点类型；P（point）：二维点，也用于表示向量。
using ld = long double;
// EPS（epsilon）：浮点比较容差，需结合题目坐标尺度选择。
const ld EPS = 1e-12L;
struct P {
    ld x, y;
    // 返回当前二维向量与 b 的逐坐标和，不修改原向量；时间 O(1)。
    P operator+(P b) const { return {x + b.x, y + b.y}; }
    // 返回当前二维向量减去 b 的结果，可表示从 b 指向当前点的位移；时间 O(1)。
    P operator-(P b) const { return {x - b.x, y - b.y}; }
    // 返回当前向量乘标量 k 的结果，不修改原向量；时间 O(1)。
    P operator*(ld k) const { return {x * k, y * k}; }
};
// 返回二维向量 a、b 的叉积，正负表示旋转方向；浮点结果需结合 EPS 判断，O(1)。
ld cross(P a, P b) { return a.x * b.y - a.y * b.x; }
// 返回二维向量 a、b 的点积，可用于投影和长度平方计算；时间 O(1)。
ld dot(P a, P b) { return a.x * b.x + a.y * b.y; }


// Dependency: P, cross, EPS.
// 用有向直线 a→b 裁剪凸多边形 poly，保留其左侧及边界，返回新的循环顶点列表。
// 要求 a!=b，顶点按边界顺序排列且不带占位；空结果表示在容差意义下无交集。
// 不修改输入，每次耗时 O(顶点数)；连续裁剪需从保证覆盖解集的包围框开始。
vector<P> clip_halfplane(const vector<P>& poly, P a, P b) {
    vector<P> out;
    int n = poly.size();
    P v = b - a;
    assert(dot(v, v) > 0);
    for (int i = 0; i < n; ++i) {
        P p = poly[i], q = poly[(i + 1) % n];
        // dp、dq：点 p、q 相对裁剪直线的有向叉积值，此处 dp 不是动态规划。
        ld dp = cross(v, p - a), dq = cross(v, q - a);
        // ip、iq：对应端点是否位于保留半平面内，包含容差范围内的边界。
        bool ip = dp >= -EPS, iq = dq >= -EPS;
        if (ip) out.push_back(p);
        if (ip != iq) {
            // Classification uses EPS; clamp near-boundary interpolation.
            ld t = dp / (dp - dq);
            t = max((ld)0, min((ld)1, t));
            out.push_back(p + (q - p) * t);
        }
    }
    return out;
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
