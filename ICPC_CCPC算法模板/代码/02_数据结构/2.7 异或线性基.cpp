#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

struct XorBasis {
    // b[i]：最高有效位为 i 的基向量，0 表示该位置尚无基向量。
    // XOR（Exclusive OR）：按位异或。
    array<u64, 64> b{};
    // 将 x 加入异或线性基；使线性空间维数增加时返回 true，否则返回 false。
    // 会修改基向量，处理完整 64 位无符号范围，时间 O(64)。
    bool insert(u64 x) {
        for (int i = 63; i >= 0; --i) {
            if ((x >> i) & 1) {
                if (!b[i]) {
                    b[i] = x;
                    return true;
                }
                x ^= b[i];
            }
        }
        return false;
    }
    // 判断 x 能否由已插入数的某个子集异或得到；允许空集，因此 0 总可表示，O(64)。
    bool contains(u64 x) const {
        for (int i = 63; i >= 0; --i) {
            if ((x >> i) & 1) {
                x ^= b[i];
            }
        }
        return x == 0;
    }
    // 返回 x 与线性空间中某个值异或后的最大无符号结果；默认 x=0 求最大子集异或和。
    // 不修改线性基，允许选择空集，时间 O(64)。
    u64 maximum(u64 x = 0) const {
        for (int i = 63; i >= 0; --i) {
            x = max(x, x ^ b[i]);
        }
        return x;
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
