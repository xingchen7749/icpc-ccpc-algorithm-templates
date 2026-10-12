// 自动生成，仅供测试；写题使用独立 cpp。
#pragma once
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
// #define int long long


namespace cp_binary {
// 在候选区间 [lo,hi) 中寻找第一个使 ok(x) 为真的值，不存在则返回原 hi。
// 要求谓词先假后真，且 hi-lo 不溢出；调用 ok 的次数为 O(log(hi-lo+1))。
template <class Pred>
i64 first_true(i64 lo, i64 hi, Pred ok) {
    while (lo < hi) {
        i64 mid = lo + (hi - lo) / 2;
        if (ok(mid)) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return lo;
}
}

namespace cp_compress {
// a[0] is unused; xs[0] is also unused.
// 将 a[1..n] 排序去重，返回带 0 号占位的有序值表 xs；不修改原数组。
// 要求 a 至少包含占位项；时间 O(n log n)，空间 O(n)。
template <class T>
vector<T> compress(const vector<T>& a) {
    vector<T> xs(a.begin() + 1, a.end());
    sort(xs.begin(), xs.end());
    xs.erase(unique(xs.begin(), xs.end()), xs.end());
    xs.insert(xs.begin(), T{});
    return xs;
}
// 返回 x 在有序离散值表 xs[1..m] 中的 lower_bound 位置，作为 1 下标的秩。
// 通常要求 x 已在值表中；否则返回插入位置，可能为 m+1。时间 O(log m)。
template <class T>
int rank_of(const vector<T>& xs, T x) {
    return lower_bound(xs.begin() + 1, xs.end(), x) - xs.begin();
}
}

namespace cp_prefix {
// 由 a[1..n] 构造前缀和 s，s[0]=0；[l,r] 的和为 s[r]-s[l-1]。
// 忽略 a[0]，不修改 a；要求累加不溢出 i64，时间和空间均为 O(n)。
vector<i64> prefix_sum(const vector<i64>& a) {
    vector<i64> s(a.size());
    for (int i = 1; i < (int)a.size(); ++i) {
        s[i] = s[i - 1] + a[i];
    }
    return s; // sum[l,r] = s[r]-s[l-1]
}
}

namespace cp_difference {
// 由 a[1..n] 构造 n+2 项的差分数组，忽略占位 a[0]；不修改 a。
// 输入至少含 a[0]，允许 n=0；末项用于区间更新的边界标记，时间和空间 O(n)。
vector<i64> difference(const vector<i64>& a) {
    assert(!a.empty());
    int n = (int)a.size() - 1;
    vector<i64> diff(n + 2);
    for (int i = 1; i <= n; ++i) {
        diff[i] = a[i] - (i == 1 ? 0 : a[i - 1]);
    }
    return diff;
}
// 在差分数组 diff 上记录闭区间 [l,r] 加 v，不直接还原原数组；l>r 时不操作。
// 有效更新要求 l>=1 且 r+1 在数组内，通常分配 n+2 项；时间 O(1)。
void range_add(vector<i64>& diff, int l, int r, i64 v) {
    if (l > r) return;
    diff[l] += v;
    diff[r + 1] -= v;
}
// 将含 n+2 项的 diff 前缀累加，返回 a[1..n]，a[0]=0；忽略末项边界标记。
// 不修改 diff，可在后续继续更新并再次还原；时间和空间 O(n)。
vector<i64> restore(const vector<i64>& diff) {
    assert(diff.size() >= 2);
    int n = (int)diff.size() - 2;
    vector<i64> a(n + 1);
    for (int i = 1; i <= n; ++i) {
        a[i] = a[i - 1] + diff[i];
    }
    return a;
}
}

namespace cp_window {
// 返回 a[1..n] 每个长度为 k 的窗口最小值，ans[j] 对应 [j,j+k-1]。
// 输入和输出的 0 号项均占位，要求 1<=k<=n；时间 O(n)，队列空间 O(k)。
vector<i64> window_min(const vector<i64>& a, int k) {
    assert(1 <= k && k < (int)a.size());
    deque<int> q;
    vector<i64> ans(1);
    for (int i = 1; i < (int)a.size(); ++i) {
        while (!q.empty() && q.front() <= i - k) {
            q.pop_front();
        }
        while (!q.empty() && a[q.back()] >= a[i]) {
            q.pop_back();
        }
        q.push_back(i);
        if (i >= k) {
            ans.push_back(a[q.front()]);
        }
    }
    return ans;
}
}

namespace cp_hash {
struct CustomHash {
    // 将 64 位整数 x 按 SplitMix64 步骤混合，返回确定性的 64 位散列值；时间 O(1)。
    static u64 mix(u64 x) {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }
    // 为整数键 x 计算带进程内固定随机种子的哈希值，供 unordered_map 等容器调用。
    // 同一进程内相同键得到相同结果；可降低固定碰撞构造的影响，但不保证无碰撞。
    size_t operator()(u64 x) const {
        static const u64 seed = chrono::steady_clock::now().time_since_epoch().count();
        return mix(x + seed);
    }
};
}

namespace cp_dsu {
vector<int> father, rk;

// 初始化编号 1..n 的独立集合，初始秩为 1；时间和空间 O(n)。
void init(int n) {
    father.resize(n + 1);
    rk.assign(n + 1, 1);
    for (int i = 1; i <= n; ++i) {
        father[i] = i;
    }
}

// 返回 id 所在集合的根，并通过递归压缩路径；要求 1<=id<=n。
int find(int id) {
    if (id != father[id]) {
        father[id] = find(father[id]);
    }
    return father[id];
}

// 按秩合并 u、v 所在集合；成功返回 true，原本同集合返回 false。
// 要求顶点为 1..n；配合路径压缩，均摊时间 O(α(n))。
bool merge(int u, int v) {
    int a = find(u), b = find(v);
    if (a == b) return false;
    if (rk[a] > rk[b]) {
        father[b] = a;
    } else if (rk[b] > rk[a]) {
        father[a] = b;
    } else {
        father[b] = a;
        rk[a]++;
    }
    return true;
}
}

namespace cp_rollback {
struct RollbackDSU {
    vector<int> p, sz;
    vector<pair<int, int>> hist;
    // 创建编号 1..n 的独立集合并清空合并历史，时间和空间 O(n)。
    RollbackDSU(int n) : p(n + 1), sz(n + 1, 1) {
        iota(p.begin(), p.end(), 0);
    }
    // 沿父指针返回 x 的代表元，不压缩路径以便回滚；时间 O(log n)。
    int find(int x) {
        while (x != p[x]) {
            x = p[x];
        }
        return x;
    }
    // 返回当前成功合并历史的长度，作为之后 rollback 的快照；不修改状态，O(1)。
    int snapshot() const { return hist.size(); }
    // 按大小合并 a、b 所在集合，并记录可撤销信息；同集合返回 false 且不记历史。
    // 有效合并返回 true；时间 O(log n)。
    bool merge(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        hist.push_back({b, sz[a]});
        p[b] = a;
        sz[a] += sz[b];
        return true;
    }
    // 撤销当前历史末尾的合并，恢复到快照 s；s 必须是当前历史中的有效长度。
    // 每撤销一次成功合并耗时 O(1)，不会恢复已被丢弃的未来快照。
    void rollback(int s) {
        while ((int)hist.size() > s) {
            auto [b, old] = hist.back();
            hist.pop_back();
            sz[p[b]] = old;
            p[b] = b;
        }
    }
};
}

namespace cp_fenwick {
struct Fenwick {
    int n;
    vector<i64> t;
    // 初始化长度为 n、下标为 1..n 的全零树状数组，时间和空间 O(n)。
    Fenwick(int n) : n(n), t(n + 1) {}
    // 将第 x 项增加 v，并更新树状数组；要求 1<=x<=n，时间 O(log n)。
    void add(int x, i64 v) {
        assert(1 <= x && x <= n);
        for (; x <= n; x += x & -x) {
            t[x] += v;
        }
    }
    // 返回闭区间 [1,r] 的元素和；要求 0<=r<=n，r=0 返回 0，时间 O(log n)。
    i64 sum(int r) const {
        i64 s = 0;
        for (; r; r -= r & -r) {
            s += t[r];
        }
        return s;
    }
    // 返回闭区间 [l,r] 的元素和；非空查询要求 1<=l<=r<=n，l>r 返回 0。
    // 不修改数组，时间 O(log n)。
    i64 query(int l, int r) const { return l > r ? 0 : sum(r) - sum(l - 1); }
    // 返回最小的下标 x，使前缀和 sum(x)>=k；将单点值看作频数时即第 k 小的位置。
    // 要求所有单点频数非负且 1<=k<=sum(n)，返回 1 下标，时间 O(log n)。
    int kth(i64 k) const {
        assert(1 <= k && k <= sum(n));
        int x = 0, step = 1;
        while (step <= n / 2) {
            step <<= 1;
        }
        for (; step; step >>= 1) {
            int y = x + step;
            if (y <= n && t[y] < k) {
                x = y;
                k -= t[y];
            }
        }
        return x + 1;
    }
};
}

namespace cp_sparse {
class ST {
private:
    int n;
    vector<vector<int>> st;

public:
    // 由 a[1..n] 构造静态区间最小值表，忽略 a[0]，要求 n>=1。
    // 预处理时间和空间 O(n log n)；原数组修改后需重新构造。
    ST(const vector<int>& a) {
        assert(a.size() >= 2);
        n = (int)a.size() - 1;
        if (n <= 0) return;

        int K = log2((double)n) + 1;
        st.assign(n + 1, vector<int>(K));

        for (int i = 1; i <= n; ++i) {
            st[i][0] = a[i];
        }

        for (int j = 1; j < K; ++j) {
            for (int i = 1; i + (1LL << j) - 1 <= n; ++i) {
                st[i][j] = min(st[i][j - 1], st[i + (1LL << (j - 1))][j - 1]);
            }
        }
    }

    // 1 下标，查询非空闭区间 [l, r] 的最小值
    int query(int l, int r) const {
        assert(1 <= l && l <= r && r <= n);
        int j = log2((double)(r - l + 1));
        return min(st[l][j], st[r - (1LL << j) + 1][j]);
    }
};
}

namespace cp_lazyseg {
class segmenttree {
private:
    int n;
    vector<int> tree, lazy; // 区间和、尚未下传的加法标记

    // 合并两个区间的结果：参数 a、b 为各自区间和，返回合并后的和
    int merge(int a, int b) { return a + b; }

    // 向上合并：通过 merge 用两个子节点的结果更新节点 p
    void pushup(int p) {
        tree[p] = merge(tree[p * 2], tree[p * 2 + 1]);
    }

    // 建树：节点 p 管理闭区间 [l,r]
    void build(const vector<int>& a, int p, int l, int r) {
        if (l == r) {
            tree[p] = a[l];
            return;
        }
        int m = (l + r) / 2;
        build(a, p * 2, l, m);
        build(a, p * 2 + 1, m + 1, r);
        pushup(p);
    }

    // 下传标记：更新孩子的区间和与标记，再清空当前标记
    void pushdown(int p, int l, int r) {
        if (l == r || lazy[p] == 0) return;
        int m = (l + r) / 2;
        tree[p * 2] += lazy[p] * (m - l + 1);
        tree[p * 2 + 1] += lazy[p] * (r - m);
        lazy[p * 2] += lazy[p];
        lazy[p * 2 + 1] += lazy[p];
        lazy[p] = 0;
    }

    // 单点赋值的递归实现：将 a[pos] 改为 v
    void set(int p, int l, int r, int pos, int v) {
        if (l == r) {
            tree[p] = v;
            lazy[p] = 0;
            return;
        }
        pushdown(p, l, r);
        int m = (l + r) / 2;
        if (pos <= m) {
            set(p * 2, l, m, pos, v);
        } else {
            set(p * 2 + 1, m + 1, r, pos, v);
        }
        pushup(p);
    }

    // 区间加的递归实现：将 [ql,qr] 内每个数增加 v
    void add(int p, int l, int r, int ql, int qr, int v) {
        if (ql <= l && r <= qr) {
            tree[p] += v * (r - l + 1);
            lazy[p] += v;
            return;
        }
        pushdown(p, l, r);
        int m = (l + r) / 2;
        if (ql <= m) {
            add(p * 2, l, m, ql, qr, v);
        }
        if (qr > m) {
            add(p * 2 + 1, m + 1, r, ql, qr, v);
        }
        pushup(p);
    }

    // 区间求和的递归实现：完全覆盖直接返回，否则累加子区间答案
    int query(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return tree[p];
        pushdown(p, l, r);
        int m = (l + r) / 2;
        int sum = 0;
        if (ql <= m) {
            sum = merge(sum, query(p * 2, l, m, ql, qr));
        }
        if (qr > m) {
            sum = merge(sum, query(p * 2 + 1, m + 1, r, ql, qr));
        }
        return sum;
    }

public:
    // 构造函数：a[0] 占位，要求 n >= 1，建树 O(n)
    segmenttree(const vector<int>& a) {
        assert(a.size() >= 2); // 至少包含占位和一个有效元素
        n = (int)a.size() - 1;
        tree.assign(4 * n + 5, 0);
        lazy.assign(4 * n + 5, 0);
        build(a, 1, 1, n);
    }

    // 单点赋值：a[pos] = v，要求 1 <= pos <= n，O(log n)
    void set(int pos, int v) {
        assert(1 <= pos && pos <= n);
        set(1, 1, n, pos, v);
    }

    // 区间加：[l,r] 内每个数增加 v，要求 1 <= l <= r <= n，O(log n)
    void add(int l, int r, int v) {
        assert(1 <= l && l <= r && r <= n);
        add(1, 1, n, l, r, v);
    }

