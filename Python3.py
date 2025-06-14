from math import pow, gcd, isclose

def is_rational(value, max_den=10**6, eps=1e-10):
    for d in range(1, max_den + 1):
        n = round(value * d)
        if isclose(value, n / d, abs_tol=eps):
            g = gcd(n, d)
            return (n // g, d // g)
    return None


a, b = map(int, input("Введите числа: ").split())

if b <= 1:
    print("infinity")
else:
    total = 0.0
    EPS = 1e-15

    for n in range(1, 1000000):
        term = pow(n, a) / pow(b, n)
        total += term
        if term < EPS:
            break

    result = is_rational(total)
    if result:
        num, den = result
        print(f"{num}/{den}")
    else:
        print("irrational")

