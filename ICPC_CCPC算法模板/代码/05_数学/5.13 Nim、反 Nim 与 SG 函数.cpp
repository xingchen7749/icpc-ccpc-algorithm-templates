#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
// #define int long long

// 判断正常 Nim 先手是否必胜：a[1..n] 为各堆石子数，忽略 a[0]。
// 所有堆大小异或非零返回 true，空局面返回 false；时间 O(n)。
bool nim_win(const vector<u64>&a){u64 x=0;for(int i=1;i<(int)a.size();++i)x^=a[i];return x!=0;}
// 判断取最后一颗石子者输的反 Nim 先手胜负，堆数组为 a[1..n]。
// 存在大于 1 的堆时看异或和，否则看 1 石子堆是否为偶数；本实现空局面返回 true。
bool misere_nim_win(const vector<u64>&a){
    u64 x=0;int ones=0;bool big=false;
    for(int i=1;i<(int)a.size();++i){u64 v=a[i];x^=v;ones+=v==1;big|=v>1;}
    return big?x!=0:ones%2==0;
}
// 计算减法游戏状态 0..N 的 SG 值；moves 是每步可取走数量的无占位列表。
// 要求 N>=0、每个动作 d>0；返回 sg[0]=0 的数组，时间 O(N*(|moves|+1))。
vector<int> subtraction_sg(int N,const vector<int>&moves){
    int K=moves.size();vector<int>sg(N+1),seen(K+1,-1);
    for(int x=1;x<=N;++x){
        for(int d:moves){assert(d>0);if(d<=x&&sg[x-d]<=K)seen[sg[x-d]]=x;}
        int g=0;while(g<=K&&seen[g]==x)++g;sg[x]=g;
    }
    return sg;
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
