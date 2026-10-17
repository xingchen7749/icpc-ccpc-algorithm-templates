#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

// 返回前缀函数 pi，pi[i] 是 s[0..i] 的最长真前缀与后缀的公共长度。
// 字符串和返回数组均为 0 下标，空串返回空数组；时间和空间 O(|s|)。
// KMP（Knuth–Morris–Pratt）：字符串匹配算法，以三位提出者的姓氏命名。
vector<int> prefix_function(const string& s) {
    // pi[i]：s[0..i] 的最长相等真前缀与真后缀的长度，不包含整个串自身。
    vector<int> pi(s.size());
    for (int i = 1; i < (int)s.size(); ++i) {
        int j = pi[i - 1];
        while (j && s[i] != s[j]) {
            j = pi[j - 1];
        }
        if (s[i] == s[j]) ++j;
        pi[i] = j;
    }
    return pi;
}
// 返回模式串 p 在 text 中所有匹配起点，使用 0 下标，允许重叠匹配。
// 空模式返回 0..text.size() 的全部边界；时间 O(|text|+|p|)，不修改输入。
vector<int> kmp(const string& text, const string& p) {
    vector<int> ans;
    if (p.empty()) {
        ans.resize(text.size() + 1);
        iota(ans.begin(), ans.end(), 0);
        return ans;
    }
    auto pi = prefix_function(p);
    int j = 0;
    for (int i = 0; i < (int)text.size(); ++i) {
        while (j && text[i] != p[j]) {
            j = pi[j - 1];
        }
        if (text[i] == p[j]) ++j;
        if (j == (int)p.size()) {
            ans.push_back(i - j + 1);
            j = pi[j - 1];
        }
    }
    return ans;
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
