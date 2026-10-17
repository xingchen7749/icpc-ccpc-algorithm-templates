#include "algorithm_adapters.hpp"

const i64 INF = (1LL << 62); // Test oracle only.
template<class T> vector<T> one_based(vector<T> a){a.insert(a.begin(),T{});return a;}
vector<vector<int>> graph_one_based(const vector<vector<int>>&g){
    vector<vector<int>> out(g.size()+1);
    for(int u=0;u<(int)g.size();++u)for(int v:g[u])out[u+1].push_back(v+1);
    return out;
}

static i64 checks=0;
#define CHECK(...) do { ++checks; if(!(__VA_ARGS__)) { \
    cerr << "FAIL at line " << __LINE__ << ": " << #__VA_ARGS__ << '\n'; exit(1); \
} } while(0)
mt19937 rng(20261006);
int rnd(int l,int r){return uniform_int_distribution<int>(l,r)(rng);}

void basics(){
    for(int n=0;n<100;++n)for(int k=1;k<=n+1;++k)
        CHECK(cp_binary::first_true(0,n+1,[&](i64 x){CHECK(1<=x && x<=n);return x>=k;})==k);
    CHECK(cp_compress::compress(vector<int>{999,3,1,3,2})==vector<int>({0,1,2,3}));
    CHECK(cp_compress::rank_of(vector<int>{0,1,2,3},2)==2);
    CHECK(cp_prefix::prefix_sum({999,1,2,3})==vector<i64>({0,1,3,6}));
    vector<i64>d(6);cp_difference::range_add(d,2,4,3);
    CHECK(cp_difference::restore(d)==vector<i64>({0,0,3,3,3}));
    CHECK(cp_difference::restore(cp_difference::difference({999}))==vector<i64>({0}));
    for(int n=1;n<=30;++n){
        vector<i64>a(n+1,999);
        for(int i=1;i<=n;++i)a[i]=rnd(-100,100);
        auto diff=cp_difference::difference(a);a[0]=0;
        CHECK(cp_difference::restore(diff)==a);
        for(int t=0;t<100;++t){
            int l=t==0?1:rnd(1,n),r=t==0?n:rnd(l,n);i64 v=rnd(-100,100);
            cp_difference::range_add(diff,l,r,v);
            for(int i=l;i<=r;++i)a[i]+=v;
            CHECK(cp_difference::restore(diff)==a);
        }
        cp_difference::range_add(diff,2,1,7);
        CHECK(cp_difference::restore(diff)==a);
    }
    for(int z=0;z<100;++z){int n=rnd(1,30),k=rnd(1,n);vector<i64>a(n+1,999);
        for(int i=1;i<=n;++i)a[i]=rnd(-20,20);auto ans=cp_window::window_min(a,k);
        CHECK(ans.size()==size_t(n-k+2));
        for(int i=1;i+k-1<=n;++i)CHECK(ans[i]==*min_element(a.begin()+i,a.begin()+i+k));
    }
    unordered_map<i64,int,cp_hash::CustomHash>mp;
    for(int i=-100;i<=100;++i)mp[i]=i*i;
    for(int i=-100;i<=100;++i)CHECK(mp[i]==i*i);
}

