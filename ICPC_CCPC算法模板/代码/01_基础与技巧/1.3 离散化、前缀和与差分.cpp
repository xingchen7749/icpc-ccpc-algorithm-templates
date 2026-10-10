#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
// #define int long long

// a[0] is unused; xs[0] is also unused.
// 将 a[1..n] 排序去重，返回带 0 号占位的有序值表 xs；不修改原数组。
// 要求 a 至少包含占位项；时间 O(n log n)，空间 O(n)。
template<class T> vector<T> compress(const vector<T>& a) {
    vector<T> xs(a.begin()+1,a.end());
    sort(xs.begin(),xs.end());
    xs.erase(unique(xs.begin(),xs.end()),xs.end());
    xs.insert(xs.begin(),T{});return xs;
}
// 返回 x 在有序离散值表 xs[1..m] 中的 lower_bound 位置，作为 1 下标的秩。
// 通常要求 x 已在值表中；否则返回插入位置，可能为 m+1。时间 O(log m)。
template<class T> int rank_of(const vector<T>& xs,T x) {
    return lower_bound(xs.begin()+1,xs.end(),x)-xs.begin();
}
// 由 a[1..n] 构造前缀和 s，s[0]=0；[l,r] 的和为 s[r]-s[l-1]。
// 忽略 a[0]，不修改 a；要求累加不溢出 i64，时间和空间均为 O(n)。
vector<i64> prefix_sum(const vector<i64>& a) {
    vector<i64>s(a.size());
    for(int i=1;i<(int)a.size();++i)s[i]=s[i-1]+a[i];
    return s; // sum[l,r] = s[r]-s[l-1]
}
// 在差分数组 diff 上记录闭区间 [l,r] 加 v，不直接还原原数组；l>r 时不操作。
// 有效更新要求 l>=1 且 r+1 在数组内，通常分配 n+2 项；时间 O(1)。
void range_add(vector<i64>& diff,int l,int r,i64 v) {
    if(l>r)return;
    diff[l]+=v;diff[r+1]-=v;
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
