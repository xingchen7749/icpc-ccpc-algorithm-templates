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

// 依赖手册 2.1「并查集」，以上已展开其实现。
struct Edge {
    int u, v;
    i64 w;
};
// 使用 2.1 并查集的 init、find、merge；独立文件中已展开依赖。
// 求 n 个顶点的最小生成森林，es 为无向边列表，可有负权、重边、自环。
// 返回 {总权值,选中边数}；选中 n-1 条边才表示整图连通，权值和须在 i64 范围内。
// 要求 n>=1，顶点为 1..n；不修改调用者的 es，会重置全局并查集。
// 时间 O(n + m log(m+1))，空间 O(n+m)。
pair<i64, int> kruskal(int n, vector<Edge> es) {
    init(n);
    // 按边权从小到大比较两条边，供 Kruskal 依次尝试选边。
    sort(es.begin(), es.end(), [](const Edge& a, const Edge& b) { return a.w < b.w; });
    int num = n;
    i64 result = 0;
    for (int i = 0; i < (int)es.size(); ++i) {
        if (!merge(es[i].u, es[i].v)) continue;
        --num;
        result += es[i].w;
        if (num == 1) break;
    }
    return {result, n - num};
}


void solve() {
    int n, m;
    cin >> n >> m;
    vector<Edge> arr(m);
    for (int i = 0; i < m; ++i) {
        cin >> arr[i].u >> arr[i].v >> arr[i].w;
    }
    auto [result, used] = kruskal(n, arr);
    if (used != n - 1) {
        cout << -1 << '\n';
    }
    else {
        cout << result << '\n';
    }
}


signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T = 1;
    cin >> T; // Remove for a single test case.
    while (T--) solve();
}
