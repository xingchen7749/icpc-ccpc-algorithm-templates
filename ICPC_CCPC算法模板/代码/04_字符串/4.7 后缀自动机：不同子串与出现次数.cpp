#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

// SAM（Suffix Automaton）：后缀自动机。
struct SAM {
    struct Node {
        // go[c]：字符 c 对应的转移状态，-1 表示没有转移。
        array<int, 26> go;
        // link：后缀链接，根为 -1；len：该状态代表的最大子串长度。
        int link = -1, len = 0;
        // cnt：构建时记录的末端贡献，新建普通状态为 1、克隆为 0。
        // 实际出现次数由 occurrences 按后缀链接汇总到副本，不能直接读取此字段。
        i64 cnt = 0;
        // 创建自动机状态：所有字符转移设为 -1，其他字段使用默认初值。
        Node() {
            go.fill(-1);
        }
    };
    vector<Node> t{Node()};
    // last：当前整个字符串对应的状态；0 号为根状态。
    int last = 0;
    // 在当前字符串末尾追加一个小写字母 ch，更新自动机、后缀链接和 last。
    // 必要时克隆状态，克隆自身出现次数为 0；固定字符集下整个构建过程为线性时间。
    void extend(char ch) {
        int c = ch - 'a', cur = t.size();
        assert(0 <= c && c < 26);
        t.emplace_back();
        t[cur].len = t[last].len + 1;
        t[cur].cnt = 1;
        int p = last;
        while (p != -1 && t[p].go[c] == -1) {
            t[p].go[c] = cur;
            p = t[p].link;
        }
        if (p == -1) {
            t[cur].link = 0;
        }
        else {
            int q = t[p].go[c];
            if (t[p].len + 1 == t[q].len) {
                t[cur].link = q;
            }
            else {
                int clone = t.size();
                t.push_back(t[q]);
                t[clone].len = t[p].len + 1;
                t[clone].cnt = 0;
                while (p != -1 && t[p].go[c] == q) {
                    t[p].go[c] = clone;
                    p = t[p].link;
                }
                t[q].link = t[cur].link = clone;
            }
        }
        last = cur;
    }
    // 返回当前字符串不同非空子串的数量，累加各非根状态代表的长度区间；时间 O(状态数)。
    i64 distinct() const {
        i64 ans = 0;
        for (int v = 1; v < (int)t.size(); ++v) {
            ans += t[v].len - t[t[v].link].len;
        }
        return ans;
    }
    // 返回每个状态的 endpos 大小，即其代表的非空子串在当前字符串中的出现次数。
    // 用副本按长度降序累加，可重复调用；根状态的值不作为空串出现次数使用。
    // S 个状态耗时 O(S log S)，额外空间 O(S)。
    vector<i64> occurrences() const {
        // ord：状态按 len 从大到小排列的顺序，让较长状态先向后缀链接贡献次数。
        vector<int> ord(t.size());
        iota(ord.begin(), ord.end(), 0);
        // 按状态最大长度降序排序，使出现次数先从较长状态传给较短的后缀链接目标。
        sort(ord.begin(), ord.end(), [&](int a, int b) { return t[a].len > t[b].len; });
        vector<i64> cnt;
        for (auto& v : t) cnt.push_back(v.cnt);
        for (int v : ord) {
            if (t[v].link != -1) {
                cnt[t[v].link] += cnt[v];
            }
        }
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