void structures(){
    cp_dsu::init(5);CHECK(cp_dsu::merge(1,5));CHECK(!cp_dsu::merge(1,5));
    CHECK(cp_dsu::find(1)==cp_dsu::find(5));CHECK(cp_dsu::find(1)!=cp_dsu::find(2));
    cp_dsu::init(1);CHECK(!cp_dsu::merge(1,1));
    cp_dsu::init(7);for(int i=1;i<=7;++i)CHECK(cp_dsu::find(i)==i);
    for(int i=1;i<=7;++i)CHECK(cp_dsu::size(i)==1);
    CHECK(cp_dsu::merge(1,2));CHECK(cp_dsu::merge(1,3));
    CHECK(cp_dsu::merge(4,1));CHECK(cp_dsu::size(4)==4);
    CHECK(!cp_dsu::merge(2,4));CHECK(!cp_dsu::merge(3,3));
    for(int i=1;i<=4;++i)CHECK(cp_dsu::size(i)==4);
    CHECK(cp_dsu::size(7)==1);
    cp_dsu::init(2);CHECK(cp_dsu::size(1)==1);CHECK(cp_dsu::size(2)==1);
    for(int z=0;z<30;++z){
        int n=rnd(1,30);cp_dsu::init(n);vector<int> label(n+1);
        iota(label.begin(),label.end(),0);
        for(int it=0;it<60;++it){
            int u=rnd(1,n),v=rnd(1,n),a=label[u],b=label[v];
            CHECK(cp_dsu::merge(u,v)==(a!=b));
            for(int i=1;i<=n;++i)if(label[i]==b)label[i]=a;
            for(int i=1;i<=n;++i)
                CHECK(cp_dsu::size(i)==count(label.begin()+1,label.end(),label[i]));
        }
    }
    cp_rollback::RollbackDSU rb(5);rb.merge(1,5);int snap=rb.snapshot();
    rb.merge(5,2);rb.merge(1,2);CHECK(rb.find(1)==rb.find(2));rb.rollback(snap);
    CHECK(rb.find(1)!=rb.find(2));CHECK(rb.sz[rb.find(1)]==2);rb.rollback(0);
    for(int i=1;i<=5;++i)CHECK(rb.find(i)==i);
    for(int z=0;z<100;++z){
        int n=rnd(1,30);vector<i64>a(n+1,999);for(int i=1;i<=n;++i)a[i]=rnd(-20,20);
        segmenttree st(vector<int>(a.begin(),a.end()));cp_fenwick::Fenwick f(n);
        for(int i=1;i<=n;++i)f.add(i,a[i]);
        for(int it=0;it<100;++it){int l=rnd(1,n),r=rnd(l,n);
            int op=rnd(0,2);
            if(op==2){int v=rnd(-20,20);st.set(l,v);f.add(l,v-a[l]);a[l]=v;}
            else if(op==1){int v=rnd(-20,20);st.add(l,r,v);
                for(int i=l;i<=r;++i){a[i]+=v;f.add(i,v);}}
            else{auto sum=accumulate(a.begin()+l,a.begin()+r+1,0LL);
                CHECK(st.query(l,r)==sum);CHECK(f.query(l,r)==sum);}
        }
        vector<int> sparse_a(a.begin(),a.end());
        cp_sparse::ST sp(sparse_a);
        for(int l=1;l<=n;++l)for(int r=l;r<=n;++r)
            CHECK(sp.query(l,r)==*min_element(a.begin()+l,a.begin()+r+1));
        cp_fenwick::Fenwick freq(n);vector<int>expanded;
        for(int i=1;i<=n;++i){int c=rnd(0,4);freq.add(i,c);while(c--)expanded.push_back(i);}
        for(int k=1;k<=(int)expanded.size();++k)CHECK(freq.kth(k)==expanded[k-1]);
        auto xs=cp_compress::compress(a);cp_persistent::PersistentKth pt(xs.size()-1);
        vector<int>ranks(n+1);for(int i=1;i<=n;++i){ranks[i]=cp_compress::rank_of(xs,a[i]);pt.append(ranks[i]);}
        vector<pair<int,int>>ranges;
        for(int q=0;q<30;++q){int l=rnd(1,n),r=rnd(l,n);ranges.push_back({l,r});
            vector<i64>b(a.begin()+l,a.begin()+r+1);sort(b.begin(),b.end());
            for(int k=1;k<=r-l+1;++k)CHECK(xs[pt.kth(l,r,k)]==b[k-1]);}
        auto res=cp_mo::mo_distinct(ranks,xs.size()-1,ranges);
        for(int i=0;i<(int)ranges.size();++i){auto[l,r]=ranges[i];
            set<i64>values(a.begin()+l,a.begin()+r+1);CHECK(res[i]==(int)values.size());}
        cp_treap::ImplicitTreap tr;for(int i=1;i<=n;++i)tr.append(a[i]);a[0]=0;
        for(int it=0;it<50;++it){int l=rnd(1,n),r=rnd(l,n);tr.reverse(l,r);
            reverse(a.begin()+l,a.begin()+r+1);CHECK(tr.values()==a);}
    }
    for(int z=0;z<100;++z){cp_basis::XorBasis b;set<u64>values{0};
        for(int i=0;i<8;++i){u64 v=rnd(0,255);b.insert(v);auto copy=values;
            for(auto x:copy)values.insert(x^v);}
        CHECK(b.maximum()==*values.rbegin());for(int x=0;x<256;++x)CHECK(b.contains(x)==bool(values.count(x)));
    }
    cp_basis::XorBasis hi;hi.insert(1ULL<<63);CHECK(hi.maximum()==(1ULL<<63));
}