    // 区间求和：返回 [l,r] 的和，要求 1 <= l <= r <= n，O(log n)
    int query(int l, int r) {
        assert(1 <= l && l <= r && r <= n);
        return query(1, 1, n, l, r);
    }
};
}

namespace cp_segtree {
class SegTree {
private:
    int n;
    vector<int> tree; // 区间和

    // 合并两个区间的结果：参数 a、b 为各自区间和，返回合并后的和
    int merge(int a, int b) { return a + b; }

    // 向上合并：通过 merge 用两个子节点的结果更新节点 p
    void pushup(int p) {
        tree[p] = merge(tree[p * 2], tree[p * 2 + 1]);
    }

    // 建树：节点 p 管理闭区间 [l,r]
    void build(const vector<int>& a, int p, int l, int r) {
        if (l == r) {
            tree[p] = a[l];
            return;
        }
        int m = (l + r) / 2;
        build(a, p * 2, l, m);
        build(a, p * 2 + 1, m + 1, r);
        pushup(p);
    }

    // 单点赋值的递归实现：将 a[pos] 改为 v
    void set(int p, int l, int r, int pos, int v) {
        if (l == r) {
            tree[p] = v;
            return;
        }
        int m = (l + r) / 2;
        if (pos <= m) {
            set(p * 2, l, m, pos, v);
        } else {
            set(p * 2 + 1, m + 1, r, pos, v);
        }
        pushup(p);
    }

    // 单点加的递归实现：将 a[pos] 增加 v，再向上更新区间和
    void add(int p, int l, int r, int pos, int v) {
        if (l == r) {
            tree[p] += v;
            return;
        }
        int m = (l + r) / 2;
        if (pos <= m) {
            add(p * 2, l, m, pos, v);
        } else {
            add(p * 2 + 1, m + 1, r, pos, v);
        }
        pushup(p);
    }

    // 区间求和的递归实现：完全覆盖直接返回，否则累加子区间答案
    int query(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return tree[p];
        int m = (l + r) / 2;
        int sum = 0;
        if (ql <= m) {
            sum = merge(sum, query(p * 2, l, m, ql, qr));
        }
        if (qr > m) {
            sum = merge(sum, query(p * 2 + 1, m + 1, r, ql, qr));
        }
        return sum;
    }

public:
    // 构造函数：a[0] 占位，要求 n >= 1，建树 O(n)
    SegTree(const vector<int>& a) {
        assert(a.size() >= 2); // 至少包含占位和一个有效元素
        n = (int)a.size() - 1;
        tree.assign(4 * n + 5, 0);
        build(a, 1, 1, n);
    }

    // 单点赋值：a[pos] = v，要求 1 <= pos <= n，O(log n)
    void set(int pos, int v) {
        assert(1 <= pos && pos <= n);
        set(1, 1, n, pos, v);
    }

    // 单点加：a[pos] += v，要求 1 <= pos <= n，O(log n)
    void add(int pos, int v) {
        assert(1 <= pos && pos <= n);
        add(1, 1, n, pos, v);
    }

