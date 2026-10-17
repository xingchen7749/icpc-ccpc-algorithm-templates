#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

// DSU（Disjoint Set Union）：并查集。
// father[x]：父节点；根节点的父节点是自身。
// rk（rank）：合并使用的秩，不是集合大小；sz[root]：根对应的集合大小。
vector<int> father, rk, sz;

// 初始化编号 1..n 的独立集合，初始秩和集合大小均为 1；时间和空间 O(n)。
void init(int n) {
    father.resize(n + 1);
    rk.assign(n + 1, 1);
    sz.assign(n + 1, 1);
    for (int i = 1; i <= n; ++i) father[i] = i;
}

// 返回 id 所在集合的根，并通过递归压缩路径；要求 1<=id<=n。
int find(int id) {
    if (id != father[id]) father[id] = find(father[id]);
    return father[id];
}

// 按秩合并 u、v 所在集合并更新集合大小；成功返回 true，原本同集合返回 false。
// 要求顶点为 1..n；配合路径压缩，均摊时间 O(α(n))。
bool merge(int u, int v) {
    int a = find(u), b = find(v);
    if (a == b) return false;
    if (rk[a] > rk[b]) {
        father[b] = a;
        sz[a] += sz[b];
    }
    else if (rk[b] > rk[a]) {
        father[a] = b;
        sz[b] += sz[a];
    }
    else {
        father[b] = a;
        rk[a]++;
        sz[a] += sz[b];
    }
    return true;
}

// 返回 id 所在集合的元素数量；要求 1<=id<=n，均摊时间 O(α(n))。
int size(int id) {
    return sz[find(id)];
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
