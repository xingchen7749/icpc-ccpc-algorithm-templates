#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

// 返回 10^k，用于编译期计算分块基数；要求 0<=k<=9。
constexpr int power10(int k) {
    int result = 1;
    while (k--) result *= 10;
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
        while (arr.size() > 1 && arr.back() == 0) arr.pop_back();
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
        if (borrow) cur += big_int::base;
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
        }
        else {
            remainder.arr.insert(remainder.arr.begin(), a.arr[i]);
        }
        int l = 0, r = big_int::base;
        while (l + 1 < r) {
            int m = (l + r) / 2;
            big_int product = chen(b, m);
            if (compare(product, remainder) <= 0) {
                l = m;
            }
            else {
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