    // 区间求和：返回 [l,r] 的和，要求 1 <= l <= r <= n，O(log n)
    int query(int l, int r) {
        assert(1 <= l && l <= r && r <= n);
        return query(1, 1, n, l, r);
    }
};
}

namespace cp_persistent {
struct PersistentKth {
    struct Node {
        int l = 0, r = 0, cnt = 0;
    };
    int m;
    vector<Node> t{{}};
    vector<int> root{0};
    // 设置离散值域为 1..m，创建空前缀版本 root[0]，要求 m>0。
    PersistentKth(int m) : m(m) {
        assert(m > 0);
    }
    // 内部持久化插入：复制 old 版本在值域 [l,r) 上通往秩 x 的路径，频数加一。
    // 返回新根编号，保留旧版本；要求 l<=x<r，新增节点数和时间均为 O(log m)。
    int insert(int old, int l, int r, int x) {
        int p = t.size();
        t.push_back(t[old]);
        ++t[p].cnt;
        if (r - l > 1) {
            int mid = (l + r) / 2;
            if (x < mid) {
                t[p].l = insert(t[old].l, l, mid, x);
            } else {
                t[p].r = insert(t[old].r, mid, r, x);
            }
        }
        return p;
    }
    // 向序列末尾追加离散秩 x，生成新的前缀频数版本；要求 1<=x<=m，O(log m)。
    void append(int x) {
        root.push_back(insert(root.back(), 1, m + 1, x));
    }
    // 返回原序列闭区间 [l,r] 的第 k 小离散秩，需再查离散值表取得实际值。
    // 要求 1<=l<=r<=已追加数量、1<=k<=r-l+1；时间 O(log m)。
    int kth(int l, int r, int k) {
        assert(1 <= k && k <= r - l + 1);
        int a = root[l - 1], b = root[r], L = 1, R = m + 1;
        while (R - L > 1) {
            int mid = (L + R) / 2, c = t[t[b].l].cnt - t[t[a].l].cnt;
            if (k <= c) {
                a = t[a].l;
                b = t[b].l;
                R = mid;
            } else {
                k -= c;
                a = t[a].r;
                b = t[b].r;
                L = mid;
            }
        }
        return L;
    }
};
}

namespace cp_basis {
struct XorBasis {
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
}

namespace cp_mo {
// 离线统计 ranges 中各闭区间的不同值个数，答案按查询输入顺序返回，不带占位。
// a 使用 1 下标且值在 1..m；非空查询满足 1<=l<=r<=n，不支持修改。
vector<int> mo_distinct(const vector<int>& a, int m, const vector<pair<int, int>>& ranges) {
    int n = (int)a.size() - 1, q = ranges.size(), B = max(1, (int)sqrt(n));
    vector<int> ord(q), ans(q), cnt(m + 1);
    iota(ord.begin(), ord.end(), 0);
    // 按左端点分块，块内右端点交替升降序排序，减少窗口指针移动；x、y 是查询编号。
    sort(ord.begin(), ord.end(), [&](int x, int y) {
        auto [l, r] = ranges[x];
        auto [L, R] = ranges[y];
        if (l / B != L / B) return l / B < L / B;
        return (l / B & 1) ? r > R : r < R;
    });
    int l = 1, r = 0, cur = 0;
    // 把位置 i 纳入当前窗口，更新其频数；该值首次出现时增加不同值计数 cur。
    auto add = [&](int i) {
        if (cnt[a[i]]++ == 0) {
            ++cur;
        }
    };
    // 把当前窗口内的位置 i 移出，更新其频数；该值不再出现时减少 cur。
    auto del = [&](int i) {
        if (--cnt[a[i]] == 0) {
            --cur;
        }
    };
    for (int id : ord) {
        auto [L, R] = ranges[id];
        while (l > L) {
            add(--l);
        }
        while (r < R) {
            add(++r);
        }
        while (l < L) {
            del(l++);
        }
        while (r > R) {
            del(r--);
        }
        ans[id] = cur;
    }
    return ans;
}
}

namespace cp_treap {
struct ImplicitTreap {
    struct Node {
        int l = 0, r = 0, sz = 1;
        u64 pri = 0;
        i64 val = 0;
        bool rev = false;
    };
    vector<Node> t{{}};
    int root = 0;
    mt19937_64 rng{712367};
    // 返回节点 p 的子树大小；节点编号 0 表示空树，返回 0，时间 O(1)。
    int size(int p) { return p ? t[p].sz : 0; }
    // 内部维护：根据左右孩子重新计算非空节点 p 的子树大小，时间 O(1)。
    void pull(int p) {
        t[p].sz = 1 + size(t[p].l) + size(t[p].r);
    }
    // 为子树 p 应用翻转：交换左右孩子并切换延迟翻转标记；空树不操作，O(1)。
    void flip(int p) {
        if (p) {
            swap(t[p].l, t[p].r);
            t[p].rev ^= 1;
        }
    }
    // 将非空节点 p 的延迟翻转标记下传给孩子，并清除自身标记，时间 O(1)。
    void push(int p) {
        if (t[p].rev) {
            flip(t[p].l);
            flip(t[p].r);
            t[p].rev = 0;
        }
    }
    // 按元素个数把子树 p 分为前 k 项与剩余项，返回两棵树的根；要求 0<=k<=size(p)。
    // 会重接子树并下传标记，期望时间 O(log n)。
    pair<int, int> split(int p, int k) {
        if (!p) return {0, 0};
        push(p);
        if (size(t[p].l) >= k) {
            auto [a, b] = split(t[p].l, k);
            t[p].l = b;
            pull(p);
            return {a, p};
        }
        auto [a, b] = split(t[p].r, k - size(t[p].l) - 1);
        t[p].r = a;
        pull(p);
        return {p, b};
    }
    // 把序列树 a 接在 b 前面合并，返回新根；要求 a 的所有项排在 b 的所有项之前。
    // 会修改树的连接关系，期望时间 O(log n)。
    int merge(int a, int b) {
        if (!a || !b) return a ? a : b;
        if (t[a].pri > t[b].pri) {
            push(a);
            t[a].r = merge(t[a].r, b);
            pull(a);
            return a;
        }
        push(b);
        t[b].l = merge(a, t[b].l);
        pull(b);
        return b;
    }
    // 在当前序列末尾添加值 v，并更新 root；期望时间 O(log n)，新增一个节点。
    void append(i64 v) {
        int p = t.size();
        Node x;
        x.pri = rng();
        x.val = v;
        t.push_back(x);
        root = merge(root, p);
    }
    // 原地翻转序列的闭区间 [l,r]，要求 1<=l<=r<=序列长度；期望时间 O(log n)。
    void reverse(int l, int r) {
        auto [a, c] = split(root, r);
        auto [x, b] = split(a, l - 1);
        flip(b);
        root = merge(merge(x, b), c);
    }
    // 内部中序遍历：把子树 p 的值依次追加到 a，遍历时下传翻转标记。
    // 不清空 a；耗时与子树大小成正比，递归空间与树高成正比。
    void collect(int p, vector<i64>& a) {
        if (!p) return;
        push(p);
        collect(t[p].l, a);
        a.push_back(t[p].val);
        collect(t[p].r, a);
    }
    // 导出当前完整序列，返回 a[1..n]，a[0] 为占位；时间和输出空间 O(n)。
    vector<i64> values() {
        vector<i64> a(1);
        collect(root, a);
        return a;
    }
};
}

namespace cp_dijkstra {
const i64 INF = (1LL << 62);

// 求源点 s 到各点的最短距离，g[u] 存 {终点,边权}，顶点使用 1..n。
// 要求边权非负，返回 d[1..n]，不可达为 INF，有限答案须小于 INF；不修改图。
// 堆实现时间 O((n+m)log(n+m))，空间 O(n+m)。
vector<i64> dijkstra(const vector<vector<pair<int, i64>>>& g, int s) {
    vector<i64> d(g.size(), INF);
    d[s] = 0;
    priority_queue<pair<i64, int>, vector<pair<i64, int>>, greater<pair<i64, int>>> q;
    q.push({0, s});
    while (!q.empty()) {
        auto [du, u] = q.top();
        q.pop();
        if (du != d[u]) continue;
        for (auto [v, w] : g[u]) {
            assert(w >= 0);
            if (w >= INF - du) continue;
            if (d[v] > du + w) {
                d[v] = du + w;
                q.push({d[v], v});
            }
        }
    }
    return d;
}
}

namespace cp_bfs01 {
// 求源点 s 的 0/1 边权最短路；g 使用 1 下标，边记录为 {终点,0或1}。
// 返回各点距离，不可达为 1LL<<62；不修改图，时间 O(n+m)。
vector<i64> bfs01(const vector<vector<pair<int, int>>>& g, int s) {
    const i64 INF = (1LL << 62);
    vector<i64> d(g.size(), INF);
    vector<char> done(g.size());
    deque<int> q;
    d[s] = 0;
    q.push_front(s);
    while (!q.empty()) {
        int u = q.front();
        q.pop_front();
        if (done[u]) continue;
        done[u] = 1;
        for (auto [v, w] : g[u]) {
            assert(w == 0 || w == 1);
            if (d[v] > d[u] + w) {
                d[v] = d[u] + w;
                if (w) {
                    q.push_back(v);
                } else {
                    q.push_front(v);
                }
            }
        }
    }
    return d;
}
}

namespace cp_bellman {
struct Edge {
    int u, v;
    i64 w;
};
// 在 n 个点、边列表 e 中求 s 出发的最短路，并检查可达负环；点编号 1..n。
// 返回 {有无可达负环,距离}，有负环时距离无效，不可达为 i128(1)<<120。
// 允许负权，不修改输入；时间 O(nm)，空间 O(n)。
pair<bool, vector<i128>> bellman_ford(int n, const vector<Edge>& e, int s) {
    const i128 INF = (i128(1) << 120);
    vector<i128> d(n + 1, INF);
    d[s] = 0;
    for (int it = 0; it < n; ++it) {
        bool changed = false;
        for (auto [u, v, w] : e) {
            if (d[u] != INF && d[v] > d[u] + w) {
                d[v] = d[u] + w;
                changed = true;
            }
        }
        if (!changed) return {false, d};
        if (it == n - 1) return {true, d};
    }
    return {false, d};
}
}

namespace cp_topo {
// 对 g[1..n] 做拓扑排序，返回无占位的顶点顺序列表；不修改图。
// 返回长度不足 n 表示存在有向环，此时列表并非全图拓扑序；时间 O(n+m)。
vector<int> topo_sort(const vector<vector<int>>& g) {
    int n = (int)g.size() - 1;
    vector<int> deg(n + 1), ans;
    queue<int> q;
    for (auto& es : g) {
        for (int v : es) {
            ++deg[v];
        }
    }
    for (int i = 1; i <= n; ++i) {
        if (!deg[i]) {
            q.push(i);
        }
    }
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        ans.push_back(u);
        for (int v : g[u]) {
            if (--deg[v] == 0) {
                q.push(v);
            }
        }
    }
    return ans;
}
}

namespace cp_kruskal {
using cp_dsu::init;
using cp_dsu::find;
using cp_dsu::merge;
struct Edge {
    int u, v;
    i64 w;
};
// 使用 2.1 并查集的 init、find、merge；独立文件中已展开依赖。
// 求 n 个顶点的最小生成森林，es 为无向边列表，可有负权、重边、自环。
// 返回 {总权值,选中边数}；选中 n-1 条边才表示整图连通，权值和须在 i64 范围内。
// 要求 n>=1，顶点为 1..n；不修改调用者的 es，会重置全局并查集。
// 时间 O(n + m log(m+1))，空间 O(n+m)。
pair<i64, int> kruskal(int n, vector<Edge> es) {
    init(n);
    // 按边权从小到大比较两条边，供 Kruskal 依次尝试选边。
    sort(es.begin(), es.end(), [](const Edge& a, const Edge& b) { return a.w < b.w; });
    int num = n;
    i64 result = 0;
    for (int i = 0; i < (int)es.size(); ++i) {
        if (!merge(es[i].u, es[i].v)) continue;
        --num;
        result += es[i].w;
        if (num == 1) break;
    }
    return {result, n - num};
}
}

namespace cp_scc {
struct SCC {
    int n, cc = 0;
    vector<vector<int>> g, rg;
    vector<int> comp;
    // 初始化编号 1..n 的有向图和反图；时间和空间 O(n)。
    SCC(int n) : n(n), g(n + 1), rg(n + 1) {}
    // 添加有向边 u→v，同时在反图加入 v→u；要求端点在 1..n，均摊 O(1)。
    void add_edge(int u, int v) {
        g[u].push_back(v);
        rg[v].push_back(u);
    }
    // 用迭代 Kosaraju 重算强连通分量，将数量写入 cc、编号写入 comp[1..n]。
    // 编号为 0..cc-1，跨分量边 u→v 满足 comp[u]<comp[v]；可加边后重新调用。
    // 时间 O(n+m)，无需递归调用栈。
    void build() {
        cc = 0;
        vector<char> vis(n + 1);
        vector<int> order;
        for (int s = 1; s <= n; ++s) {
            if (!vis[s]) {
                vector<pair<int, int>> st{{s, 0}};
                vis[s] = 1;
                while (!st.empty()) {
                    int u = st.back().first;
                    int& i = st.back().second;
                    if (i == (int)g[u].size()) {
                        order.push_back(u);
                        st.pop_back();
                    } else {
                        int v = g[u][i++];
                        if (!vis[v]) {
                            vis[v] = 1;
                            st.push_back({v, 0});
                        }
                    }
                }
            }
        }
        comp.assign(n + 1, -1);
        reverse(order.begin(), order.end());
        for (int s : order) {
            if (comp[s] == -1) {
                vector<int> st{s};
                comp[s] = cc;
                while (!st.empty()) {
                    int u = st.back();
                    st.pop_back();
                    for (int v : rg[u]) {
                        if (comp[v] == -1) {
                            comp[v] = cc;
                            st.push_back(v);
                        }
                    }
                }
                ++cc;
            }
        }
    }
};
}

namespace cp_tarjan {
struct TarjanSCC {
    int n, cc = 0, timer = 0;
    vector<vector<int>> g;
    vector<int> dfn, low, st, comp;
    vector<char> in_stack;
    // 初始化编号 1..n 的有向图；遍历数组在 build 中初始化。
    TarjanSCC(int n) : n(n), g(n + 1) {}
    // 添加有向边 u→v，端点需在 1..n；均摊 O(1)。
    void add_edge(int u, int v) {
        g[u].push_back(v);
    }
    // 内部 Tarjan 遍历：访问尚未访问的 u，维护 dfn、low 和栈，并弹出完整强连通分量。
    // 由 build 初始化状态后调用，可能递归至 O(n) 深度。
    void dfs(int u) {
        dfn[u] = low[u] = ++timer;
        st.push_back(u);
        in_stack[u] = 1;
        for (int v : g[u]) {
            if (!dfn[v]) {
                dfs(v);
                low[u] = min(low[u], low[v]);
            } else if (in_stack[v]) {
                // Only vertices still in the SCC stack can lower low[u].
                low[u] = min(low[u], dfn[v]);
            }
        }
        if (low[u] == dfn[u]) {
            while (true) {
                int v = st.back();
                st.pop_back();
                in_stack[v] = 0;
                comp[v] = cc;
                if (v == u) break;
            }
            ++cc;
        }
    }
    // 清空遍历状态并重算全部强连通分量，写入 comp[1..n] 与数量 cc。
    // 编号为 0..cc-1，跨分量边 u→v 满足 comp[u]>comp[v]；时间 O(n+m)。
    void build() {
        cc = timer = 0;
        st.clear();
        dfn.assign(n + 1, 0);
        low.assign(n + 1, 0);
        comp.assign(n + 1, -1);
        in_stack.assign(n + 1, 0);
        for (int u = 1; u <= n; ++u) {
            if (!dfn[u]) {
                dfs(u);
            }
        }
    }
};
}

namespace cp_twosat {
using cp_scc::SCC;
// Dependency: SCC from the previous entry.
struct TwoSAT {
    int n;
    SCC s;
    vector<int> value;
    // 创建 n 个布尔变量的蕴含图，变量编号 1..n，每个变量对应两个图节点。
    TwoSAT(int n) : n(n), s(2 * n) {}
    // 加入子句 (变量 a 等于 av) 或 (变量 b 等于 bv)，转成两条蕴含边。
    // 变量编号为 1..n；clause(x,v,x,v) 可强制 x=v，均摊 O(1)。
    void clause(int a, bool av, int b, bool bv) {
        int x = 2 * a - 1 + av, y = 2 * b - 1 + bv;
        s.add_edge((x % 2 ? x + 1 : x - 1), y);
        s.add_edge((y % 2 ? y + 1 : y - 1), x);
    }
    // 判断全部子句是否可同时满足；可满足返回 true，并把一组赋值写入 value[1..n]。
    // 返回 false 时不能使用 value 作为解；此处赋值比较配合 Kosaraju 编号，时间 O(n+m)。
    bool solve() {
        s.build();
        value.resize(n + 1);
        for (int i = 1; i <= n; ++i) {
            if (s.comp[2 * i - 1] == s.comp[2 * i]) return false;
            value[i] = s.comp[2 * i - 1] < s.comp[2 * i];
        }
        return true;
    }
};
}

namespace cp_bridges {
struct LowLink {
    int n, timer = 0;
    vector<vector<pair<int, int>>> g;
    vector<int> dfn, low, bridges;
    vector<char> cut;
    // 创建 n 个顶点的无向图，初始化访问时间、low 值和割点标记；点编号为 1..n。
    LowLink(int n) : n(n), g(n + 1), dfn(n + 1), low(n + 1), cut(n + 1) {}
    // 添加无向边 u-v，两个方向使用同一 id；每条物理边需有独立非负 id，以支持重边。
    void add_edge(int u, int v, int id) {
        g[u].push_back({v, id});
        g[v].push_back({u, id});
    }
    // 内部割点/桥搜索：从未访问顶点 u 出发，pe 为父边编号，根使用 -1。
    // 更新 low、cut 和 bridges，按父边 id 跳过反向边；递归深度最多 O(n)。
    void dfs(int u, int pe) {
        dfn[u] = low[u] = ++timer;
        int child = 0;
        for (auto [v, id] : g[u]) {
            if (id == pe) continue;
            if (!dfn[v]) {
                ++child;
                dfs(v, id);
                low[u] = min(low[u], low[v]);
                if (low[v] > dfn[u]) {
                    bridges.push_back(id);
                }
                if (pe != -1 && low[v] >= dfn[u]) {
                    cut[u] = 1;
                }
            } else {
                low[u] = min(low[u], dfn[v]);
            }
        }
        if (pe == -1 && child > 1) {
            cut[u] = 1;
        }
    }
    // 遍历所有连通块，写入割点标记 cut[1..n] 和桥的边编号列表 bridges，时间 O(n+m)。
    // 应在全部边添加完后调用一次；本实现不清空旧状态，修改图后需重建对象。
    void build() {
        for (int i = 1; i <= n; ++i) {
            if (!dfn[i]) {
                dfs(i, -1);
            }
        }
    }
};
}

namespace cp_hld {
struct HLD {
    int n;
    vector<vector<int>> g;
    vector<int> par, dep, sz, heavy, head, pos, rev;
    // 以 root 为根对非空连通无向树 g[1..n] 做迭代树链剖分，构造父亲、深度和 dfs 序。
    // pos/rev 使用 1 下标，子树区间为 [pos[u],pos[u]+sz[u]-1]；时间和空间 O(n)。
    HLD(vector<vector<int>> g, int root = 1)
        : n((int)g.size() - 1), g(move(g)), par(n + 1, -1), dep(n + 1), sz(n + 1, 1),
          heavy(n + 1, -1), head(n + 1), pos(n + 1), rev(n + 1) {
        vector<int> ord{root};
        par[root] = root;
        for (int i = 0; i < n; ++i) {
            int u = ord[i];
            for (int v : this->g[u]) {
                if (v != par[u]) {
                    par[v] = u;
                    dep[v] = dep[u] + 1;
                    ord.push_back(v);
                }
            }
        }
        for (int i = n - 1; i > 0; --i) {
            int u = ord[i], p = par[u];
            sz[p] += sz[u];
            if (heavy[p] == -1 || sz[u] > sz[heavy[p]]) {
                heavy[p] = u;
            }
        }
        int timer = 1;
        vector<pair<int, int>> st{{root, root}};
        while (!st.empty()) {
            auto [u, h] = st.back();
            st.pop_back();
            for (; u != -1; u = heavy[u]) {
                head[u] = h;
                pos[u] = timer;
                rev[timer++] = u;
                for (int v : this->g[u]) {
                    if (v != par[u] && v != heavy[u]) {
                        st.push_back({v, v});
                    }
                }
            }
        }
    }
    // 返回树上顶点 u、v 的最近公共祖先，要求顶点有效且已完成构造；时间 O(log n)。
    int lca(int u, int v) {
        while (head[u] != head[v]) {
            if (dep[head[u]] < dep[head[v]]) swap(u, v);
            u = par[head[u]];
        }
        return dep[u] < dep[v] ? u : v;
    }
    // 将 u 到 v 的点路径拆成若干 dfs 序闭区间，逐段调用 f(l,r)，包含两端与 LCA。
    // 共 O(log n) 段，但段间顺序和方向无保证；适合求和等可交换运算。
    template <class F>
    void path(int u, int v, F f) {
        while (head[u] != head[v]) {
            if (dep[head[u]] < dep[head[v]]) swap(u, v);
            f(pos[head[u]], pos[u]);
            u = par[head[u]];
        }
        if (pos[u] > pos[v]) swap(u, v);
        f(pos[u], pos[v]);
    }
};
}

namespace cp_virtual {
using cp_hld::HLD;
// Dependency: HLD.
// 根据已预处理的 HLD 和关键点列表 v 构造虚树，自动补充必要 LCA 并去重。
// 返回 {节点列表,父到子的虚边列表}，均保存原树的 1 下标编号；空关键点返回空结果。
// 列表不带占位，非空时首节点为虚树根；k 个关键点耗时 O(k log k+k log n)。
pair<vector<int>, vector<pair<int, int>>> virtual_tree(HLD& h, vector<int> v) {
    if (v.empty()) return {{}, {}};
    // 按原树 dfs 序比较顶点 a、b，保证关键点和补入的 LCA 按树上遍历顺序排列。
    auto cmp = [&](int a, int b) { return h.pos[a] < h.pos[b]; };
    sort(v.begin(), v.end(), cmp);
    v.erase(unique(v.begin(), v.end()), v.end());
    int k = v.size();
    for (int i = 1; i < k; ++i) {
        v.push_back(h.lca(v[i - 1], v[i]));
    }
    sort(v.begin(), v.end(), cmp);
    v.erase(unique(v.begin(), v.end()), v.end());
    vector<int> st;
    vector<pair<int, int>> edges;
    for (int u : v) {
        while (!st.empty() &&
               !(h.pos[st.back()] <= h.pos[u] && h.pos[u] < h.pos[st.back()] + h.sz[st.back()])) {
            st.pop_back();
        }
        if (!st.empty()) {
            edges.push_back({st.back(), u});
        }
        st.push_back(u);
    }
    return {v, edges};
}
}

namespace cp_matching {
struct BipartiteMatch {
    int nL, nR, timer = 0;
    vector<vector<int>> g;
    vector<int> mr, seen;
    // 初始化左部 l 个点、右部 r 个点的二分图，两侧各从 1 编号；mr 初值为 -1。
    BipartiteMatch(int l, int r) : nL(l), nR(r), g(l + 1), mr(r + 1, -1), seen(r + 1) {}
    // 内部增广搜索：从左点 u 尝试找到可用右点或重排旧匹配，成功返回 true 并更新 mr。
    // 使用当前 timer 防止本轮重复访问右点，由 solve 为每轮设置新时间戳。
    bool dfs(int u) {
        for (int v : g[u]) {
            if (seen[v] != timer) {
                seen[v] = timer;
                if (mr[v] == -1 || dfs(mr[v])) {
                    mr[v] = u;
                    return true;
                }
            }
        }
        return false;
    }
    // 重置并求最大匹配，返回匹配边数，mr[v] 保存右点 v 匹配的左点，-1 表示未匹配。
    // 调用前把左到右的边放入 g；时间 O(nL*m)。
    int solve() {
        fill(mr.begin(), mr.end(), -1);
        int ans = 0;
        for (int u = 1; u <= nL; ++u) {
            ++timer;
            ans += dfs(u);
        }
        return ans;
    }
};
}

namespace cp_dinic {
const i64 INF = (1LL << 62);

struct Dinic {
    struct E {
        int v, rev;
        i64 cap;
    };
    int n;
    vector<vector<E>> g;
    vector<int> level, it;
    // 创建 n 个顶点的空残量网络及层次、当前弧数组，顶点编号为 1..n。
    Dinic(int n) : n(n), g(n + 1), level(n + 1), it(n + 1) {}
    // 添加容量为 c 的有向边 u→v 和容量为 0 的反向残量边；c>=0，自环直接忽略。
    void add_edge(int u, int v, i64 c) {
        if (u == v) return;
        assert(c >= 0);
        int a = g[u].size(), b = g[v].size();
        g[u].push_back({v, b, c});
        g[v].push_back({u, a, 0});
    }
    // 内部建层：沿正残量边从 s 广搜，更新 level，返回汇点 t 是否可达；时间 O(n+m)。
    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto e : g[u]) {
                if (e.cap > 0 && level[e.v] < 0) {
                    level[e.v] = level[u] + 1;
                    q.push(e.v);
                }
            }
        }
        return level[t] >= 0;
    }
    // 在层次图中从 u 向汇点 t 尝试推送至多 f 流量，返回实际流量并更新正反残量。
    // 使用当前弧 it 跳过已尝试边；要求先 bfs 建层并初始化 it。
    i64 dfs(int u, int t, i64 f) {
        if (u == t) return f;
        for (int& i = it[u]; i < (int)g[u].size(); ++i) {
            E& e = g[u][i];
            if (e.cap > 0 && level[e.v] == level[u] + 1) {
                i64 z = dfs(e.v, t, min(f, e.cap));
                if (z) {
                    e.cap -= z;
                    g[e.v][e.rev].cap += z;
                    return z;
                }
            }
        }
        return 0;
    }
    // 在当前残量网络上增加 s→t 流，返回本次新增流量，最多 limit；要求 s!=t、limit>=0。
    // 会修改残量网络，重复调用只返回新增部分；未被 limit 截断时可用残量可达点求最小割。
    i64 flow(int s, int t, i64 limit = INF) {
        assert(s != t);
        i64 ans = 0;
        while (ans < limit && bfs(s, t)) {
            fill(it.begin(), it.end(), 0);
            while (i64 f = dfs(s, t, limit - ans)) {
                ans += f;
                if (ans == limit) break;
            }
        }
        return ans;
    }
};
}

