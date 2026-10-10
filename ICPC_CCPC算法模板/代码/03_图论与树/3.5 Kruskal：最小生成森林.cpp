#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
// #define int long long

struct Edge{int u,v;i64 w;};
// 求 n 个顶点的最小生成森林，es 为无向边列表，可有负权、重边、自环。
// 返回 {总权值,选中边数}；选中 n-1 条边才表示整图连通，权值和须在 i64 范围内。
// 顶点为 1..n，不修改调用者的 es；时间 O(m log m)。
pair<i64,int> kruskal(int n,vector<Edge> es){
    vector<int>p(n+1),sz(n+1,1);iota(p.begin(),p.end(),0);
    // 内部并查集查找：返回顶点 x 的代表元，并压缩路径。
    auto find=[&](int x){
        while(x!=p[x]){p[x]=p[p[x]];x=p[x];}return x;
    };
    // 按边权从小到大比较两条边，供 Kruskal 依次尝试选边。
    sort(es.begin(),es.end(),[](Edge a,Edge b){return a.w<b.w;});
    i64 ans=0;int cnt=0;
    for(auto [u,v,w]:es){
        u=find(u);v=find(v);if(u==v)continue;
        if(sz[u]<sz[v])swap(u,v);p[v]=u;sz[u]+=sz[v];
        ans+=w;++cnt;
    }
    return {ans,cnt};
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
