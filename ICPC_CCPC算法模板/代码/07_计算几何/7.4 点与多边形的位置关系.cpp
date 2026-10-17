#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long


struct Point {
    i64 x, y;
    // 按 (x,y) 字典序比较当前点和 b，供严格排序使用；不使用浮点容差，O(1)。
    bool operator<(Point b) const { return tie(x, y) < tie(b.x, b.y); }
    // 判断当前点与 b 的两个整数坐标是否完全相等，供点集去重使用，O(1)。
    bool operator==(Point b) const { return x == b.x && y == b.y; }
};
// 返回向量 AB 与 AC 的叉积，即三点构成三角形的两倍有向面积。
// 正值表示 a→b→c 逆时针，0 表示共线；坐标绝对值要求不超过 10^18，O(1)。
i128 cross(Point a, Point b, Point c) {
    return (i128(b.x) - a.x) * (i128(c.y) - a.y) - (i128(b.y) - a.y) * (i128(c.x) - a.x);
}
// 返回 x 的符号：负数为 -1，零为 0，正数为 1；时间 O(1)。
int sign(i128 x) { return (x > 0) - (x < 0); }
// 判断点 p 是否在线段 ab 上，包含端点，也支持 a=b 的退化线段。
// 使用精确叉积和坐标范围判断，坐标满足整数几何范围约定；时间 O(1)。
bool on_segment(Point p, Point a, Point b) {
    return cross(a, b, p) == 0 && min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x) &&
           min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y);
}


// 判断点 q 与简单多边形 p 的位置关系：0 为外部、1 为边界、2 为内部。
// p 是按边界顺序排列的无占位循环列表，顺逆时针均可；时间 O(n)。
int inside(const vector<Point>& p, Point q) {
    // wn（winding number）：绕数；点不在边界时，非零表示在多边形内部。
    int wn = 0, n = p.size();
    for (int i = 0; i < n; ++i) {
        Point a = p[i], b = p[(i + 1) % n];
        if (on_segment(q, a, b)) return 1;
        if (a.y <= q.y && b.y > q.y && cross(a, b, q) > 0) {
            ++wn;
        }
        if (a.y > q.y && b.y <= q.y && cross(a, b, q) < 0) {
            --wn;
        }
    }
    return wn ? 2 : 0;
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