namespace cp_mcf {
struct MinCostFlow {
    struct E {
        int v, rev;
        i64 cap, cost;
    };
    int n;
    vector<vector<E>> g;
    // 创建 n 个顶点的空费用残量网络，顶点编号为 1..n。
    MinCostFlow(int n) : n(n), g(n + 1) {}
    // 添加容量 cap、单位费用 cost 的有向边，并建立费用取反的残量边。
    // 要求端点有效且不同、cap>=0、cost!=LLONG_MIN；允许负费用。
    void add_edge(int u, int v, i64 cap, i64 cost) {
        assert(u != v && cap >= 0 && cost != LLONG_MIN);
        int a = g[u].size(), b = g[v].size();
        g[u].push_back({v, b, cap, cost});
        g[v].push_back({u, a, 0, -cost});
    }
    // 从 s 向 t 增广至多 limit 单位流，返回 {本次流量,本次总费用}，并修改残量网络。
    // 要求 s!=t、limit>=0，初始网络不能有正容量负费用环；精确需求必须检查流量等于 limit。
    // 每轮用 Bellman-Ford，若增广 A 次，时间 O(A*n*m)，费用用 i128。
    pair<i64, i128> flow(int s, int t, i64 limit) {
        assert(s != t);
        i64 f = 0;
        i128 cost = 0, INF = i128(1) << 120;
        while (f < limit) {
            vector<i128> d(n + 1, INF);
            vector<int> pv(n + 1, -1), pe(n + 1, -1);
            d[s] = 0;
            for (int z = 0; z < n - 1; ++z) {
                bool changed = false;
                for (int u = 1; u <= n; ++u) {
                    if (d[u] != INF) {
                        for (int i = 0; i < (int)g[u].size(); ++i) {
                            auto e = g[u][i];
                            if (e.cap > 0 && d[e.v] > d[u] + e.cost) {
                                d[e.v] = d[u] + e.cost;
                                pv[e.v] = u;
                                pe[e.v] = i;
                                changed = true;
                            }
                        }
                    }
                }
                if (!changed) break;
            }
            if (pv[t] == -1) break;
            i64 add = limit - f;
            for (int v = t; v != s; v = pv[v]) {
                add = min(add, g[pv[v]][pe[v]].cap);
            }
            for (int v = t; v != s; v = pv[v]) {
                E& e = g[pv[v]][pe[v]];
                e.cap -= add;
                g[v][e.rev].cap += add;
            }
            f += add;
            cost += i128(add) * d[t];
        }
        return {f, cost};
    }
};
}

namespace cp_tree_point_diff {
using cp_hld::HLD;
// 依赖手册 3.7 的 HLD（树链剖分、LCA 与子树区间），独立文件已展开。
class TreePointDiff {
private:
    HLD h;
    vector<i64> diff;

public:
    // 为非空连通无向树 g[1..n] 初始化点差分，根为 root，初始增量全为 0。
    // 要求 1<=root<=n；使用迭代预处理，时间和空间 O(n)。
    TreePointDiff(const vector<vector<int>>& g, int root = 1) : h(g, root), diff(g.size(), 0) {}

    // 将 u 到 v 路径上的所有点加 val，包含两端；时间 O(log n)。
    void add(int u, int v, i64 val) {
        assert(1 <= u && u <= h.n && 1 <= v && v <= h.n);
        int p = h.lca(u, v);
        diff[u] += val;
        diff[v] += val;
        diff[p] -= val;
        if (h.par[p] != p) {
            diff[h.par[p]] -= val;
        }
    }

    // 返回各点累计增量 ans[1..n]，ans[0]=0；时间和空间 O(n)。
    // 按 dfs 序逆序向父亲累加，不修改 diff，可重复调用并继续添加操作。
    vector<i64> result() const {
        vector<i64> ans = diff;
        for (int i = h.n; i > 1; --i) {
            int u = h.rev[i];
            ans[h.par[u]] += ans[u];
        }
        return ans;
    }
};
}

namespace cp_tree_edge_diff {
using cp_hld::HLD;
// 依赖手册 3.7 的 HLD（树链剖分、LCA 与子树区间），独立文件已展开。
class TreeEdgeDiff {
private:
    HLD h;
    vector<i64> diff;

public:
    // 为非空连通无向树 g[1..n] 初始化边差分，根为 root，初始增量全为 0。
    // 要求 1<=root<=n；使用迭代预处理，时间和空间 O(n)。
    TreeEdgeDiff(const vector<vector<int>>& g, int root = 1) : h(g, root), diff(g.size(), 0) {}

    // 将 u 到 v 路径上的所有边加 val；u==v 时无边被修改，时间 O(log n)。
    void add(int u, int v, i64 val) {
        assert(1 <= u && u <= h.n && 1 <= v && v <= h.n);
        int p = h.lca(u, v);
        diff[u] += val;
        diff[v] += val;
        diff[p] -= val;
        diff[p] -= val;
    }

    // 返回各边累计增量，ans[u] 对应父节点到 u 的边，根及 ans[0] 为 0。
    // 逆序向父亲累加，不修改 diff，可重复调用并继续更新；时间和空间 O(n)。
    vector<i64> result() const {
        vector<i64> ans = diff;
        for (int i = h.n; i > 1; --i) {
            int u = h.rev[i];
            ans[h.par[u]] += ans[u];
        }
        ans[h.rev[1]] = 0;
        return ans;
    }
};
}

namespace cp_kmp {
// 返回前缀函数 pi，pi[i] 是 s[0..i] 的最长真前缀与后缀的公共长度。
// 字符串和返回数组均为 0 下标，空串返回空数组；时间和空间 O(|s|)。
vector<int> prefix_function(const string& s) {
    vector<int> pi(s.size());
    for (int i = 1; i < (int)s.size(); ++i) {
        int j = pi[i - 1];
        while (j && s[i] != s[j]) {
            j = pi[j - 1];
        }
        if (s[i] == s[j]) {
            ++j;
        }
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
        if (text[i] == p[j]) {
            ++j;
        }
        if (j == (int)p.size()) {
            ans.push_back(i - j + 1);
            j = pi[j - 1];
        }
    }
    return ans;
}
}

namespace cp_zfunc {
// 返回 z[i]=s 与后缀 s[i..] 的最长公共前缀长度，位置从 0 开始。
// 约定非空串 z[0]=|s|，空串返回空数组；时间和空间 O(|s|)。
vector<int> z_function(const string& s) {
    int n = s.size();
    vector<int> z(n);
    if (n) {
        z[0] = n;
    }
    for (int i = 1, l = 0, r = 0; i < n; ++i) {
        if (i < r) {
            z[i] = min(r - i, z[i - l]);
        }
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
            ++z[i];
        }
        if (i + z[i] > r) {
            l = i;
            r = i + z[i];
        }
    }
    return z;
}
}

namespace cp_manacher {
// 返回 {奇回文半径 d1,偶回文半径 d2}，均为 0 下标。
// d1[i] 含中心 i，回文长度为 2*d1[i]-1；d2[i] 的中心在 i-1 与 i 之间，长度为 2*d2[i]。
// 空串返回两个空数组；时间和空间 O(|s|)。
pair<vector<int>, vector<int>> manacher(const string& s) {
    int n = s.size();
    vector<int> d1(n), d2(n);
    for (int i = 0, l = 0, r = -1; i < n; ++i) {
        int k = i > r ? 1 : min(d1[l + r - i], r - i + 1);
        while (i - k >= 0 && i + k < n && s[i - k] == s[i + k]) {
            ++k;
        }
        d1[i] = k--;
        if (i + k > r) {
            l = i - k;
            r = i + k;
        }
    }
    for (int i = 0, l = 0, r = -1; i < n; ++i) {
        int k = i > r ? 0 : min(d2[l + r - i + 1], r - i + 1);
        while (i - k - 1 >= 0 && i + k < n && s[i - k - 1] == s[i + k]) {
            ++k;
        }
        d2[i] = k--;
        if (i + k > r) {
            l = i - k - 1;
            r = i + k;
        }
    }
    return {d1, d2};
}
}

namespace cp_strhash {
struct StringHash {
    static constexpr i64 M1 = 1000000007, M2 = 1000000009, B = 911382323;
    vector<i64> h1, h2, p1, p2;
    // 预处理字符串 s 的双模前缀哈希和幂表，字符串位置为 0 下标；时间和空间 O(|s|)。
    StringHash(const string& s)
        : h1(s.size() + 1), h2(s.size() + 1), p1(s.size() + 1, 1), p2(s.size() + 1, 1) {
        for (int i = 0; i < (int)s.size(); ++i) {
            int x = (unsigned char)s[i] + 1;
            h1[i + 1] = (h1[i] * B + x) % M1;
            h2[i + 1] = (h2[i] * B + x) % M2;
            p1[i + 1] = p1[i] * B % M1;
            p2[i + 1] = p2[i] * B % M2;
        }
    }
    // 返回子串 s[l..r) 的两个模数下哈希，要求 0<=l<=r<=|s|；时间 O(1)。
    // 比较子串时需同时比较长度；即使双哈希相等，也仍存在碰撞可能。
    pair<i64, i64> get(int l, int r) const {
        return {(h1[r] - h1[l] * p1[r - l] % M1 + M1) % M1,
                (h2[r] - h2[l] * p2[r - l] % M2 + M2) % M2};
    }
};
}

namespace cp_aho {
struct Aho {
    struct Node {
        array<int, 26> go{};
        int fail = 0;
    };
    vector<Node> t{{}};
    vector<int> order;
    // 插入非空小写模式串 s，返回其终止节点编号；重复模式可返回同一节点。
    // 只能在 build 前调用，时间 O(|s|)。
    int insert(const string& s) {
        assert(!s.empty());
        int u = 0;
        for (char ch : s) {
            int c = ch - 'a';
            assert(0 <= c && c < 26);
            if (!t[u].go[c]) {
                t[u].go[c] = t.size();
                t.push_back({});
            }
            u = t[u].go[c];
        }
        return u;
    }
    // 在全部模式插入后构造 fail 指针并补全字符转移，记录 BFS 顺序 order。
    // 本实现应只调用一次，之后不能继续 insert；S 个节点耗时 O(26S)。
    void build() {
        queue<int> q;
        for (int v : t[0].go) {
            if (v) {
                q.push(v);
            }
        }
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            order.push_back(u);
            for (int c = 0; c < 26; ++c) {
                int v = t[u].go[c];
                if (v) {
                    t[v].fail = t[t[u].fail].go[c];
                    q.push(v);
                } else {
                    t[u].go[c] = t[t[u].fail].go[c];
                }
            }
        }
    }
    // 统计小写文本 s 中各自动机节点所代表模式的出现次数，允许重叠。
    // 需先 build；返回值按节点编号索引，模式答案取其 insert 返回的终止节点。
    // 时间 O(|s|+S)，不修改自动机，可以对不同文本重复调用。
    vector<i64> count(const string& s) const {
        vector<i64> cnt(t.size());
        int u = 0;
        for (char ch : s) {
            u = t[u].go[ch - 'a'];
            ++cnt[u];
        }
        for (int i = (int)order.size() - 1; i >= 0; --i) {
            int v = order[i];
            cnt[t[v].fail] += cnt[v];
        }
        return cnt;
    }
};
}

