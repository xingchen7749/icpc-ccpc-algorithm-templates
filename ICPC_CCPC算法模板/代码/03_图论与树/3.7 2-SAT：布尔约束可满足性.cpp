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

struct TarjanSCC {
    int n,cc=0,timer=0;
    vector<vector<int>> g;
    vector<int> dfn,low,st,comp;
    vector<char> in_stack;
    // 初始化编号 1..n 的有向图；遍历数组在 build 中初始化。
    TarjanSCC(int n):n(n),g(n+1){}
    // 添加有向边 u→v，端点需在 1..n；均摊 O(1)。
    void add_edge(int u,int v){g[u].push_back(v);}
    // 内部 Tarjan 遍历：访问尚未访问的 u，维护 dfn、low 和栈，并弹出完整强连通分量。
    // 由 build 初始化状态后调用，可能递归至 O(n) 深度。
    void dfs(int u){
        dfn[u]=low[u]=++timer;
        st.push_back(u);in_stack[u]=1;
        for(int v:g[u]){
            if(!dfn[v]){
                dfs(v);low[u]=min(low[u],low[v]);
            }else if(in_stack[v]){
                // Only vertices still in the SCC stack can lower low[u].
                low[u]=min(low[u],dfn[v]);
            }
        }
        if(low[u]==dfn[u]){
            while(true){
                int v=st.back();st.pop_back();
                in_stack[v]=0;comp[v]=cc;
                if(v==u)break;
            }
            ++cc;
        }
    }
    // 清空遍历状态并重算全部强连通分量，写入 comp[1..n] 与数量 cc。
    // 编号为 0..cc-1，跨分量边 u→v 满足 comp[u]>comp[v]；时间 O(n+m)。
    void build(){
        cc=timer=0;st.clear();
        dfn.assign(n+1,0);low.assign(n+1,0);
        comp.assign(n+1,-1);in_stack.assign(n+1,0);
        for(int u=1;u<=n;++u)if(!dfn[u])dfs(u);
    }
};

// Dependency: SCC from the previous entry.
struct TwoSAT {
    int n;SCC s;vector<int>value;
    // 创建 n 个布尔变量的蕴含图，变量编号 1..n，每个变量对应两个图节点。
    TwoSAT(int n):n(n),s(2*n){}
    // 加入子句 (变量 a 等于 av) 或 (变量 b 等于 bv)，转成两条蕴含边。
    // 变量编号为 1..n；clause(x,v,x,v) 可强制 x=v，均摊 O(1)。
    void clause(int a,bool av,int b,bool bv){
        int x=2*a-1+av,y=2*b-1+bv;
        s.add_edge((x%2?x+1:x-1),y);
        s.add_edge((y%2?y+1:y-1),x);
    }
    // 判断全部子句是否可同时满足；可满足返回 true，并把一组赋值写入 value[1..n]。
    // 返回 false 时不能使用 value 作为解；此处赋值比较配合 Kosaraju 编号，时间 O(n+m)。
    bool solve(){
        s.build();value.resize(n+1);
        for(int i=1;i<=n;++i){
            if(s.comp[2*i-1]==s.comp[2*i])return false;
            value[i]=s.comp[2*i-1]<s.comp[2*i];
        }
        return true;
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