void graphs(){
    for(int n:{0,1,6}){
        cp_scc::SCC a(n);cp_tarjan::TarjanSCC b(n);
        if(n){a.add_edge(1,1);b.add_edge(1,1);}
        if(n==6)for(auto [u,v]:vector<pair<int,int>>{{1,2},{1,2},{2,1},{2,3},{3,4},{4,3},{5,5}}){
            a.add_edge(u,v);b.add_edge(u,v);
        }
        a.build();b.build();CHECK(a.cc==b.cc);
        for(int u=1;u<=n;++u)for(int v=1;v<=n;++v)
            CHECK((a.comp[u]==a.comp[v])==(b.comp[u]==b.comp[v]));
        auto old=b.comp;b.build();CHECK(b.comp==old);
    }
    {cp_tarjan::TarjanSCC t(3);t.add_edge(1,2);t.add_edge(2,3);t.build();CHECK(t.cc==3);
        t.add_edge(3,1);t.build();CHECK(t.cc==1);}
    for(int z=0;z<80;++z){
        int n=rnd(1,8);vector<vector<pair<int,i64>>>g(n+1);
        vector<vector<pair<int,int>>>g01(n+1);vector<vector<i64>>dist(n+1,vector<i64>(n+1,1000000));
        vector<vector<int>>d01(n+1,vector<int>(n+1,1000000));
        cp_scc::SCC sc(n);cp_tarjan::TarjanSCC tarjan(n);
        for(int i=1;i<=n;++i){dist[i][i]=0;d01[i][i]=0;}
        for(int i=1;i<=n;++i)for(int j=1;j<=n;++j)if(i!=j&&rnd(0,3)==0){
            int w=rnd(0,20),b=rnd(0,1);g[i].push_back({j,w});g01[i].push_back({j,b});
            dist[i][j]=w;d01[i][j]=b;sc.add_edge(i,j);tarjan.add_edge(i,j);
        }
        for(int k=1;k<=n;++k)for(int i=1;i<=n;++i)for(int j=1;j<=n;++j){
            dist[i][j]=min(dist[i][j],dist[i][k]+dist[k][j]);
            d01[i][j]=min(d01[i][j],d01[i][k]+d01[k][j]);}
        sc.build();tarjan.build();CHECK(sc.cc==tarjan.cc);
        for(int u=1;u<=n;++u)for(auto [v,w]:g[u])if(sc.comp[u]!=sc.comp[v]){
            CHECK(sc.comp[u]<sc.comp[v]);CHECK(tarjan.comp[u]>tarjan.comp[v]);}
        for(int i=1;i<=n;++i){auto dd=cp_dijkstra::dijkstra(g,i);auto db=cp_bfs01::bfs01(g01,i);
            for(int j=1;j<=n;++j){CHECK(dd[j]==(dist[i][j]==1000000?INF:dist[i][j]));
                CHECK(db[j]==(d01[i][j]==1000000?INF:d01[i][j]));
                CHECK((sc.comp[i]==sc.comp[j])==(dist[i][j]<1000000&&dist[j][i]<1000000));
                CHECK((tarjan.comp[i]==tarjan.comp[j])==(dist[i][j]<1000000&&dist[j][i]<1000000));}}
    }
    {using namespace cp_bellman;auto [neg,d]=bellman_ford(3,{{1,2,3},{2,3,-5}},1);
        CHECK(!neg&&d[3]==-2);CHECK(bellman_ford(2,{{1,2,0},{2,1,-1}},1).first);
        CHECK(!bellman_ford(3,{{2,3,-2},{3,2,1}},1).first);}
    CHECK(cp_topo::topo_sort({{},{2},{3},{}})==vector<int>({1,2,3}));
    CHECK(cp_topo::topo_sort({{},{2},{1}}).empty());
    for(int z=0;z<60;++z){int n=rnd(2,5);vector<cp_kruskal::Edge>es;
        for(int u=1;u<=n;++u)for(int v=u+1;v<=n;++v)es.push_back({u,v,rnd(-9,20)});
        i64 best=INF;int m=es.size();
        for(int mask=0;mask<(1<<m);++mask)if(__builtin_popcount((unsigned)mask)==n-1){
            vector<vector<int>>g(n+1);i64 w=0;
            for(int i=0;i<m;++i)if(mask>>i&1){g[es[i].u].push_back(es[i].v);g[es[i].v].push_back(es[i].u);w+=es[i].w;}
            vector<int>st{1};vector<char>vis(n+1);vis[1]=1;
            for(int j=0;j<(int)st.size();++j)for(int v:g[st[j]])if(!vis[v]){vis[v]=1;st.push_back(v);}
            if((int)st.size()==n)best=min(best,w);}
        auto [w,c]=cp_kruskal::kruskal(n,es);CHECK(w==best&&c==n-1);
    }
    CHECK(cp_kruskal::kruskal(3,{{1,2,4}})==make_pair(4LL,1));
    for(int z=0;z<150;++z){int n=rnd(1,7),m=rnd(0,20);cp_twosat::TwoSAT t(n);
        vector<array<int,4>>cs;for(int i=0;i<m;++i){int a=rnd(1,n),b=rnd(1,n),av=rnd(0,1),bv=rnd(0,1);
            t.clause(a,av,b,bv);cs.push_back({a,av,b,bv});}
        auto valid=[&](int mask){for(auto [a,av,b,bv]:cs)if(((mask>>(a-1)&1)!=av)&&((mask>>(b-1)&1)!=bv))return false;return true;};
        bool exists=false;for(int mask=0;mask<(1<<n);++mask)exists|=valid(mask);
        bool got=t.solve();CHECK(got==exists);
        if(got){int mask=0;for(int i=1;i<=n;++i)mask|=t.value[i]<<(i-1);CHECK(valid(mask));}
        cp_tarjan::TarjanSCC sc(2*n);
        for(auto [a,av,b,bv]:cs){int x=2*a-1+av,y=2*b-1+bv;
            sc.add_edge(x%2?x+1:x-1,y);sc.add_edge(y%2?y+1:y-1,x);}
        sc.build();bool feasible=true;int mask=0;
        for(int i=1;i<=n;++i){feasible&=sc.comp[2*i-1]!=sc.comp[2*i];mask|=(sc.comp[2*i-1]>sc.comp[2*i])<<(i-1);}
        CHECK(feasible==exists);if(feasible)CHECK(valid(mask));
    }
    for(int z=0;z<80;++z){int n=rnd(1,8),m=rnd(0,12);vector<pair<int,int>>edges;cp_bridges::LowLink ll(n);
        for(int i=0;i<m;++i){int u=rnd(1,n),v=rnd(1,n);edges.push_back({u,v});ll.add_edge(u,v,i);}ll.build();
        auto comps=[&](int banV,int banE){vector<vector<int>>g(n+1);
            for(int i=0;i<m;++i){auto[u,v]=edges[i];if(i!=banE&&u!=banV&&v!=banV){g[u].push_back(v);g[v].push_back(u);}}
            vector<char>vis(n+1);int count=0;
            for(int u=1;u<=n;++u)if(u!=banV&&!vis[u]){++count;vector<int>st{u};vis[u]=1;
                for(int j=0;j<(int)st.size();++j)for(int v:g[st[j]])if(!vis[v]){vis[v]=1;st.push_back(v);}}
            return count;};
        int base=comps(-1,-1);set<int>bridges(ll.bridges.begin(),ll.bridges.end());
        for(int i=0;i<m;++i)CHECK(bool(bridges.count(i))==(comps(-1,i)>base));
        for(int u=1;u<=n;++u)CHECK(bool(ll.cut[u])==(comps(u,-1)>base));
    }
}