namespace cp_suffixarray {
struct SuffixArray {
    vector<int> sa, rk, lcp;
    // 构造字符串 s 的后缀排序 sa、逆排名 rk 和相邻后缀公共前缀长度 lcp。
    // 均用 0 下标，lcp[0]=0，支持空串；比较排序倍增耗时 O(n log²n)，空间 O(n)。
    SuffixArray(const string& s) {
        int n = s.size();
        sa.resize(n);
        rk.resize(n);
        lcp.assign(n, 0);
        iota(sa.begin(), sa.end(), 0);
        for (int i = 0; i < n; ++i) {
            rk[i] = (unsigned char)s[i];
        }
        vector<int> tmp(n);
        for (int k = 1; k < n; k *= 2) {
            // 读取当前轮位置 i 的排名，超过字符串末尾时返回 -1，作为缺失后半段的最小排名。
            auto val = [&](int i) { return i < n ? rk[i] : -1; };
            // 比较后缀 a、b 的前后两段排名，每段长 k；供当前倍增轮排序与判断新排名。
            auto cmp = [&](int a, int b) {
                return pair{rk[a], val(a + k)} < pair{rk[b], val(b + k)};
            };
            sort(sa.begin(), sa.end(), cmp);
            tmp[sa[0]] = 0;
            for (int i = 1; i < n; ++i) {
                tmp[sa[i]] = tmp[sa[i - 1]] + cmp(sa[i - 1], sa[i]);
            }
            rk = tmp;
            if (rk[sa.back()] == n - 1) break;
        }
        for (int i = 0; i < n; ++i) {
            rk[sa[i]] = i;
        }
        for (int i = 0, h = 0; i < n; ++i) {
            if (rk[i] == 0) {
                h = 0;
                continue;
            }
            int j = sa[rk[i] - 1];
            while (i + h < n && j + h < n && s[i + h] == s[j + h]) {
                ++h;
            }
            lcp[rk[i]] = h;
            if (h) {
                --h;
            }
        }
    }
};
}

namespace cp_sam {
struct SAM {
    struct Node {
        array<int, 26> go;
        int link = -1, len = 0;
        i64 cnt = 0;
        // 创建自动机状态：所有字符转移设为 -1，其他字段使用默认初值。
        Node() {
            go.fill(-1);
        }
    };
    vector<Node> t{Node()};
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
        } else {
            int q = t[p].go[c];
            if (t[p].len + 1 == t[q].len) {
                t[cur].link = q;
            } else {
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
        vector<int> ord(t.size());
        iota(ord.begin(), ord.end(), 0);
        // 按状态最大长度降序排序，使出现次数先从较长状态传给较短的后缀链接目标。
        sort(ord.begin(), ord.end(), [&](int a, int b) { return t[a].len > t[b].len; });
        vector<i64> cnt;
        for (auto& v : t) {
            cnt.push_back(v.cnt);
        }
        for (int v : ord) {
            if (t[v].link != -1) {
                cnt[t[v].link] += cnt[v];
            }
        }
        return cnt;
    }
};
}

namespace cp_booth {
// 返回字典序最小循环移位的 0 下标起点；多个最优起点取最小者，空串返回 0。
// 返回位置而非字符串；时间 O(n)，复制双倍串使用 O(n) 空间。
int min_rotation(const string& s) {
    int n = s.size();
    if (!n) return 0;
    string t = s + s;
    int i = 0, j = 1, k = 0;
    while (i < n && j < n && k < n) {
        if (t[i + k] == t[j + k]) {
            ++k;
            continue;
        }
        if (t[i + k] > t[j + k]) {
            i += k + 1;
            if (i == j) {
                ++i;
            }
        } else {
            j += k + 1;
            if (i == j) {
                ++j;
            }
        }
        k = 0;
    }
    return min(i, j);
}
}

namespace cp_qpow {
// 快速幂：计算 a 的 b 次方对 mod 取模，返回 [0,mod-1] 内的余数。
// a 可为负，b 为非负指数，要求 mod>0；b=0 返回 1%mod，mod=1 返回 0。
// 用 i128 进行乘法以避免中间溢出；时间 O(log(b+1))，额外空间 O(1)。
i64 qpow(i64 a, u64 b, i64 mod) {
    a %= mod;
    if (a < 0) a += mod;

    i64 res = 1 % mod;
    while (b) {
        if (b & 1) res = i128(res) * a % mod;
        a = i128(a) * a % mod;
        b >>= 1;
    }
    return res;
}
}

namespace cp_fermat {
using cp_qpow::qpow;
// 依赖 qpow：见 5.1「快速幂」（代码/05_数学/5.1 快速幂.cpp）。
// 要求 mod 是素数，且 a % mod != 0。
// 用费马小定理返回 a 在模 mod 下的逆元，结果在 [0,mod-1]。
// 要求 mod 是大于 1 的素数且 a 不被 mod 整除；本函数不检查素性。
// 支持负 a，依赖 qpow；时间 O(log mod)，空间 O(1)。
i64 inverse_fermat(i64 a, i64 mod) {
    assert(mod > 1 && a % mod != 0);
    return qpow(a, mod - 2, mod);
}
}

namespace cp_modular {
// 将整数 a 规范为模 m 的非负余数，返回 [0,m-1] 内的值；要求 m>0，时间 O(1)。
i64 norm(i64 a, i64 m) {
    a %= m;
    return a < 0 ? a + m : a;
}
// 扩展欧几里得：返回 gcd(a,b)，并通过引用写入满足 a*x+b*y=gcd(a,b) 的系数。
// 要求 a、b 非负；系数用 i128 保存，递归耗时与空间为对数级。
i64 exgcd(i64 a, i64 b, i128& x, i128& y) {
    if (!b) {
        x = 1;
        y = 0;
        return a;
    }
    i128 u, v;
    i64 g = exgcd(b, a % b, u, v);
    x = v;
    y = u - i128(a / b) * v;
    return g;
}
// 返回 a 在模 m 下的逆元，要求 m>1，支持合数模数和负 a。
// 若 gcd(a,m)!=1 则返回 nullopt，否则返回 [0,m-1] 内的逆元；时间 O(log m)。
optional<i64> inverse(i64 a, i64 m) {
    assert(m > 1);
    i128 x, y;
    if (exgcd(norm(a, m), m, x, y) != 1) return nullopt;
    x %= m;
    if (x < 0) {
        x += m;
    }
    return (i64)x;
}
}

namespace cp_sieve {
struct Sieve {
    vector<int> primes, lp, phi, mu;
    // 在线性时间内筛出 1..n 的素数、最小质因子 lp、欧拉函数 phi 和莫比乌斯函数 mu。
    // 要求 n>=1；phi[1]=mu[1]=1，lp[1]=0；时间和空间 O(n)。
    Sieve(int n) : lp(n + 1), phi(n + 1), mu(n + 1) {
        assert(n >= 1);
        phi[1] = mu[1] = 1;
        for (int i = 2; i <= n; ++i) {
            if (!lp[i]) {
                lp[i] = i;
                primes.push_back(i);
                phi[i] = i - 1;
                mu[i] = -1;
            }
            for (int p : primes) {
                if (1LL * i * p > n) break;
                int x = i * p;
                lp[x] = p;
                if (i % p == 0) {
                    phi[x] = phi[i] * p;
                    mu[x] = 0;
                    break;
                }
                phi[x] = phi[i] * (p - 1);
                mu[x] = -mu[i];
            }
        }
    }
    // 判断 x 是否为素数，要求 0<=x<=n；调用：s.is_prime(x)。
    bool is_prime(int x) const {
        assert(0 <= x && x < (int)lp.size());
        return x >= 2 && lp[x] == x;
    }
};
}

namespace cp_comb {
using cp_qpow::qpow;
// 依赖 qpow：见 5.1「快速幂」（代码/05_数学/5.1 快速幂.cpp），复制本节时一并复制。
struct Comb {
    i64 p;
    vector<i64> fac, ifac;
    // 预处理 0..n 的阶乘和逆阶乘，用于模素数 p 的组合数查询。
    // 要求 0<=n<p 且 p 是素数（不检查素性）；时间 O(n+log p)，空间 O(n)。
    Comb(int n, i64 p) : p(p), fac(n + 1, 1), ifac(n + 1, 1) {
        assert(n < p && p >= 2);
        for (int i = 1; i <= n; ++i) {
            fac[i] = i128(fac[i - 1]) * i % p;
        }
        ifac[n] = qpow(fac[n], p - 2, p);
        for (int i = n; i > 0; --i) {
            ifac[i - 1] = i128(ifac[i]) * i % p;
        }
    }
    // 返回 C(n,k)%p；k<0 或 k>n 返回 0，有效查询要求 0<=n<=预处理上限。
    // 使用阶乘和逆阶乘，单次时间 O(1)。
    i64 C(int n, int k) const {
        if (k < 0 || k > n) return 0;
        assert(n < (int)fac.size());
        return i128(fac[n]) * ifac[k] % p * ifac[n - k] % p;
    }
};
}

namespace cp_lucas {
using cp_comb::Comb;
struct Lucas : Comb {
    // 预处理完整的 0..p-1 阶乘表；p 必须是可承受 O(p) 时空开销的素数。
    Lucas(int p) : Comb(p - 1, p) {}
    // 用 Lucas 定理返回大参数 C(n,k)%p，要求构造时完整预处理到 p-1。
    // p 必须为素数；逐位处理 p 进制，时间 O(log_p(max(n,k)+1))，k>n 返回 0。
    i64 lucas(u64 n, u64 k) const {
        assert((u64)fac.size() == (u64)p);
        i64 r = 1;
        while (n || k) {
            r = i128(r) * C(n % p, k % p) % p;
            n /= p;
            k /= p;
        }
        return r;
    }
};
}

namespace cp_crt {
using cp_modular::exgcd;
// 依赖 exgcd：见 5.2.2「扩展欧几里得求逆元」（代码/05_数学/5.2.2 扩展欧几里得求逆元.cpp）。
struct CRTResult {
    int status;
    i64 r, m;
};
// 合并 x≡a(mod m) 与 x≡b(mod n)，要求 m,n>0，允许模数不互质。
// 返回 {status,r,m}：0 表示无解，1 表示成功，2 表示合并模数超出 i64。
// 成功时解为 x≡r(mod 返回模数)，且 0<=r<返回模数；依赖 exgcd，耗时为对数级。
CRTResult crt_merge(i64 a, i64 m, i64 b, i64 n) {
    assert(m > 0 && n > 0);
    a %= m;
    if (a < 0) {
        a += m;
    }
    b %= n;
    if (b < 0) {
        b += n;
    }
    i128 x, y;
    i64 g = exgcd(m, n, x, y);
    i128 diff = i128(b) - a;
    if (diff % g) return {0, 0, 0};
    i128 mod = n / g;
    i128 t = (diff / g * x) % mod;
    if (t < 0) {
        t += mod;
    }
    i128 lcm = i128(m / g) * n;
    if (lcm > LLONG_MAX) return {2, 0, 0};
    i128 r = (i128(a) + i128(m) * t) % lcm;
    return {1, (i64)r, (i64)lcm};
}
}

namespace cp_primality {
using u128 = __uint128_t;
// 返回 (a*b)%m，要求 m>0；使用 u128 保存乘积，支持完整 u64 范围，O(1)。
u64 mul(u64 a, u64 b, u64 m) { return u128(a) * b % m; }
// 完整 u64 模数专用快速幂；5.1「快速幂」的 mod 为 i64，不能覆盖这里的范围。
// 内部模快速幂：用于素性测试，返回 a^b%m；调用场景要求 m>1。
// 模数支持完整 u64 范围，乘法通过 mul 防溢出；时间 O(log(b+1))。
u64 pw(u64 a, u64 b, u64 m) {
    u64 r = 1;
    for (; b; b >>= 1, a = mul(a, a, m)) {
        if (b & 1) {
            r = mul(r, a, m);
        }
    }
    return r;
}
// 判断无符号 64 位整数 n 是否为素数，n<2 返回 false。
// 使用覆盖完整 u64 范围的确定性 Miller-Rabin 底数，固定轮数下耗时 O(log n) 次模乘。
bool is_prime(u64 n) {
    if (n < 2) return false;
    for (u64 p : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}) {
        if (n % p == 0) return n == p;
    }
    u64 d = n - 1;
    int s = 0;
    while (!(d & 1)) {
        d >>= 1;
        ++s;
    }
    for (u64 a : {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL}) {
        if (a % n == 0) continue;
        u64 x = pw(a % n, d, n);
        if (x == 1 || x == n - 1) continue;
        bool ok = false;
        for (int r = 1; r < s; ++r) {
            x = mul(x, x, n);
            if (x == n - 1) {
                ok = true;
                break;
            }
        }
        if (!ok) return false;
    }
    return true;
}
}

