#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
// #define int long long

struct Aho {
    struct Node{array<int,26>go{};int fail=0;};
    vector<Node>t{{}};vector<int>order;
    // 插入非空小写模式串 s，返回其终止节点编号；重复模式可返回同一节点。
    // 只能在 build 前调用，时间 O(|s|)。
    int insert(const string&s){
        assert(!s.empty());int u=0;
        for(char ch:s){int c=ch-'a';assert(0<=c&&c<26);
            if(!t[u].go[c]){t[u].go[c]=t.size();t.push_back({});}
            u=t[u].go[c];
        }return u;
    }
    // 在全部模式插入后构造 fail 指针并补全字符转移，记录 BFS 顺序 order。
    // 本实现应只调用一次，之后不能继续 insert；S 个节点耗时 O(26S)。
    void build(){
        queue<int>q;for(int v:t[0].go)if(v)q.push(v);
        while(!q.empty()){
            int u=q.front();q.pop();order.push_back(u);
            for(int c=0;c<26;++c){
                int v=t[u].go[c];
                if(v){t[v].fail=t[t[u].fail].go[c];q.push(v);}
                else t[u].go[c]=t[t[u].fail].go[c];
            }
        }
    }
    // 统计小写文本 s 中各自动机节点所代表模式的出现次数，允许重叠。
    // 需先 build；返回值按节点编号索引，模式答案取其 insert 返回的终止节点。
    // 时间 O(|s|+S)，不修改自动机，可以对不同文本重复调用。
    vector<i64> count(const string&s)const{
        vector<i64>cnt(t.size());int u=0;
        for(char ch:s){u=t[u].go[ch-'a'];++cnt[u];}
        for(int i=(int)order.size()-1;i>=0;--i){int v=order[i];cnt[t[v].fail]+=cnt[v];}
        return cnt;
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