void trees(){
    for(int z=0;z<60;++z){int n=rnd(1,40);vector<vector<int>>g(n+1);vector<int>par(n+1,1);
        for(int i=2;i<=n;++i){par[i]=rnd(1,i-1);g[i].push_back(par[i]);g[par[i]].push_back(i);}
        int root=(z%3==0?n:1);cp_hld::HLD h(g,root);
        vector<int>order{root};par[root]=root;
        for(int i=0;i<n;++i)for(int v:g[order[i]])if(v!=par[order[i]]){
            par[v]=order[i];order.push_back(v);
        }
        auto ancestor=[&](int a,int b){while(b!=a&&b!=root)b=par[b];return a==b;};
        for(int u=1;u<=n;++u)for(int v=1;v<=n;++v){
            int lca=u;while(!ancestor(lca,v))lca=par[lca];CHECK(h.lca(u,v)==lca);
            CHECK((h.pos[u]<=h.pos[v]&&h.pos[v]<=h.pos[u]+h.sz[u]-1)==ancestor(u,v));
            vector<int>got,exp;h.path(u,v,[&](int l,int r){for(int i=l;i<=r;++i)got.push_back(h.rev[i]);});
            for(int x=u;x!=lca;x=par[x])exp.push_back(x);for(int x=v;x!=lca;x=par[x])exp.push_back(x);exp.push_back(lca);
            sort(got.begin(),got.end());sort(exp.begin(),exp.end());CHECK(got==exp);
        }
        vector<int>keys;for(int i=0;i<10;++i)keys.push_back(rnd(1,n));
        auto [nodes,edges]=cp_virtual::virtual_tree(h,keys);CHECK(edges.size()+1==nodes.size());
        set<int>s(nodes.begin(),nodes.end());for(int k:keys)CHECK(s.count(k));
        for(auto [u,v]:edges){CHECK(ancestor(u,v));for(int w:nodes)if(w!=u&&w!=v)CHECK(!(ancestor(u,w)&&ancestor(w,v)));}
        CHECK(cp_virtual::virtual_tree(h,{}).first.empty());
    }
}

void tree_difference(){
    for(int shape=0;shape<3;++shape)for(int n=1;n<=30;++n){
        vector<vector<int>>g(n+1);
        for(int u=2;u<=n;++u){
            int p=shape==0?u-1:shape==1?1:rnd(1,u-1);
            g[u].push_back(p);g[p].push_back(u);
        }
        for(int root:vector<int>{1,n}){
            cp_tree_point_diff::TreePointDiff points(g,root);
            cp_tree_edge_diff::TreeEdgeDiff edges(g,root);
            vector<int>parent(n+1,-1),order{root};parent[root]=0;
            for(int i=0;i<n;++i)for(int v:g[order[i]])if(parent[v]==-1){
                parent[v]=order[i];order.push_back(v);
            }
            vector<i64>expectedPoints(n+1),expectedEdges(n+1);
            CHECK(points.result()==expectedPoints);CHECK(edges.result()==expectedEdges);
            for(int u=1;u<=n;++u)for(int v=1;v<=n;++v){
                i64 val=rnd(-100,100);points.add(u,v,val);edges.add(u,v,val);
                // 从 u 搜索到 v，逐点、逐边直接更新，独立于被测 LCA。
                vector<int>prev(n+1,-1),queue{u};prev[u]=0;
                for(int i=0;i<(int)queue.size();++i)for(int x:g[queue[i]])if(prev[x]==-1){
                    prev[x]=queue[i];queue.push_back(x);
                }
                for(int x=v;;x=prev[x]){
                    expectedPoints[x]+=val;
                    if(x==u)break;
                    int p=prev[x];expectedEdges[parent[x]==p?x:p]+=val;
                }
                CHECK(points.result()==expectedPoints);CHECK(edges.result()==expectedEdges);
                CHECK(points.result()==expectedPoints);CHECK(edges.result()==expectedEdges);
            }
        }
    }
    int n=100000;vector<vector<int>>g(n+1);
    for(int i=2;i<=n;++i){g[i-1].push_back(i);g[i].push_back(i-1);}
    cp_tree_point_diff::TreePointDiff points(g,n);
    cp_tree_edge_diff::TreeEdgeDiff edges(g,n);
    points.add(1,n,3000000000LL);edges.add(1,n,-3000000000LL);
    auto p=points.result(),e=edges.result();
    for(int i=1;i<=n;++i){CHECK(p[i]==3000000000LL);CHECK(e[i]==(i==n?0:-3000000000LL));}
}