namespace cp_factor {
using cp_primality::u128;
using cp_primality::mul;
using cp_primality::pw;
using cp_primality::is_prime;
// Pollard-Rho 随机寻找并返回 n 的一个非平凡因子，要求 n 是合数且 n>=4。
// 偶数直接返回 2；奇数失败时重新尝试，无确定的最坏运行时间保证。
u64 rho(u64 n) {
    if (n % 2 == 0) return 2;
    static mt19937_64 rng(712367821);
    while (true) {
        u64 c = rng() % (n - 1) + 1, x = rng() % n, y = x, d = 1;
        // 当前 Rho 轮的迭代函数：返回 (v²+c)%n，用 128 位中间量避免平方和加法溢出。
        auto f = [&](u64 v) { return (u64)((u128(mul(v, v, n)) + c) % n); };
        while (d == 1) {
            x = f(x);
            y = f(f(y));
            d = gcd(x > y ? x - y : y - x, n);
        }
        if (d != n) return d;
    }
}
// 把 n 的所有质因子（含重数）追加到 out，要求 n>=1；n=1 不添加任何元素。
// 不清空、不排序 out；先判断素数，再用 Rho 分治，运行时间具有随机性。
void factor(u64 n, vector<u64>& out) {
    assert(n >= 1);
    if (n == 1) return;
    if (is_prime(n)) {
        out.push_back(n);
        return;
    }
    u64 d = rho(n);
    factor(d, out);
    factor(n / d, out);
}
}

namespace cp_quotient {
// 计算 Σ(i=1..n) floor(n/i)，按商相同的最大连续区间合并贡献。
// 要求 n>=0，n=0 返回 0；结果用 i128，时间 O(sqrt(n))，额外空间 O(1)。
i128 quotient_sum(i64 n) {
    assert(n >= 0);
    i128 ans = 0;
    for (i64 l = 1; l <= n;) {
        i64 q = n / l, r = n / q;
        ans += i128(r - l + 1) * q;
        if (r == n) break;
        l = r + 1;
    }
    return ans;
}
}

namespace cp_ntt {
using cp_qpow::qpow;
const int MOD = 998244353;
// 依赖 qpow：见 5.1「快速幂」（代码/05_数学/5.1 快速幂.cpp），复制本节时一并复制。
// 对系数数组 a 原地做 NTT；inv=false 为正变换，true 为逆变换并乘长度逆元。
// 要求长度是 1..2^23 内的二次幂，系数在 [0,998244353)；时间 O(n log n)。
void ntt(vector<int>& a, bool inv) {
    int n = a.size();
    assert(n > 0 && (n & (n - 1)) == 0 && n <= (1 << 23));
    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) {
            j ^= bit;
        }
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
        int wlen = qpow(3, (MOD - 1) / len, MOD);
        if (inv) {
            wlen = qpow(wlen, MOD - 2, MOD);
        }
        for (int i = 0; i < n; i += len) {
            int w = 1;
            for (int j = 0; j < len / 2; ++j) {
                int u = a[i + j], v = 1LL * a[i + j + len / 2] * w % MOD;
                a[i + j] = u + v;
                if (a[i + j] >= MOD) {
                    a[i + j] -= MOD;
                }
                a[i + j + len / 2] = u - v;
                if (a[i + j + len / 2] < 0) {
                    a[i + j + len / 2] += MOD;
                }
                w = 1LL * w * wlen % MOD;
            }
        }
    }
    if (inv) {
        int ni = qpow(n, MOD - 2, MOD);
        for (int& x : a) {
            x = 1LL * x * ni % MOD;
        }
    }
}
// 返回多项式 a、b 在模 998244353 下的普通卷积，系数按次数从 0 编号。
// 输入系数需已在 [0,MOD)，任一为空返回空数组；补齐后的变换长度 L<=2^23。
// 返回长度 |a|+|b|-1，不修改调用者的数组；时间 O(L log L)，空间 O(L)。
vector<int> convolution(vector<int> a, vector<int> b) {
    if (a.empty() || b.empty()) return {};
    int need = a.size() + b.size() - 1, n = 1;
    while (n < need) {
        n <<= 1;
    }
    assert(n <= (1 << 23));
    a.resize(n);
    b.resize(n);
    ntt(a, false);
    ntt(b, false);
    for (int i = 0; i < n; ++i) {
        a[i] = 1LL * a[i] * b[i] % MOD;
    }
    ntt(a, true);
    a.resize(need);
    return a;
}
}

namespace cp_gauss {
// 对增广矩阵 a[1..n][1..m+1] 做实数高斯消元，末列为常数项，0 行/列占位。
// 返回 {状态,x}：0 无解、1 唯一解、2 无穷多解；有解时 x[1..m] 给出自由变量取 0 的一组解。
// 要求 n>0、各行尺寸一致、eps>0；矩阵按值传入，时间 O(n*m*min(n,m))。
pair<int, vector<long double>> gauss(vector<vector<long double>> a, long double eps = 1e-12L) {
    int n = (int)a.size() - 1;
    assert(n > 0);
    int m = (int)a[1].size() - 2, row = 1;
    vector<int> where(m + 1, 0);
    for (int col = 1; col <= m && row <= n; ++col) {
        int sel = row;
        for (int i = row; i <= n; ++i) {
            if (fabsl(a[i][col]) > fabsl(a[sel][col])) {
                sel = i;
            }
        }
        if (fabsl(a[sel][col]) < eps) continue;
        swap(a[sel], a[row]);
        where[col] = row;
        long double d = a[row][col];
        for (int j = col; j <= m + 1; ++j) {
            a[row][j] /= d;
        }
        for (int i = 1; i <= n; ++i) {
            if (i != row) {
                long double f = a[i][col];
                for (int j = col; j <= m + 1; ++j) {
                    a[i][j] -= f * a[row][j];
                }
            }
        }
        ++row;
    }
    vector<long double> x(m + 1);
    for (int j = 1; j <= m; ++j) {
        if (where[j]) {
            x[j] = a[where[j]][m + 1];
        }
    }
    for (int i = 1; i <= n; ++i) {
        long double v = 0;
        for (int j = 1; j <= m; ++j) {
            v += a[i][j] * x[j];
        }
        if (fabsl(v - a[i][m + 1]) > eps) return {0, {}};
    }
    return {row == m + 1 ? 1 : 2, x};
}
}

namespace cp_matrix {
using Matrix = vector<vector<i64>>;
// 返回同阶方阵 a*b 对 mod 取模的结果；行列为 1..n，0 行/列占位。
// 要求 mod>0、元素在 [0,mod)，输入尺寸一致；不修改输入，时间 O(n³)，输出空间 O(n²)。
Matrix multiply(const Matrix& a, const Matrix& b, i64 mod) {
    int n = (int)a.size() - 1;
    Matrix c(n + 1, vector<i64>(n + 1));
    for (int i = 1; i <= n; ++i) {
        for (int k = 1; k <= n; ++k) {
            if (a[i][k]) {
                for (int j = 1; j <= n; ++j) {
                    c[i][j] = (i128(c[i][j]) + i128(a[i][k]) * b[k][j]) % mod;
                }
            }
        }
    }
    return c;
}
// 返回方阵 a 的非负整数 e 次幂模 mod；行列使用 1 下标，元素需在 [0,mod)。
// 要求 mod>0；e=0 返回模意义的单位矩阵，时间 O(n³log(e+1))，空间 O(n²)。
Matrix matrix_power(Matrix a, u64 e, i64 mod) {
    assert(mod > 0);
    int n = (int)a.size() - 1;
    Matrix r(n + 1, vector<i64>(n + 1));
    for (int i = 1; i <= n; ++i) {
        r[i][i] = 1 % mod;
    }
    for (; e; e >>= 1, a = multiply(a, a, mod)) {
        if (e & 1) {
            r = multiply(r, a, mod);
        }
    }
    return r;
}
}

namespace cp_recurrence {
// 已知 f[t]=c[0]f[t-1]+...+c[k-1]f[t-k]，返回 f[n]%mod。
// init[0..k-1] 为前 k 项，c 长度相同且 k>0；要求 mod>0，初值和系数已在 [0,mod)。
// 用多项式约简求第 n 项，时间 O(k²log(n+1))，空间 O(k)。
i64 linear_nth(const vector<i64>& init, const vector<i64>& c, u64 n, i64 mod) {
    int k = c.size();
    assert(k > 0 && init.size() == c.size() && mod > 0);
    // 内部合并：将次数小于 k 的多项式 a、b 相乘，再按递推特征关系约简并取模。
    // 返回长度 k 的系数数组，不修改输入；时间 O(k²)。
    auto combine = [&](const vector<i64>& a, const vector<i64>& b) {
        vector<i64> t(2 * k - 1);
        for (int i = 0; i < k; ++i) {
            for (int j = 0; j < k; ++j) {
                t[i + j] = (i128(t[i + j]) + i128(a[i]) * b[j]) % mod;
            }
        }
        for (int i = 2 * k - 2; i >= k; --i) {
            for (int j = 1; j <= k; ++j) {
                t[i - j] = (i128(t[i - j]) + i128(t[i]) * c[j - 1]) % mod;
            }
        }
        t.resize(k);
        return t;
    };
    vector<i64> r(k), x(k);
    r[0] = 1 % mod;
    if (k == 1) {
        x[0] = c[0];
    } else {
        x[1] = 1 % mod;
    }
    for (; n; n >>= 1, x = combine(x, x)) {
        if (n & 1) {
            r = combine(r, x);
        }
    }
    i64 ans = 0;
    for (int i = 0; i < k; ++i) {
        ans = (i128(ans) + i128(r[i]) * init[i]) % mod;
    }
    return ans;
}
}

namespace cp_fft {
using cd = complex<double>;
// 对复数数组 a 原地做 FFT；inv=false 为正变换，true 为逆变换并除以长度。
// 要求长度为正二次幂；浮点计算存在误差，时间 O(n log n)。
void fft(vector<cd>& a, bool inv) {
    int n = a.size();
    assert(n > 0 && (n & (n - 1)) == 0);
    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) {
            j ^= bit;
        }
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    const double PI = acos(-1.0);
    for (int len = 2; len <= n; len <<= 1) {
        cd step = polar(1.0, 2 * PI / len * (inv ? -1 : 1));
        for (int i = 0; i < n; i += len) {
            cd w = 1;
            for (int j = 0; j < len / 2; ++j) {
                cd u = a[i + j], v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= step;
            }
        }
    }
    if (inv) {
        for (cd& x : a) {
            x /= n;
        }
    }
}
// 返回实系数多项式 a、b 的近似卷积，系数按次数从 0 编号；空输入返回空数组。
// 输出长度 |a|+|b|-1，不修改输入；变换长度 L 下时间 O(L log L)、空间 O(L)。
// 用于整数卷积时须先验证误差再舍入，不能直接当作精确大整数卷积。
vector<double> convolution(const vector<double>& a, const vector<double>& b) {
    if (a.empty() || b.empty()) return {};
    int need = a.size() + b.size() - 1, n = 1;
    while (n < need) {
        n <<= 1;
    }
    vector<cd> x(n), y(n);
    copy(a.begin(), a.end(), x.begin());
    copy(b.begin(), b.end(), y.begin());
    fft(x, false);
    fft(y, false);
    for (int i = 0; i < n; ++i) {
        x[i] *= y[i];
    }
    fft(x, true);
    vector<double> ans(need);
    for (int i = 0; i < need; ++i) {
        ans[i] = x[i].real();
    }
    return ans;
}
}

namespace cp_games {
// 判断正常 Nim 先手是否必胜：a[1..n] 为各堆石子数，忽略 a[0]。
// 所有堆大小异或非零返回 true，空局面返回 false；时间 O(n)。
bool nim_win(const vector<u64>& a) {
    u64 x = 0;
    for (int i = 1; i < (int)a.size(); ++i) {
        x ^= a[i];
    }
    return x != 0;
}
}

namespace cp_misere {
// 判断取最后一颗石子者输的反 Nim 先手胜负，堆数组为 a[1..n]。
// 存在大于 1 的堆时看异或和，否则看 1 石子堆是否为偶数；本实现空局面返回 true。
bool misere_nim_win(const vector<u64>& a) {
    u64 x = 0;
    int ones = 0;
    bool big = false;
    for (int i = 1; i < (int)a.size(); ++i) {
        u64 v = a[i];
        x ^= v;
        ones += v == 1;
        big |= v > 1;
    }
    return big ? x != 0 : ones % 2 == 0;
}
}

namespace cp_sg {
// 计算减法游戏状态 0..N 的 SG 值；moves 是每步可取走数量的无占位列表。
// 要求 N>=0、每个动作 d>0；返回 sg[0]=0 的数组，时间 O(N*(|moves|+1))。
vector<int> subtraction_sg(int N, const vector<int>& moves) {
    int K = moves.size();
    vector<int> sg(N + 1), seen(K + 1, -1);
    for (int x = 1; x <= N; ++x) {
        for (int d : moves) {
            assert(d > 0);
            if (d <= x && sg[x - d] <= K) {
                seen[sg[x - d]] = x;
            }
        }
        int g = 0;
        while (g <= K && seen[g] == x) {
            ++g;
        }
        sg[x] = g;
    }
    return sg;
}
}

namespace cp_bigint {
// 返回 10^k，用于编译期计算分块基数；要求 0<=k<=9。
constexpr int power10(int k) {
    int result = 1;
    while (k--) {
        result *= 10;
    }
    return result;
}

// 非负高精度整数：低位在前，从 0 下标开始，零统一表示为 {0}。
struct big_int {
    static constexpr int width = 3; // 只修改这里，每块 1～9 位
    static_assert(1 <= width && width <= 9, "width must be between 1 and 9");
    static constexpr int base = power10(width);
    vector<int> arr;

    // 将非负 int 转为高精度整数，默认构造为 0。
    big_int(int x = 0) {
        assert(x >= 0);
        do {
            arr.push_back(x % base);
            x /= base;
        } while (x);
    }

