#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

// DSU（Disjoint Set Union）：并查集；Rollback 表示支持回滚。
struct RollbackDSU {
    // father[x]：父节点；sz[root]：根对应的集合大小。
    vector<int> father, sz;
    // hist（history）：成功合并的历史，保存被挂接的根及合并前另一集合的大小。
    vector<pair<int, int>> hist;
    // 创建编号 1..n 的独立集合并清空合并历史，时间和空间 O(n)。
    RollbackDSU(int n) : father(n + 1), sz(n + 1, 1) {
        iota(father.begin(), father.end(), 0);
    }
    // 沿父指针返回 x 的代表元，不压缩路径以便回滚；时间 O(log n)。
    int find(int x) {
        while (x != father[x]) x = father[x];
        return x;
    }
    // 返回当前成功合并历史的长度，作为之后 rollback 的快照；不修改状态，O(1)。
    int snapshot() const { return hist.size(); }
    // 按大小合并 a、b 所在集合，并记录可撤销信息；同集合返回 false 且不记历史。
    // 有效合并返回 true；时间 O(log n)。
    bool merge(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        hist.push_back({b, sz[a]});
        father[b] = a;
        sz[a] += sz[b];
        return true;
    }
    // 撤销当前历史末尾的合并，恢复到快照 s；s 必须是当前历史中的有效长度。
    // 每撤销一次成功合并耗时 O(1)，不会恢复已被丢弃的未来快照。
    void rollback(int s) {
        while ((int)hist.size() > s) {
            auto [b, old] = hist.back();
            hist.pop_back();
            sz[father[b]] = old;
            father[b] = b;
        }
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