void flows(){
    for(int z=0;z<100;++z){int n=rnd(2,7);cp_dinic::Dinic d(n);vector<tuple<int,int,int>>es;
        for(int u=1;u<=n;++u)for(int v=1;v<=n;++v)if(u!=v&&rnd(0,3)==0){int c=rnd(0,8);d.add_edge(u,v,c);es.push_back({u,v,c});}
        i64 best=INF;for(int mask=0;mask<(1<<n);++mask)if((mask&1)&&!(mask>>(n-1)&1)){
            i64 c=0;for(auto[u,v,w]:es)if((mask>>(u-1)&1)&&!(mask>>(v-1)&1))c+=w;best=min(best,c);}
        CHECK(d.flow(1,n)==best);CHECK(d.flow(1,n)==0);
    }
    for(int z=0;z<80;++z){int l=rnd(1,6),r=rnd(1,6);cp_matching::BipartiteMatch b(l,r);
        for(int u=1;u<=l;++u)for(int v=1;v<=r;++v)if(rnd(0,1))b.g[u].push_back(v);
        function<int(int,int)>dfs=[&](int u,int mask){if(u>l)return 0;int ans=dfs(u+1,mask);
            for(int v:b.g[u])if(!(mask>>(v-1)&1))ans=max(ans,1+dfs(u+1,mask|(1<<(v-1))));return ans;};
        CHECK(b.solve()==dfs(1,0));CHECK(b.solve()==dfs(1,0));
    }
    for(int z=0;z<100;++z){int n=rnd(2,5),lim=rnd(1,3);cp_mcf::MinCostFlow f(n);vector<tuple<int,int,int>>es;
        for(int u=1;u<=n;++u)for(int v=u+1;v<=n;++v)if(rnd(0,1)){int c=rnd(-5,8);f.add_edge(u,v,1,c);es.push_back({u,v,c});}
        int bestf=0;i64 bestc=0;int m=es.size();
        for(int mask=0;mask<(1<<m);++mask){vector<int>bal(n+1);i64 cost=0;
            for(int i=0;i<m;++i)if(mask>>i&1){auto[u,v,c]=es[i];--bal[u];++bal[v];cost+=c;}
            bool ok=true;for(int i=2;i<n;++i)ok&=bal[i]==0;int flow=bal[n];
            if(!ok||flow>lim||flow<0||bal[1]!=-flow)continue;
            if(flow>bestf||(flow==bestf&&cost<bestc)){bestf=flow;bestc=cost;}}
        auto [flow,cost]=f.flow(1,n,lim);CHECK(flow==bestf&&cost==bestc);
    }
}

void strings(){
    CHECK(cp_kmp::kmp("ababa","aba")==vector<int>({0,2}));
    for(int z=0;z<120;++z){int n=rnd(0,20);string s;for(int i=0;i<n;++i)s+=char('a'+rnd(0,2));
        auto pi=cp_kmp::prefix_function(s);auto zz=cp_zfunc::z_function(s);
        for(int i=0;i<n;++i){int best=0;for(int k=1;k<=i;++k)if(s.substr(0,k)==s.substr(i-k+1,k))best=k;CHECK(pi[i]==best);
            int len=0;while(i+len<n&&s[len]==s[i+len])++len;CHECK(zz[i]==len);}
        auto [d1,d2]=cp_manacher::manacher(s);i64 count=0;set<string>subs;
        cp_strhash::StringHash hash(s);
        for(int l=0;l<n;++l)for(int r=l+1;r<=n;++r){string t=s.substr(l,r-l),rev=t;reverse(rev.begin(),rev.end());count+=t==rev;subs.insert(t);
            cp_strhash::StringHash hh(t);CHECK(hash.get(l,r)==hh.get(0,r-l));}
        CHECK(count==accumulate(d1.begin(),d1.end(),0LL)+accumulate(d2.begin(),d2.end(),0LL));
        cp_suffixarray::SuffixArray sa(s);vector<int>exp(n);iota(exp.begin(),exp.end(),0);
        sort(exp.begin(),exp.end(),[&](int a,int b){return s.substr(a)<s.substr(b);});CHECK(sa.sa==exp);
        for(int i=1;i<n;++i){int len=0;while(sa.sa[i]+len<n&&sa.sa[i-1]+len<n&&s[sa.sa[i]+len]==s[sa.sa[i-1]+len])++len;CHECK(sa.lcp[i]==len);}
        cp_sam::SAM sam;for(char c:s)sam.extend(c);CHECK(sam.distinct()==(i64)subs.size());auto occ=sam.occurrences();
        cp_aho::Aho ac;vector<string>patterns;vector<int>ends;
        for(int k=0;k<8;++k){string p;for(int j=0,m=rnd(1,5);j<m;++j)p+=char('a'+rnd(0,2));patterns.push_back(p);ends.push_back(ac.insert(p));}
        ac.build();auto cnt=ac.count(s);
        for(int k=0;k<8;++k){string p=patterns[k];vector<int>positions;for(int i=0;i+(int)p.size()<=n;++i)if(s.substr(i,p.size())==p)positions.push_back(i);
            CHECK(cp_kmp::kmp(s,p)==positions);CHECK(cnt[ends[k]]==(i64)positions.size());
            int u=0;for(char c:p){u=sam.t[u].go[c-'a'];if(u==-1)break;}
            CHECK((u==-1?0:occ[u])==(i64)positions.size());}
        CHECK(cp_kmp::kmp(s,"").size()==s.size()+1);
        int rot=cp_booth::min_rotation(s);string got=s.substr(rot)+s.substr(0,rot);
        for(int i=0;i<n;++i)CHECK(got<=s.substr(i)+s.substr(0,i));
    }
}

