n, m = map(int, input().split())
a = list(map(int, input().split()))

suffix_sum = [0] * (n + 1)
for i in range(n - 1, -1, -1):
    suffix_sum[i] = suffix_sum[i + 1] + a[i]

dp = [0] * (n + 1)

for i in range(n - 1, -1, -1):
    max_diff = -10**7
    for k in range(1, min(m, n - i) + 1):
        taken_sum = suffix_sum[i] - suffix_sum[i + k]
        diff = taken_sum - dp[i + k]
        if diff > max_diff:
            max_diff = diff
    dp[i] = max_diff

print(1 if dp[0] > 0 else 0)
