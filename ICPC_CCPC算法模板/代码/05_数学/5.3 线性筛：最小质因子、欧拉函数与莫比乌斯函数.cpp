#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

struct Sieve {
    // primes：素数列表；lp（least prime factor）：最小质因子，lp[1]=0。
    // phi：欧拉函数值；mu：莫比乌斯函数值，phi[1]=mu[1]=1。
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
