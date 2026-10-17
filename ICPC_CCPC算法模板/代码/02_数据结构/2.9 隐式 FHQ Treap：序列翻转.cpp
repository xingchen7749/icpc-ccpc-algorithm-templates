#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

// FHQ Treap：基于 split/merge 的无旋 Treap；FHQ 来自作者名 fhq。
// Treap 由 Tree（树）和 Heap（堆）合成；隐式版本用中序位置表示序列下标。
struct ImplicitTreap {
    struct Node {
        // l、r：左右儿子编号，0 表示空树；sz：以当前节点为根的子树大小。
        int l = 0, r = 0, sz = 1;
        // pri（priority）：随机优先级，维护堆性质。
        u64 pri = 0;
        i64 val = 0;
        // rev（reverse）：待下传的区间翻转标记，不是反向下标。
        bool rev = false;
    };
    // t：节点池，0 号为空节点；有效节点从 1 开始。
    vector<Node> t{{}};
    int root = 0;
    mt19937_64 rng{712367};
    // 返回节点 p 的子树大小；节点编号 0 表示空树，返回 0，时间 O(1)。
    int size(int p) { return p ? t[p].sz : 0; }
    // 内部维护：根据左右孩子重新计算非空节点 p 的子树大小，时间 O(1)。
    void pull(int p) {
        t[p].sz = 1 + size(t[p].l) + size(t[p].r);
    }
    // 为子树 p 应用翻转：交换左右孩子并切换延迟翻转标记；空树不操作，O(1)。
    void flip(int p) {
        if (p) {
            swap(t[p].l, t[p].r);
            t[p].rev ^= 1;
        }
    }
    // 将非空节点 p 的延迟翻转标记下传给孩子，并清除自身标记，时间 O(1)。
    void push(int p) {
        if (t[p].rev) {
            flip(t[p].l);
            flip(t[p].r);
            t[p].rev = 0;
        }
    }
    // 按元素个数把子树 p 分为前 k 项与剩余项，返回两棵树的根；要求 0<=k<=size(p)。
    // 会重接子树并下传标记，期望时间 O(log n)。
    pair<int, int> split(int p, int k) {
        if (!p) return {0, 0};
        push(p);
        if (size(t[p].l) >= k) {
            auto [a, b] = split(t[p].l, k);
            t[p].l = b;
            pull(p);
            return {a, p};
        }
        auto [a, b] = split(t[p].r, k - size(t[p].l) - 1);
        t[p].r = a;
        pull(p);
        return {p, b};
    }
    // 把序列树 a 接在 b 前面合并，返回新根；要求 a 的所有项排在 b 的所有项之前。
    // 会修改树的连接关系，期望时间 O(log n)。
    int merge(int a, int b) {
        if (!a || !b) return a ? a : b;
        if (t[a].pri > t[b].pri) {
            push(a);
            t[a].r = merge(t[a].r, b);
            pull(a);
            return a;
        }
        push(b);
        t[b].l = merge(a, t[b].l);
        pull(b);
        return b;
    }
    // 在当前序列末尾添加值 v，并更新 root；期望时间 O(log n)，新增一个节点。
    void append(i64 v) {
        int p = t.size();
        Node x;
        x.pri = rng();
        x.val = v;
        t.push_back(x);
        root = merge(root, p);
    }
    // 原地翻转序列的闭区间 [l,r]，要求 1<=l<=r<=序列长度；期望时间 O(log n)。
    void reverse(int l, int r) {
        auto [a, c] = split(root, r);
        auto [x, b] = split(a, l - 1);
        flip(b);
        root = merge(merge(x, b), c);
    }
    // 内部中序遍历：把子树 p 的值依次追加到 a，遍历时下传翻转标记。
    // 不清空 a；耗时与子树大小成正比，递归空间与树高成正比。
    void collect(int p, vector<i64>& a) {
        if (!p) return;
        push(p);
        collect(t[p].l, a);
        a.push_back(t[p].val);
        collect(t[p].r, a);
    }
    // 导出当前完整序列，返回 a[1..n]，a[0] 为占位；时间和输出空间 O(n)。
    vector<i64> values() {
        vector<i64> a(1);
        collect(root, a);
        return a;
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
