#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
// #define int long long

struct SCC {
    int n,cc=0;vector<vector<int>>g,rg;vector<int>comp;
    // 初始化编号 1..n 的有向图和反图；时间和空间 O(n)。
    SCC(int n):n(n),g(n+1),rg(n+1){}
    // 添加有向边 u→v，同时在反图加入 v→u；要求端点在 1..n，均摊 O(1)。
    void add_edge(int u,int v){g[u].push_back(v);rg[v].push_back(u);}
    // 用迭代 Kosaraju 重算强连通分量，将数量写入 cc、编号写入 comp[1..n]。
    // 编号为 0..cc-1，跨分量边 u→v 满足 comp[u]<comp[v]；可加边后重新调用。
    // 时间 O(n+m)，无需递归调用栈。
    void build(){
        cc=0;vector<char>vis(n+1);vector<int>order;
        for(int s=1;s<=n;++s)if(!vis[s]){
            vector<pair<int,int>>st{{s,0}};vis[s]=1;
            while(!st.empty()){
                int u=st.back().first;
                int &i=st.back().second;
                if(i==(int)g[u].size()){order.push_back(u);st.pop_back();}
                else{int v=g[u][i++];if(!vis[v]){vis[v]=1;st.push_back({v,0});}}
            }
        }
        comp.assign(n+1,-1);reverse(order.begin(),order.end());
        for(int s:order)if(comp[s]==-1){
            vector<int>st{s};comp[s]=cc;
            while(!st.empty()){
                int u=st.back();st.pop_back();
                for(int v:rg[u])if(comp[v]==-1){comp[v]=cc;st.push_back(v);}
            }
            ++cc;
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