    // 将非空十进制数字字符串转为高精度整数，自动去除前导零。
    big_int(const string& s) {
        assert(!s.empty());
        for (int r = (int)s.size(); r > 0; r -= width) {
            int x = 0;
            for (int l = max(0, r - width); l < r; ++l) {
                assert(s[l] >= '0' && s[l] <= '9');
                x = x * 10 + s[l] - '0';
            }
            arr.push_back(x);
        }
        trim();
    }

    // 返回有效块数。
    int size() const { return (int)arr.size(); }

    // 删除最高端多余的零块，至少保留一块表示 0。
    void trim() {
        while (arr.size() > 1 && arr.back() == 0) {
            arr.pop_back();
        }
    }

    // 判断当前整数是否为 0。
    bool is_zero() const { return arr.size() == 1 && arr[0] == 0; }
};

// 输出 a，最高块正常输出，其余块补足 width 位；不自动换行。
void print(const big_int& a) {
    cout << a.arr.back();
    for (int i = a.size() - 2; i >= 0; --i) {
        string s = to_string(a.arr[i]);
        cout << string(big_int::width - (int)s.size(), '0') << s;
    }
}

// 比较 a、b：小于返回 -1，相等返回 0，大于返回 1。
int compare(const big_int& a, const big_int& b) {
    if (a.size() != b.size()) return a.size() < b.size() ? -1 : 1;
    for (int i = a.size() - 1; i >= 0; --i) {
        if (a.arr[i] != b.arr[i]) return a.arr[i] < b.arr[i] ? -1 : 1;
    }
    return 0;
}

// 返回 a+b，从低位向高位逐块相加并进位。
big_int jia(const big_int& a, const big_int& b) {
    big_int result;
    int n = max(a.size(), b.size());
    result.arr.assign(n + 1, 0);
    int carry = 0;
    for (int i = 0; i < n; ++i) {
        int x = i < a.size() ? a.arr[i] : 0;
        int y = i < b.size() ? b.arr[i] : 0;
        int cur = x + y + carry;
        result.arr[i] = cur % big_int::base;
        carry = cur / big_int::base;
    }
    result.arr[n] = carry;
    result.trim();
    return result;
}

// 返回 a-b，要求 a>=b；从低位向高位逐块相减并借位。
big_int jian(const big_int& a, const big_int& b) {
    assert(compare(a, b) >= 0);
    big_int result;
    result.arr.assign(a.size(), 0);
    int borrow = 0;
    for (int i = 0; i < a.size(); ++i) {
        int y = i < b.size() ? b.arr[i] : 0;
        int cur = a.arr[i] - y - borrow;
        borrow = cur < 0;
        if (borrow) {
            cur += big_int::base;
        }
        result.arr[i] = cur;
    }
    result.trim();
    return result;
}

// 返回高精度 a 与非负 int b 的乘积，中间运算使用 i64。
big_int chen(const big_int& a, int b) {
    assert(b >= 0);
    if (b == 0 || a.is_zero()) return big_int(0);
    big_int result;
    result.arr.assign(a.size(), 0);
    i64 carry = 0;
    for (int i = 0; i < a.size(); ++i) {
        i64 cur = (i64)a.arr[i] * b + carry;
        result.arr[i] = (int)(cur % big_int::base);
        carry = cur / big_int::base;
    }
    while (carry) {
        result.arr.push_back((int)(carry % big_int::base));
        carry /= big_int::base;
    }
    result.trim();
    return result;
}

// 返回两个高精度整数的乘积，采用逐块竖式乘法。
big_int chen(const big_int& a, const big_int& b) {
    if (a.is_zero() || b.is_zero()) return big_int(0);
    big_int result;
    result.arr.assign(a.size() + b.size(), 0);
    for (int i = 0; i < a.size(); ++i) {
        i64 carry = 0;
        for (int j = 0; j < b.size(); ++j) {
            i64 cur = result.arr[i + j] + (i64)a.arr[i] * b.arr[j] + carry;
            result.arr[i + j] = (int)(cur % big_int::base);
            carry = cur / big_int::base;
        }
        // 此位置尚未被之前的外层循环写入。
        result.arr[i + b.size()] = (int)carry;
    }
    result.trim();
    return result;
}

// 计算高精度 a 除以正 int b，返回 {商, 余数}，从高位向低位处理。
pair<big_int, int> divmod(const big_int& a, int b) {
    assert(b > 0);
    big_int quotient;
    quotient.arr.assign(a.size(), 0);
    i64 remainder = 0;
    for (int i = a.size() - 1; i >= 0; --i) {
        i64 cur = remainder * big_int::base + a.arr[i];
        quotient.arr[i] = (int)(cur / b);
        remainder = cur % b;
    }
    quotient.trim();
    return {quotient, (int)remainder};
}

// 计算高精度 a 除以非零高精度 b，返回 {商, 余数}。
// 采用竖式除法，每块商在 [0,base-1] 内二分。
pair<big_int, big_int> divmod(const big_int& a, const big_int& b) {
    assert(!b.is_zero());
    if (compare(a, b) < 0) return {big_int(0), a};
    if (b.size() == 1) {
        auto [q, r] = divmod(a, b.arr[0]);
        return {q, big_int(r)};
    }
    big_int quotient, remainder;
    quotient.arr.assign(a.size(), 0);
    for (int i = a.size() - 1; i >= 0; --i) {
        // remainder = remainder*base + a.arr[i]。
        if (remainder.is_zero()) {
            remainder.arr[0] = a.arr[i];
        } else {
            remainder.arr.insert(remainder.arr.begin(), a.arr[i]);
        }
        int l = 0, r = big_int::base;
        while (l + 1 < r) {
            int m = (l + r) / 2;
            big_int product = chen(b, m);
            if (compare(product, remainder) <= 0) {
                l = m;
            } else {
                r = m;
            }
        }
        quotient.arr[i] = l;
        if (l != 0) {
            remainder = jian(remainder, chen(b, l));
        }
    }
    quotient.trim();
    return {quotient, remainder};
}

// 返回高精度 a 除以正 int b 的整数商。
big_int chu(const big_int& a, int b) { return divmod(a, b).first; }

// 返回高精度 a 除以非零高精度 b 的整数商。
big_int chu(const big_int& a, const big_int& b) { return divmod(a, b).first; }
}

namespace cp_knapsack {
// 将一件重量 w、价值 v 的物品加入 0/1 背包，原地更新 dp[0..W]，容量倒序。
// 要求 w>0；默认 dp 为容量不超过上限的最优值且允许不选，加法不得溢出；时间 O(W)。
void zero_one(vector<i64>& dp, int w, i64 v) {
    assert(w > 0);
    for (int c = (int)dp.size() - 1; c >= w; --c) {
        dp[c] = max(dp[c], dp[c - w] + v);
    }
}
}

namespace cp_complete {
// 将可无限使用的重量 w、价值 v 的物品加入完全背包，原地更新 dp，容量正序。
// 要求 w>0，dp 使用容量 0..W；当前初始化约定为不要求恰好装满，时间 O(W)。
void complete(vector<i64>& dp, int w, i64 v) {
    assert(w > 0);
    for (int c = w; c < (int)dp.size(); ++c) {
        dp[c] = max(dp[c], dp[c - w] + v);
    }
}
}

namespace cp_bounded {
using cp_knapsack::zero_one;
// 将最多 count 件重量 w、价值 v 的物品加入多重背包，二进制拆分后更新 dp。
// 要求 w>0、count>=0，价值乘积及转移和不溢出；时间 O(W*log(count+1))。
void bounded(vector<i64>& dp, int w, i64 v, int count) {
    assert(w > 0 && count >= 0);
    for (i64 k = 1; count > 0; k *= 2) {
        int take = (int)min<i64>(k, count);
        count -= take;
        i64 weight = 1LL * w * take;
        if (weight < (int)dp.size()) {
            zero_one(dp, (int)weight, v * take);
        }
    }
}
}

namespace cp_lis {
// 返回 a[1..n] 的一条最长严格上升子序列对应的原数组下标列表，忽略 a[0]。
// 结果列表不带占位，不保证字典序最小；时间 O(n log n)，空间 O(n)。
vector<int> lis_indices(const vector<i64>& a) {
    vector<i64> tail;
    vector<int> at, pre(a.size(), -1);
    for (int i = 1; i < (int)a.size(); ++i) {
        int k = lower_bound(tail.begin(), tail.end(), a[i]) - tail.begin();
        if (k) {
            pre[i] = at[k - 1];
        }
        if (k == (int)tail.size()) {
            tail.push_back(a[i]);
            at.push_back(i);
        } else {
            tail[k] = a[i];
            at[k] = i;
        }
    }
    vector<int> ans;
    if (at.empty()) return ans;
    for (int i = at.back(); i != -1; i = pre[i]) {
        ans.push_back(i);
    }
    reverse(ans.begin(), ans.end());
    return ans;
}
}

namespace cp_lcs {
// 返回字符串 a、b 的最长公共子序列长度，允许不连续；空串对应答案 0。
// 滚动数组不保存恢复方案所需的信息；时间 O(|a|*|b|)，空间 O(|b|)。
int lcs(const string& a, const string& b) {
    vector<int> dp(b.size() + 1);
    for (char x : a) {
        int diag = 0;
        for (int j = 1; j <= (int)b.size(); ++j) {
            int old = dp[j];
            if (x == b[j - 1]) {
                dp[j] = diag + 1;
            } else {
                dp[j] = max(dp[j], dp[j - 1]);
            }
            diag = old;
        }
    }
    return dp.back();
}
}

namespace cp_digitdp {
// 统计闭区间 [0,N] 中十进制相邻数字不同的整数个数，计入 0；N<0 返回 0。
// 前导零不参与比较；可用 count(R)-count(L-1) 求区间答案，时间与十进制位数成正比。
u64 count_no_equal_adjacent(i64 N) {
    if (N < 0) return 0;
    string s = to_string(N);
    u64 memo[20][11]{};
    bool vis[20][11]{};
    // 内部数位搜索：pos 为当前 0 下标位，prev 为上一位数字，10 表示尚未开始。
    // tight 表示前缀是否贴住上界；返回当前状态的合法补全数量，仅缓存不贴上界的状态。
    function<u64(int, int, bool)> dfs = [&](int pos, int prev, bool tight) -> u64 {
        if (pos == (int)s.size()) return 1;
        if (!tight && vis[pos][prev]) return memo[pos][prev];
        int lim = tight ? s[pos] - '0' : 9;
        u64 ans = 0;
        for (int d = 0; d <= lim; ++d) {
            int next = (prev == 10 && d == 0) ? 10 : d;
            if (prev != 10 && d == prev) continue;
            ans += dfs(pos + 1, next, tight && d == lim);
        }
        if (!tight) {
            vis[pos][prev] = true;
            memo[pos][prev] = ans;
        }
        return ans;
    };
    return dfs(0, 10, true);
}
}

namespace cp_sos {
// 返回每个掩码 S 的子集和 Σ(T⊆S) f[T]；按值处理，不修改调用者的数组。
// 要求 0<=n<31、f.size()==2^n 且内存足够，累加不溢出；时间 O(n*2^n)。
vector<i64> subset_sum(vector<i64> f, int n) {
    assert(0 <= n && n < 31 && f.size() == (1ULL << n));
    for (int b = 0; b < n; ++b) {
        for (int s = 0; s < (1 << n); ++s) {
            if (s >> b & 1) {
                f[s] += f[s ^ (1 << b)];
            }
        }
    }
    return f;
}
// 返回每个掩码 S 的超集和 Σ(T⊇S) f[T]，掩码 0 表示空集。
// 要求 0<=n<31、f.size()==2^n 且内存足够，累加不溢出；时间 O(n*2^n)。
vector<i64> superset_sum(vector<i64> f, int n) {
    assert(0 <= n && n < 31 && f.size() == (1ULL << n));
    for (int b = 0; b < n; ++b) {
        for (int s = 0; s < (1 << n); ++s) {
            if (!(s >> b & 1)) {
                f[s] += f[s ^ (1 << b)];
            }
        }
    }
    return f;
}
}

namespace cp_tree_dp {
// 返回连通无向树的最大权独立集权值和，允许不选任何点；全负权时返回 0。
// g 和 w 均使用 1..n，0 号占位，要求权值和不溢出；迭代遍历耗时、空间均为 O(n)。
i64 tree_independent_set(const vector<vector<int>>& g, const vector<i64>& w) {
    int n = (int)g.size() - 1;
    if (!n) return 0;
    vector<int> p(n + 1, -1), ord{1};
    p[1] = 1;
    for (int i = 0; i < n; ++i) {
        for (int v : g[ord[i]]) {
            if (v != p[ord[i]]) {
                p[v] = ord[i];
                ord.push_back(v);
            }
        }
    }
    vector<array<i64, 2>> dp(n + 1);
    for (int i = n - 1; i >= 0; --i) {
        int u = ord[i];
        dp[u][1] = w[u];
        for (int v : g[u]) {
            if (v != p[u]) {
                dp[u][0] += max(dp[v][0], dp[v][1]);
                dp[u][1] += dp[v][0];
            }
        }
    }
    return max(dp[1][0], dp[1][1]);
}
}

