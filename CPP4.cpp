#include <iostream>
#include <vector>
using namespace std;

int main() {

    int n, m;
    cin >> n >> m;
    vector<int> a(n);
    for (int &x : a) cin >> x;

    // Суффиксные суммы для быстрого подсчёта суммы от i до j
    vector<long long> suffix_sum(n + 1, 0);
    for (int i = n - 1; i >= 0; --i)
        suffix_sum[i] = suffix_sum[i + 1] + a[i];

    // dp[i] — максимальная разница для игрока, начинающего ход с позиции i
    vector<long long> dp(n + 1, 0);

    for (int i = n - 1; i >= 0; --i) {
        long long max_diff = -1e7;
        for (int k = 1; k <= m && i + k <= n; ++k) {
            long long taken_sum = suffix_sum[i] - suffix_sum[i + k];
            long long diff = taken_sum - dp[i + k];
            if (diff > max_diff)
                max_diff = diff;
        }
        dp[i] = max_diff;
    }

    cout << (dp[0] > 0 ? 1 : 0) << "\n";

    return 0;
}

