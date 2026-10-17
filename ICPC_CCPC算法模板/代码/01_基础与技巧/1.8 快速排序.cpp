#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

// 将 a[l..r] 原地按升序排序，无返回值；调用时保证区间下标合法。
// 数组采用 1 下标，a[0] 占位；l>=r 时不操作。
// 平均时间 O(n log n)，最坏 O(n²)；递归栈平均 O(log n)，最坏 O(n)。
void quick_sort(vector<int>& a, int l, int r) {
    if (l >= r) return;

    int i = l - 1, j = r + 1;
    int pivot = a[l + (r - l) / 2];  // 保存基准值，避免交换后基准改变。

    while (i < j) {
        do {
            i++;
        } while (a[i] < pivot);

        do {
            j--;
        } while (a[j] > pivot);

        if (i < j) {
            swap(a[i], a[j]);
        }
    }

    // 分区后，左区间元素不大于基准，右区间元素不小于基准。
    quick_sort(a, l, j);
    quick_sort(a, j + 1, r);
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
