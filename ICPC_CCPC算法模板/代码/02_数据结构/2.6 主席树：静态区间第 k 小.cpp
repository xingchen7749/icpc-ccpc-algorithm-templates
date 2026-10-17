#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

struct PersistentKth {
    struct Node {
        // l、r：左右儿子的节点编号，0 表示空节点；cnt：该值域内的元素数量。
        int l = 0, r = 0, cnt = 0;
    };
    int m;
    // t：可持久化节点池，0 号为空节点。
    vector<Node> t{{}};
    // root[i]：插入前 i 个元素后的版本根；root[0] 表示空前缀。
    vector<int> root{0};
    // 设置离散值域为 1..m，创建空前缀版本 root[0]，要求 m>0。
    PersistentKth(int m) : m(m) {
        assert(m > 0);
    }
    // 内部持久化插入：复制 old 版本在值域 [l,r) 上通往秩 x 的路径，频数加一。
    // 返回新根编号，保留旧版本；要求 l<=x<r，新增节点数和时间均为 O(log m)。
    int insert(int old, int l, int r, int x) {
        int p = t.size();
        t.push_back(t[old]);
        ++t[p].cnt;
        if (r - l > 1) {
            int mid = (l + r) / 2;
            if (x < mid) {
                t[p].l = insert(t[old].l, l, mid, x);
            }
            else {
                t[p].r = insert(t[old].r, mid, r, x);
            }
        }
        return p;
    }
    // 向序列末尾追加离散秩 x，生成新的前缀频数版本；要求 1<=x<=m，O(log m)。
    void append(int x) {
        root.push_back(insert(root.back(), 1, m + 1, x));
    }
    // 返回原序列闭区间 [l,r] 的第 k 小离散秩，需再查离散值表取得实际值。
    // 要求 1<=l<=r<=已追加数量、1<=k<=r-l+1；时间 O(log m)。
    int kth(int l, int r, int k) {
        assert(1 <= k && k <= r - l + 1);
        int a = root[l - 1], b = root[r], L = 1, R = m + 1;
        while (R - L > 1) {
            int mid = (L + R) / 2, c = t[t[b].l].cnt - t[t[a].l].cnt;
            if (k <= c) {
                a = t[a].l;
                b = t[b].l;
                R = mid;
            }
            else {
                k -= c;
                a = t[a].r;
                b = t[b].r;
                L = mid;
            }
        }
        return L;
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