void math(){
    CHECK(cp_qpow::qpow(2,10,1000)==24);CHECK(cp_qpow::qpow(-2,3,7)==6);
    CHECK(cp_qpow::qpow(2,0,1)==0);CHECK(cp_qpow::qpow(0,0,7)==1);
    CHECK(cp_qpow::qpow(LLONG_MAX-1,2,LLONG_MAX)==1);
    CHECK(cp_qpow::qpow(LLONG_MIN,1,LLONG_MAX)==LLONG_MAX-1);
    CHECK(cp_qpow::qpow(-1,ULLONG_MAX,7)==6);
    for(int p:{2,3,7,11,97})for(int a=-100;a<=100;++a)if(a%p!=0){
        auto inv=cp_fermat::inverse_fermat(a,p);
        CHECK(cp_modular::norm(a,p)*inv%p==1);
        CHECK(cp_modular::inverse(a,p)==inv);
    }
    for(int m=2;m<80;++m)for(int a=-20;a<80;++a){auto inv=cp_modular::inverse(a,m);
        CHECK(bool(inv)==(gcd(abs(a),m)==1));if(inv)CHECK(cp_modular::norm(a,m)*(*inv)%m==1);}
    cp_sieve::Sieve s(1000);
    for(int n=1;n<=1000;++n){int phi=0;for(int a=1;a<=n;++a)phi+=gcd(a,n)==1;CHECK(phi==s.phi[n]);
        int x=n,mu=1;for(int p=2;p<=x;++p)if(x%p==0){int cnt=0;while(x%p==0){x/=p;++cnt;}if(cnt>1)mu=0;else mu=-mu;}CHECK(mu==s.mu[n]);}
    cp_comb::Comb comb(100,1000000007);vector<vector<i64>>pas(101,vector<i64>(101));pas[0][0]=1;
    for(int n=1;n<=100;++n){pas[n][0]=1;for(int k=1;k<=n;++k)pas[n][k]=(pas[n-1][k]+pas[n-1][k-1])%1000000007;}
    for(int n=0;n<=100;++n)for(int k=0;k<=n;++k)CHECK(comb.C(n,k)==pas[n][k]);
    cp_lucas::Lucas small(7);vector<vector<int>>p7(101,vector<int>(101));p7[0][0]=1;
    for(int n=1;n<=100;++n){p7[n][0]=1;for(int k=1;k<=n;++k)p7[n][k]=(p7[n-1][k]+p7[n-1][k-1])%7;}
    for(int n=0;n<=100;++n)for(int k=0;k<=n;++k)CHECK(small.lucas(n,k)==p7[n][k]);
    for(int z=0;z<200;++z){int m=rnd(1,40),n=rnd(1,40),a=rnd(-50,50),b=rnd(-50,50);auto c=cp_crt::crt_merge(a,m,b,n);
        int exp=-1;for(int x=0;x<lcm(m,n);++x)if((x-a)%m==0&&(x-b)%n==0){exp=x;break;}
        CHECK(c.status==(exp==-1?0:1));if(c.status==1)CHECK(c.r==exp&&c.m==lcm(m,n));}
    CHECK(cp_crt::crt_merge(0,LLONG_MAX,0,LLONG_MAX-1).status==2);
    for(u64 n=0;n<6000;++n){bool prime=n>=2;for(u64 d=2;d*d<=n;++d)if(n%d==0){prime=false;break;}CHECK(cp_primality::is_prime(n)==prime);}
    CHECK(cp_primality::is_prime(18446744073709551557ULL));CHECK(!cp_primality::is_prime(341550071728321ULL));
    for(u64 n:{1ULL,2ULL,4ULL,600851475143ULL,1000000016000000063ULL,18446744073709551615ULL}){
        vector<u64>f;cp_factor::factor(n,f);__uint128_t prod=1;for(u64 p:f){CHECK(cp_primality::is_prime(p));prod*=p;}CHECK(prod==n);}
    for(int n=0;n<300;++n){i128 ans=0;for(int i=1;i<=n;++i)ans+=n/i;CHECK(cp_quotient::quotient_sum(n)==ans);}
    for(int z=0;z<100;++z){int n=rnd(1,30),m=rnd(1,30);vector<int>a(n),b(m),c(n+m-1);vector<double>ad(n),bd(m);
        for(int i=0;i<n;++i)ad[i]=a[i]=rnd(0,100);for(int i=0;i<m;++i)bd[i]=b[i]=rnd(0,100);
        for(int i=0;i<n;++i)for(int j=0;j<m;++j)c[i+j]=(c[i+j]+1LL*a[i]*b[j])%cp_ntt::MOD;
        CHECK(cp_ntt::convolution(a,b)==c);auto f=cp_fft::convolution(ad,bd);for(int i=0;i<(int)c.size();++i)CHECK(abs(f[i]-c[i])<1e-6);}
    CHECK(cp_ntt::convolution({},{}).empty());CHECK(cp_fft::convolution({},{}).empty());
    auto [status,x]=cp_gauss::gauss({{0,0,0,0},{0,1,1,3},{0,1,-1,1}});CHECK(status==1&&fabsl(x[1]-2)<1e-9&&fabsl(x[2]-1)<1e-9);
    CHECK(cp_gauss::gauss({{0,0,0,0},{0,1,1,2},{0,2,2,4}}).first==2);CHECK(cp_gauss::gauss({{0,0,0,0},{0,1,1,2},{0,2,2,5}}).first==0);
    i64 a=0,b=1;for(int n=0;n<100;++n){CHECK(cp_matrix::matrix_power({{0,0,0},{0,1,1},{0,1,0}},n,1000000007)[1][2]==a);
        CHECK(cp_recurrence::linear_nth({0,1},{1,1},n,1000000007)==a);tie(a,b)=pair{b,(a+b)%1000000007};}
    for(int z=0;z<80;++z){int k=rnd(1,6);vector<i64>init(k),c(k),f(80);for(int i=0;i<k;++i){f[i]=init[i]=rnd(0,50);c[i]=rnd(0,50);}
        for(int i=k;i<80;++i)for(int j=1;j<=k;++j)f[i]=(f[i]+c[j-1]*f[i-j])%1000000007;
        for(int n=0;n<80;++n)CHECK(cp_recurrence::linear_nth(init,c,n,1000000007)==f[n]);}
    CHECK(!cp_games::nim_win({0,1,2,3}));CHECK(cp_games::nim_win({0,1,2,4}));
    CHECK(!cp_misere::misere_nim_win({0,1,1,1}));CHECK(cp_misere::misere_nim_win({0,1,1}));
    auto sg=cp_sg::subtraction_sg(100,{1,2});for(int i=0;i<=100;++i)CHECK(sg[i]==i%3);
}

