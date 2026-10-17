#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

class SegTree {
private:
    int n;
    vector<int> tree; // 区间和

    // 合并两个区间的结果：参数 a、b 为各自区间和，返回合并后的和
    int merge(int a, int b) { return a + b; }

    // 向上合并：通过 merge 用两个子节点的结果更新节点 p
    void pushup(int p) {
        tree[p] = merge(tree[p * 2], tree[p * 2 + 1]);
    }

    // 建树：节点 p 管理闭区间 [l,r]
    void build(const vector<int>& a, int p, int l, int r) {
        if (l == r) {
            tree[p] = a[l];
            return;
        }
        int m = (l + r) / 2;
        build(a, p * 2, l, m);
        build(a, p * 2 + 1, m + 1, r);
        pushup(p);
    }

    // 单点赋值的递归实现：将 a[pos] 改为 v
    void set(int p, int l, int r, int pos, int v) {
        if (l == r) {
            tree[p] = v;
            return;
        }
        int m = (l + r) / 2;
        if (pos <= m) {
            set(p * 2, l, m, pos, v);
        }
        else {
            set(p * 2 + 1, m + 1, r, pos, v);
        }
        pushup(p);
    }

    // 单点加的递归实现：将 a[pos] 增加 v，再向上更新区间和
    void add(int p, int l, int r, int pos, int v) {
        if (l == r) {
            tree[p] += v;
            return;
        }
        int m = (l + r) / 2;
        if (pos <= m) {
            add(p * 2, l, m, pos, v);
        }
        else {
            add(p * 2 + 1, m + 1, r, pos, v);
        }
        pushup(p);
    }

    // 区间求和的递归实现：完全覆盖直接返回，否则累加子区间答案
    int query(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return tree[p];
        int m = (l + r) / 2;
        int sum = 0;
        if (ql <= m) {
            sum = merge(sum, query(p * 2, l, m, ql, qr));
        }
        if (qr > m) {
            sum = merge(sum, query(p * 2 + 1, m + 1, r, ql, qr));
        }
        return sum;
    }

public:
    // 构造函数：a[0] 占位，要求 n >= 1，建树 O(n)
    SegTree(const vector<int>& a) {
        assert(a.size() >= 2); // 至少包含占位和一个有效元素
        n = (int)a.size() - 1;
        tree.assign(4 * n + 5, 0);
        build(a, 1, 1, n);
    }

    // 单点赋值：a[pos] = v，要求 1 <= pos <= n，O(log n)
    void set(int pos, int v) {
        assert(1 <= pos && pos <= n);
        set(1, 1, n, pos, v);
    }

    // 单点加：a[pos] += v，要求 1 <= pos <= n，O(log n)
    void add(int pos, int v) {
        assert(1 <= pos && pos <= n);
        add(1, 1, n, pos, v);
    }

    // 区间求和：返回 [l,r] 的和，要求 1 <= l <= r <= n，O(log n)
    int query(int l, int r) {
        assert(1 <= l && l <= r && r <= n);
        return query(1, 1, n, l, r);
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
