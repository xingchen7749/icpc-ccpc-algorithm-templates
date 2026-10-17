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


// 返回直线 p+t*v 与 q+s*w 的唯一交点；v、w 必须是非零方向向量。
// 在 EPS 意义下平行或重合时返回 nullopt，不区分这两种情况；时间 O(1)。
optional<P> line_intersection(P p, P v, P q, P w) {
    ld d = cross(v, w);
    if (fabsl(d) < EPS) return nullopt;
    return p + v * (cross(q - p, w) / d);
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
