#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

// DP（Dynamic Programming）：动态规划；李超树可维护直线形式的转移最小值。
struct LiChao {
    struct Line {
        i64 k = 0, b = 0;
        // 计算当前直线在 x 处的 y=k*x+b，使用 i128 中间量；有效值绝对值须小于 2^126。
        i128 eval(i64 x) const { return i128(k) * x + b; }
    };
    const i128 INF = i128(1) << 126;
    // xs：排序去重后的所有查询横坐标，下标从 0 开始。
    vector<i64> xs;
    // t[p]：线段树节点保存的候选直线；has[p] 表示该节点是否已有直线。
    vector<Line> t;
    vector<char> has;
    // 收集全部将被查询的横坐标 x[1..m]，忽略 x[0]，排序去重后建立空李超树。
    // 至少需要一个有效坐标；初始化时间 O(m log m)，存储 O(m)。
    LiChao(vector<i64> x) : xs(move(x)) {
        assert(!xs.empty());
        xs.erase(xs.begin());
        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(), xs.end()), xs.end());
        assert(!xs.empty());
        t.resize(xs.size() * 4);
        has.resize(xs.size() * 4);
    }
    // 向李超树插入整条直线 v={k,b}，维护已收集坐标处的最小值；时间 O(log m)。
    void add(Line v) {
        add(1, 0, xs.size(), v);
    }
    // 内部递归插入：节点 p 覆盖压缩坐标下标区间 [l,r)，其中下标从 0 开始。
    // 保留在中点更优的直线，再递归处理另一条线可能更优的一侧。
    void add(int p, int l, int r, Line v) {
        if (!has[p]) {
            has[p] = 1;
            t[p] = v;
            return;
        }
        int m = (l + r) / 2;
        bool left = v.eval(xs[l]) < t[p].eval(xs[l]);
        bool mid = v.eval(xs[m]) < t[p].eval(xs[m]);
        if (mid) swap(t[p], v);
        if (r - l == 1) return;
        if (left != mid) {
            add(p * 2, l, m, v);
        }
        else {
            add(p * 2 + 1, m, r, v);
        }
    }
    // 返回所有已插入直线在横坐标 x 处的最小值；x 必须在构造时收集的坐标中。
    // 尚未插入直线时返回 INF，时间 O(log m)，不修改树。
    i128 query(i64 x) {
        int idx = lower_bound(xs.begin(), xs.end(), x) - xs.begin();
        assert(idx < (int)xs.size() && xs[idx] == x);
        int p = 1, l = 0, r = xs.size();
        i128 ans = INF;
        while (true) {
            if (has[p]) {
                ans = min(ans, t[p].eval(x));
            }
            if (r - l == 1) break;
            int m = (l + r) / 2;
            if (idx < m) {
                p *= 2;
                r = m;
            }
            else {
                p = p * 2 + 1;
                l = m;
            }
        }
        return ans;
    }
};

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