namespace cp_lichao {
struct LiChao {
    struct Line {
        i64 k = 0, b = 0;
        // 计算当前直线在 x 处的 y=k*x+b，使用 i128 中间量；有效值绝对值须小于 2^126。
        i128 eval(i64 x) const { return i128(k) * x + b; }
    };
    const i128 INF = i128(1) << 126;
    vector<i64> xs;
    vector<Line> t;
    vector<char> has;
    // 收集全部将被查询的横坐标 x[1..m]，忽略 x[0]，排序去重后建立空李超树。
    // 至少需要一个有效坐标；初始化时间 O(m log m)，存储 O(m)。
    LiChao(vector<i64> x) : xs(move(x)) {
        assert(!xs.empty());
        xs.erase(xs.begin());
        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(), xs.end()), xs.end());
        assert(!xs.empty());
        t.resize(xs.size() * 4);
        has.resize(xs.size() * 4);
    }
    // 向李超树插入整条直线 v={k,b}，维护已收集坐标处的最小值；时间 O(log m)。
    void add(Line v) {
        add(1, 0, xs.size(), v);
    }
    // 内部递归插入：节点 p 覆盖压缩坐标下标区间 [l,r)，其中下标从 0 开始。
    // 保留在中点更优的直线，再递归处理另一条线可能更优的一侧。
    void add(int p, int l, int r, Line v) {
        if (!has[p]) {
            has[p] = 1;
            t[p] = v;
            return;
        }
        int m = (l + r) / 2;
        bool left = v.eval(xs[l]) < t[p].eval(xs[l]);
        bool mid = v.eval(xs[m]) < t[p].eval(xs[m]);
        if (mid) swap(t[p], v);
        if (r - l == 1) return;
        if (left != mid) {
            add(p * 2, l, m, v);
        } else {
            add(p * 2 + 1, m, r, v);
        }
    }
    // 返回所有已插入直线在横坐标 x 处的最小值；x 必须在构造时收集的坐标中。
    // 尚未插入直线时返回 INF，时间 O(log m)，不修改树。
    i128 query(i64 x) {
        int idx = lower_bound(xs.begin(), xs.end(), x) - xs.begin();
        assert(idx < (int)xs.size() && xs[idx] == x);
        int p = 1, l = 0, r = xs.size();
        i128 ans = INF;
        while (true) {
            if (has[p]) {
                ans = min(ans, t[p].eval(x));
            }
            if (r - l == 1) break;
            int m = (l + r) / 2;
            if (idx < m) {
                p *= 2;
                r = m;
            } else {
                p = p * 2 + 1;
                l = m;
            }
        }
        return ans;
    }
};
}

namespace cp_divide_dp {
const i64 INF = (1LL << 62);

// 计算一层 cur[i]=min(0<=j<i){prev[j]+cost(j,i)}，返回 cur[0..n]，cur[0]=INF。
// prev 的下标是前缀长度，cost(j,i) 对应原数组 [j+1,i]；要求最优决策位置单调不减。
// 不可达 prev 用 INF，有限转移结果须在 (-INF,INF)；cost 为 O(1) 时耗时 O(n log n)。
template <class Cost>
vector<i64> divide_dp_layer(const vector<i64>& prev, Cost cost) {
    int n = prev.size() - 1;
    vector<i64> cur(n + 1, INF);
    // 内部决策分治：求 cur[l..r]，最优决策只在 [optL,optR] 中搜索。
    // 先计算中点最优决策，再收窄左右搜索范围；只有决策单调性成立时才正确。
    function<void(int, int, int, int)> go = [&](int l, int r, int optL, int optR) {
        if (l > r) return;
        int mid = (l + r) / 2, best = optL;
        for (int j = optL; j <= min(mid - 1, optR); ++j) {
            if (prev[j] == INF) continue;
            i128 v = i128(prev[j]) + cost(j, mid);
            if (v < cur[mid]) {
                cur[mid] = (i64)v;
                best = j;
            }
        }
        go(l, mid - 1, optL, best);
        go(mid + 1, r, best, optR);
    };
    if (n) {
        go(1, n, 0, n - 1);
    }
    return cur;
}
}

namespace cp_geo_int {

struct Point {
    i64 x, y;
    // 按 (x,y) 字典序比较当前点和 b，供严格排序使用；不使用浮点容差，O(1)。
    bool operator<(Point b) const { return tie(x, y) < tie(b.x, b.y); }
    // 判断当前点与 b 的两个整数坐标是否完全相等，供点集去重使用，O(1)。
    bool operator==(Point b) const { return x == b.x && y == b.y; }
};
// 返回向量 AB 与 AC 的叉积，即三点构成三角形的两倍有向面积。
// 正值表示 a→b→c 逆时针，0 表示共线；坐标绝对值要求不超过 10^18，O(1)。
i128 cross(Point a, Point b, Point c) {
    return (i128(b.x) - a.x) * (i128(c.y) - a.y) - (i128(b.y) - a.y) * (i128(c.x) - a.x);
}
// 返回 x 的符号：负数为 -1，零为 0，正数为 1；时间 O(1)。
int sign(i128 x) { return (x > 0) - (x < 0); }
// 判断点 p 是否在线段 ab 上，包含端点，也支持 a=b 的退化线段。
// 使用精确叉积和坐标范围判断，坐标满足整数几何范围约定；时间 O(1)。
bool on_segment(Point p, Point a, Point b) {
    return cross(a, b, p) == 0 && min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x) &&
           min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y);
}

}

namespace cp_intgeo {
using cp_geo_int::Point;
using cp_geo_int::cross;
using cp_geo_int::sign;
using cp_geo_int::on_segment;
// 判断闭线段 ab 与 cd 是否有交点，包含端点接触、共线重叠和退化点。
// 使用整数精确判断，坐标绝对值不超过 10^18；时间 O(1)。
bool intersect(Point a, Point b, Point c, Point d) {
    int x = sign(cross(a, b, c)), y = sign(cross(a, b, d));
    int z = sign(cross(c, d, a)), w = sign(cross(c, d, b));
    if (!x && on_segment(c, a, b)) return true;
    if (!y && on_segment(d, a, b)) return true;
    if (!z && on_segment(a, c, d)) return true;
    if (!w && on_segment(b, c, d)) return true;
    return x * y < 0 && z * w < 0;
}
}

namespace cp_hull {
using cp_geo_int::Point;
using cp_geo_int::cross;
// 返回点集 p 的逆时针凸包顶点列表，不带占位、不重复首点，删除边上中间共线点。
// 输入可无序、有重复，不修改调用者的点集；全共线时只留端点，时间 O(n log n)。
vector<Point> convex_hull(vector<Point> p) {
    sort(p.begin(), p.end());
    p.erase(unique(p.begin(), p.end()), p.end());
    int n = p.size();
    if (n <= 1) return p;
    vector<Point> h(2 * n);
    int k = 0;
    for (auto v : p) {
        while (k >= 2 && cross(h[k - 2], h[k - 1], v) <= 0) {
            --k;
        }
        h[k++] = v;
    }
    for (int i = n - 2, t = k + 1; i >= 0; --i) {
        while (k >= t && cross(h[k - 2], h[k - 1], p[i]) <= 0) {
            --k;
        }
        h[k++] = p[i];
    }
    h.resize(k - 1);
    return h;
}
}

namespace cp_polygon {
using cp_geo_int::Point;
// Dependency: Point, cross, on_segment.
// 返回按边界顺序排列的多边形顶点 p 的两倍有向面积，逆时针为正、顺时针为负。
// p 是无占位循环列表，不需重复首点；要求叉积累加不溢出 i128，时间 O(n)。
i128 area2(const vector<Point>& p) {
    i128 a = 0;
    int n = p.size();
    for (int i = 0; i < n; ++i) {
        auto q = p[(i + 1) % n];
        a += i128(p[i].x) * q.y - i128(p[i].y) * q.x;
    }
    return a;
}
}

namespace cp_point_location {
using cp_geo_int::Point;
using cp_geo_int::cross;
using cp_geo_int::on_segment;
// 判断点 q 与简单多边形 p 的位置关系：0 为外部、1 为边界、2 为内部。
// p 是按边界顺序排列的无占位循环列表，顺逆时针均可；时间 O(n)。
int inside(const vector<Point>& p, Point q) {
    int wn = 0, n = p.size();
    for (int i = 0; i < n; ++i) {
        Point a = p[i], b = p[(i + 1) % n];
        if (on_segment(q, a, b)) return 1;
        if (a.y <= q.y && b.y > q.y && cross(a, b, q) > 0) {
            ++wn;
        }
        if (a.y > q.y && b.y <= q.y && cross(a, b, q) < 0) {
            --wn;
        }
    }
    return wn ? 2 : 0;
}
}

namespace cp_diameter {
using cp_geo_int::Point;
using cp_geo_int::cross;
// 返回整数点 a、b 的欧氏距离平方，避免开方；坐标绝对值不超过 10^18，时间 O(1)。
i128 dist2(Point a, Point b) {
    i128 x = i128(a.x) - b.x, y = i128(a.y) - b.y;
    return x * x + y * y;
}
// 用旋转卡壳返回凸多边形最远两点距离的平方；不足两个点返回 0。
// p 必须逆时针排列且无边上中间共线点，可直接使用 convex_hull 的结果；时间 O(n)。
i128 diameter2(const vector<Point>& p) {
    int n = p.size();
    if (n < 2) return 0;
    if (n == 2) return dist2(p[0], p[1]);
    i128 ans = 0;
    int j = 1;
    for (int i = 0; i < n; ++i) {
        int ni = (i + 1) % n;
        while (cross(p[i], p[ni], p[(j + 1) % n]) > cross(p[i], p[ni], p[j])) {
            j = (j + 1) % n;
        }
        ans = max({ans, dist2(p[i], p[j]), dist2(p[ni], p[j])});
    }
    return ans;
}
}

namespace cp_geo_float {

using ld = long double;
const ld EPS = 1e-12L;
struct P {
    ld x, y;
    // 返回当前二维向量与 b 的逐坐标和，不修改原向量；时间 O(1)。
    P operator+(P b) const { return {x + b.x, y + b.y}; }
    // 返回当前二维向量减去 b 的结果，可表示从 b 指向当前点的位移；时间 O(1)。
    P operator-(P b) const { return {x - b.x, y - b.y}; }
    // 返回当前向量乘标量 k 的结果，不修改原向量；时间 O(1)。
    P operator*(ld k) const { return {x * k, y * k}; }
};
// 返回二维向量 a、b 的叉积，正负表示旋转方向；浮点结果需结合 EPS 判断，O(1)。
ld cross(P a, P b) { return a.x * b.y - a.y * b.x; }
// 返回二维向量 a、b 的点积，可用于投影和长度平方计算；时间 O(1)。
ld dot(P a, P b) { return a.x * b.x + a.y * b.y; }

}

namespace cp_floatgeo {
using cp_geo_float::P;
using cp_geo_float::ld;
using cp_geo_float::cross;
using cp_geo_float::dot;
using cp_geo_float::EPS;
// 返回直线 p+t*v 与 q+s*w 的唯一交点；v、w 必须是非零方向向量。
// 在 EPS 意义下平行或重合时返回 nullopt，不区分这两种情况；时间 O(1)。
optional<P> line_intersection(P p, P v, P q, P w) {
    ld d = cross(v, w);
    if (fabsl(d) < EPS) return nullopt;
    return p + v * (cross(q - p, w) / d);
}
}

namespace cp_circle {
using cp_geo_float::P;
using cp_geo_float::ld;
using cp_geo_float::cross;
using cp_geo_float::dot;
using cp_geo_float::EPS;
// 求圆心 a、半径 r 与圆心 b、半径 s 两圆的交点，要求半径非负。
// 返回 {数量,交点列表}：0/1/2 为有限交点数，-1 表示重合且有无穷交点。
// 按 EPS 判定，重合的零半径圆按一个交点处理；时间和空间 O(1)。
pair<int, vector<P>> circle_intersection(P a, ld r, P b, ld s) {
    assert(r >= 0 && s >= 0);
    P v = b - a;
    ld d = sqrtl(dot(v, v));
    if (d < EPS) {
        if (fabsl(r - s) > EPS) return {0, {}};
        if (r < EPS) return {1, {a}};
        return {-1, {}};
    }
    if (d > r + s + EPS || d < fabsl(r - s) - EPS) return {0, {}};
    ld x = (r * r - s * s + d * d) / (2 * d), h2 = r * r - x * x;
    if (h2 < -EPS) return {0, {}};
    P base = a + v * (x / d);
    if (h2 <= EPS) return {1, {base}};
    P off = P{-v.y, v.x} * (sqrtl(max((ld)0, h2)) / d);
    return {2, {base + off, base - off}};
}
}

namespace cp_halfplane {
using cp_geo_float::P;
using cp_geo_float::ld;
using cp_geo_float::cross;
using cp_geo_float::dot;
using cp_geo_float::EPS;
// Dependency: P, cross, EPS.
// 用有向直线 a→b 裁剪凸多边形 poly，保留其左侧及边界，返回新的循环顶点列表。
// 要求 a!=b，顶点按边界顺序排列且不带占位；空结果表示在容差意义下无交集。
// 不修改输入，每次耗时 O(顶点数)；连续裁剪需从保证覆盖解集的包围框开始。
vector<P> clip_halfplane(const vector<P>& poly, P a, P b) {
    vector<P> out;
    int n = poly.size();
    P v = b - a;
    assert(dot(v, v) > 0);
    for (int i = 0; i < n; ++i) {
        P p = poly[i], q = poly[(i + 1) % n];
        ld dp = cross(v, p - a), dq = cross(v, q - a);
        bool ip = dp >= -EPS, iq = dq >= -EPS;
        if (ip) {
            out.push_back(p);
        }
        if (ip != iq) {
            // Classification uses EPS; clamp near-boundary interpolation.
            ld t = dp / (dp - dq);
            t = max((ld)0, min((ld)1, t));
            out.push_back(p + (q - p) * t);
        }
    }
    return out;
}
}

using cp_lazyseg::segmenttree;
using cp_segtree::SegTree;
