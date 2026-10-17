#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

// 01 Trie（Binary Trie）：二进制字典树，支持重复数。
class BinaryTrie {
private:
    struct Node {
        int ch[2] = {};  // 当前位为 0、1 的子节点，0 表示不存在
        int cnt = 0;    // 经过该节点的数的数量，包含重复数
    };

    int max_bit;      // 最高二进制位编号，共处理 max_bit+1 位
    vector<Node> tr;  // tr[0] 占位，tr[1] 为根；删除后保留节点

public:
    // 建立空字典树；最高位编号 max_bit 必须在 0..63 内。
    BinaryTrie(int max_bit = 63) : max_bit(max_bit), tr(2) {
        assert(0 <= max_bit && max_bit <= 63);
    }

    // 清空所有数并恢复根节点，保留设定的最高位编号。
    void clear() {
        tr.assign(2, Node{});
    }

    // 返回当前数的数量，包含重复数。
    int size() const {
        return tr[1].cnt;
    }

    // 返回 x 的出现次数；x 必须在设定位宽内，不存在时返回 0。
    int count(u64 x) const {
        assert(max_bit == 63 || (x >> (max_bit + 1)) == 0);
        int u = 1;
        for (int i = max_bit; i >= 0; i--) {
            int bit = (x >> i) & 1ULL;
            int v = tr[u].ch[bit];
            if (v == 0 || tr[v].cnt == 0) return 0;
            u = v;
        }
        return tr[u].cnt;
    }

    // 插入位宽范围内的 x，允许重复插入。
    void insert(u64 x) {
        assert(max_bit == 63 || (x >> (max_bit + 1)) == 0);
        int u = 1;
        tr[u].cnt++;
        for (int i = max_bit; i >= 0; i--) {
            int bit = (x >> i) & 1ULL;
            if (tr[u].ch[bit] == 0) {
                int v = (int)tr.size();
                tr.emplace_back();
                tr[u].ch[bit] = v;
            }
            u = tr[u].ch[bit];
            tr[u].cnt++;
        }
    }

    // 删除位宽范围内的一个 x；成功返回 true，不存在则返回 false。
    bool erase(u64 x) {
        if (count(x) == 0) return false;
        int u = 1;
        tr[u].cnt--;
        for (int i = max_bit; i >= 0; i--) {
            int bit = (x >> i) & 1ULL;
            u = tr[u].ch[bit];
            tr[u].cnt--;
        }
        return true;
    }

    // 返回集合中各个 y 与 x 异或后的最大值，不是 y 本身。
    // 集合必须非空，x 必须在设定位宽内。
    u64 max_xor(u64 x) const {
        assert(size() > 0);
        assert(max_bit == 63 || (x >> (max_bit + 1)) == 0);
        int u = 1;
        u64 ans = 0;
        for (int i = max_bit; i >= 0; i--) {
            int bit = (x >> i) & 1ULL;
            int v = tr[u].ch[bit ^ 1];
            // 高位优先选择相反位，使异或结果尽量大；跳过已删除的分支。
            if (v != 0 && tr[v].cnt > 0) {
                ans |= 1ULL << i;
                u = v;
            }
            else {
                u = tr[u].ch[bit];
            }
        }
        return ans;
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