void dp(){
    for(int z=0;z<100;++z){int n=rnd(0,12);vector<i64>a(n);for(auto&v:a)v=rnd(-10,10);
        int best=0;for(int mask=0;mask<(1<<n);++mask){i64 prev=LLONG_MIN;bool ok=true;int len=0;
            for(int i=0;i<n;++i)if(mask>>i&1){ok&=a[i]>prev;prev=a[i];++len;}if(ok)best=max(best,len);}
        auto ids=cp_lis::lis_indices(one_based(a));CHECK((int)ids.size()==best);
        for(int i=1;i<(int)ids.size();++i)CHECK(ids[i]>ids[i-1]&&a[ids[i]-1]>a[ids[i-1]-1]);
        int W=rnd(0,30),m=rnd(1,7);vector<int>w(m),count(m);vector<i64>v(m),got(W+1),exp(W+1);
        for(int i=0;i<m;++i){w[i]=rnd(1,10);v[i]=rnd(-3,15);count[i]=rnd(0,5);cp_bounded::bounded(got,w[i],v[i],count[i]);
            auto next=exp;for(int c=0;c<=W;++c)for(int k=1;k<=count[i]&&k*w[i]<=c;++k)next[c]=max(next[c],exp[c-k*w[i]]+k*v[i]);exp=next;}
        CHECK(got==exp);
        fill(got.begin(),got.end(),0);fill(exp.begin(),exp.end(),0);
        for(int i=0;i<m;++i){cp_complete::complete(got,w[i],v[i]);auto next=exp;
            for(int c=0;c<=W;++c)for(int k=1;k*w[i]<=c;++k)next[c]=max(next[c],exp[c-k*w[i]]+k*v[i]);exp=next;}CHECK(got==exp);
    }
    CHECK(cp_lcs::lcs("abcde","ace")==3);CHECK(cp_lcs::lcs("aaaa","aa")==2);CHECK(cp_lcs::lcs("","ab")==0);
    u64 cnt=0;for(int n=0;n<=10000;++n){string s=to_string(n);bool ok=true;for(int i=1;i<(int)s.size();++i)ok&=s[i]!=s[i-1];cnt+=ok;
        if(n<100||n%37==0)CHECK(cp_digitdp::count_no_equal_adjacent(n)==cnt);}
    CHECK(cp_digitdp::count_no_equal_adjacent(-1)==0);
    for(int z=0;z<30;++z){int n=rnd(0,7);vector<i64>a(1<<n);for(auto&v:a)v=rnd(-10,10);
        auto sub=cp_sos::subset_sum(a,n),sup=cp_sos::superset_sum(a,n);
        for(int s=0;s<(1<<n);++s){i64 x=0,y=0;for(int t=0;t<(1<<n);++t){if((t&s)==t)x+=a[t];if((t&s)==s)y+=a[t];}CHECK(sub[s]==x&&sup[s]==y);}}
    for(int z=0;z<70;++z){int n=rnd(1,12);vector<vector<int>>g(n);vector<pair<int,int>>es;vector<i64>w(n);
        for(int i=1;i<n;++i){int p=rnd(0,i-1);g[p].push_back(i);g[i].push_back(p);es.push_back({p,i});}
        for(auto&v:w)v=rnd(-5,12);i64 best=0;
        for(int mask=0;mask<(1<<n);++mask){bool ok=true;for(auto[u,v]:es)if((mask>>u&1)&&(mask>>v&1))ok=false;
            if(ok){i64 sum=0;for(int i=0;i<n;++i)if(mask>>i&1)sum+=w[i];best=max(best,sum);}}
        CHECK(cp_tree_dp::tree_independent_set(graph_one_based(g),one_based(w))==best);
    }
    for(int z=0;z<50;++z){vector<i64>xs;for(int i=0;i<30;++i)xs.push_back(rnd(-100,100));cp_lichao::LiChao lc(one_based(xs));
        vector<cp_lichao::LiChao::Line>lines;
        for(int i=0;i<40;++i){cp_lichao::LiChao::Line l{rnd(-100,100),rnd(-100,100)};lines.push_back(l);lc.add(l);
            for(i64 x:xs){i128 best=lc.INF;for(auto l:lines)best=min(best,l.eval(x));CHECK(lc.query(x)==best);}}
    }
    for(int z=0;z<30;++z){int n=rnd(1,40);vector<i64>s(n+1);for(int i=1;i<=n;++i)s[i]=s[i-1]+rnd(0,10);
        auto cost=[&](int l,int r){return (s[r]-s[l])*(s[r]-s[l]);};
        vector<i64>prev(n+1,INF);prev[0]=0;
        for(int k=1;k<=5;++k){vector<i64>exp(n+1,INF);
            for(int i=1;i<=n;++i)for(int j=0;j<i;++j)if(prev[j]!=INF)exp[i]=min(exp[i],prev[j]+cost(j,i));
            auto got=cp_divide_dp::divide_dp_layer(prev,cost);CHECK(got==exp);prev=got;}}
}

