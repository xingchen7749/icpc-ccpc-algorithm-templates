#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
// #define int long long

struct DSU {
    vector<int> p, sz;
    // 初始化编号 1..n 的 n 个独立集合，0 号位置不用；时间和空间 O(n)。
    DSU(int n): p(n+1), sz(n+1,1) { iota(p.begin(),p.end(),0); }
    // 返回元素 x 所在集合的代表元，并用路径压缩缩短查找路径；要求 1<=x<=n。
    int find(int x) {
        while (x != p[x]) { p[x] = p[p[x]]; x = p[x]; }
        return x;
    }
    // 按集合大小合并 a、b 所在集合；成功合并返回 true，原本同集合返回 false。
    // 会修改父指针和集合大小，配合路径压缩均摊 O(α(n))。
    bool merge(int a, int b) {
        a=find(a); b=find(b); if (a==b) return false;
        if (sz[a]<sz[b]) swap(a,b);
        p[b]=a; sz[a]+=sz[b]; return true;
    }
    // 返回 x 所在连通块的元素数，查找时可能压缩路径；要求 x 为有效元素。
    int size(int x) { return sz[find(x)]; }
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