void geometry(){
    using cp_intgeo::Point;
    CHECK(cp_intgeo::intersect({0,0},{2,0},{1,0},{3,0}));
    CHECK(!cp_intgeo::intersect({0,0},{1,0},{2,0},{3,0}));
    CHECK(cp_intgeo::intersect({0,0},{0,0},{0,0},{1,1}));
    CHECK(cp_intgeo::intersect({0,0},{2,2},{0,2},{2,0}));
    for(int z=0;z<150;++z){vector<Point>p;int n=rnd(0,30);for(int i=0;i<n;++i)p.push_back({rnd(-20,20),rnd(-20,20)});
        auto h=cp_hull::convex_hull(p);i128 best=0;for(auto a:p)for(auto b:p)best=max(best,cp_diameter::dist2(a,b));
        CHECK(cp_diameter::diameter2(h)==best);
        for(auto a:p)CHECK(cp_point_location::inside(h,a)!=0);
        if(h.size()>=3){CHECK(cp_polygon::area2(h)>0);for(int i=0;i<(int)h.size();++i)CHECK(cp_intgeo::cross(h[i],h[(i+1)%h.size()],h[(i+2)%h.size()])>0);}
    }
    vector<Point>sq{{0,0},{4,0},{4,4},{0,4}};
    CHECK(cp_point_location::inside(sq,{2,2})==2);CHECK(cp_point_location::inside(sq,{0,2})==1);CHECK(cp_point_location::inside(sq,{5,2})==0);
    CHECK(cp_polygon::area2(sq)==32);
    auto p=cp_floatgeo::line_intersection({0,0},{1,1},{0,1},{1,-1});CHECK(p&&fabsl(p->x-.5L)<1e-10&&fabsl(p->y-.5L)<1e-10);
    CHECK(!cp_floatgeo::line_intersection({0,0},{1,1},{0,1},{1,1}));
    auto [cnt,points]=cp_circle::circle_intersection({0,0},1,{1,0},1);CHECK(cnt==2);
    for(auto p:points){CHECK(fabsl(p.x*p.x+p.y*p.y-1)<1e-10);CHECK(fabsl((p.x-1)*(p.x-1)+p.y*p.y-1)<1e-10);}
    CHECK(cp_circle::circle_intersection({0,0},1,{2,0},1).first==1);
    CHECK(cp_circle::circle_intersection({0,0},1,{0,0},1).first==-1);
    CHECK(cp_circle::circle_intersection({0,0},1,{3,0},1).first==0);
    CHECK(cp_circle::circle_intersection({0,0},0,{0,0},0).first==1);
    auto cut=cp_halfplane::clip_halfplane({{0,0},{4,0},{4,4},{0,4}},{2,-1},{2,5});
    long double area=0;for(int i=0;i<(int)cut.size();++i){CHECK(cut[i].x<=2+1e-10);area+=cp_floatgeo::cross(cut[i],cut[(i+1)%cut.size()]);}
    CHECK(fabsl(area-16)<1e-10);
    CHECK(cp_halfplane::clip_halfplane(cut,{-1,5},{-1,-1}).size()>0);
    CHECK(cp_halfplane::clip_halfplane(cut,{-1,-1},{-1,5}).empty());
}

int main(){
    basics();cout<<"basics OK\n";
    structures();cout<<"structures OK\n";
    graphs();cout<<"graphs OK\n";
    trees();cout<<"trees OK\n";
    tree_difference();cout<<"tree difference OK\n";
    flows();cout<<"flows OK\n";
    strings();cout<<"strings OK\n";
    math();cout<<"math OK\n";
    dp();cout<<"dp OK\n";
    geometry();cout<<"geometry OK\n";
    cout<<"PASS "<<checks<<" checks; seed=20261006\n";
}
