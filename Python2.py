import random
import math
from typing import List, Tuple

def is_prime_list(n: int) -> List[int]:
    is_prime = [True] * (n + 1)
    primes = []
    for p in range(2, int(n ** 0.5) + 1):
        if is_prime[p]:
            for i in range(p * p, n + 1, p):
                is_prime[i] = False
    for p in range(2, n + 1):
        if is_prime[p]:
            primes.append(p)
    return primes

def pow_mod(a: int, b: int, n: int) -> int:
    result = 1
    a %= n
    while b > 0:
        if b & 1:
            result = (result * a) % n
        a = (a * a) % n
        b >>= 1
    return result

def rn(a: int, b: int) -> int:
    return random.randint(a, b)

def rd(a: float, b: float) -> float:
    return random.uniform(a, b)

def test_miller(n: int, t: int, q: List[int]) -> bool:
    if n <= 1: return False
    if n == 2: return True
    if n % 2 == 0: return False
    a = []
    while len(a) < t:
        aj = rn(2, n - 1)
        if aj not in a:
            a.append(aj)
    for aj in a:
        if pow_mod(aj, n - 1, n) != 1:
            return False
    for qi in q:
        if all(pow_mod(aj, (n - 1) // qi, n) == 1 for aj in a):
            return False
    return True

def test_pocklington(n: int, t: int, q: List[int]) -> bool:
    a = []
    while len(a) < t:
        aj = rn(1, n - 1)
        if aj not in a:
            a.append(aj)
    if not all(pow_mod(aj, n - 1, n) == 1 for aj in a):
        return False
    for aj in a:
        if all(pow_mod(aj, (n - 1) // qi, n) != 1 for qi in q):
            return True
    return False

def build_miller(prime: List[int], bit: int) -> Tuple[int, List[int]]:
    max_index = 0
    while max_index < len(prime) and prime[max_index] < (1 << (bit - 1)):
        max_index += 1
    m, q = 1, []
    while True:
        num = rn(0, max_index - 1)
        power = rn(1, int(math.log2(1 << (bit - 1))))
        temp = prime[num] ** power
        if temp > 0 and m <= (2 ** 31 - 1) // temp:
            m *= temp
            q.append(prime[num])
        if m > (1 << (bit - 2)):
            if m >= (1 << (bit - 1)):
                m, q = 1, []
            else:
                break
    return 2 * m + 1, q

def build_pocklington(prime: List[int], bit: int) -> Tuple[int, List[int]]:
    max_index = 0
    while max_index < len(prime) and prime[max_index] < (1 << ((bit // 2) + 1)):
        max_index += 1
    f, q = 1, []
    while True:
        num = rn(0, max_index - 1)
        power = rn(1, int(math.log2(1 << ((bit // 2) + 1))))
        temp = prime[num] ** power
        if temp > 0 and f <= (2 ** 31 - 1) // temp:
            f *= temp
            q.append(prime[num])
        if f > (1 << (bit // 2)):
            if f >= (1 << ((bit // 2) + 1)):
                f, q = 1, []
            else:
                break
    while True:
        R = rn((1 << ((bit // 2) - 1)) + 1, (1 << (bit // 2)) - 1)
        if R % 2 == 0:
            break
    return R * f + 1, q

def build_gost(prime: List[int], bit: int) -> int:
    max_index = 0
    while max_index < len(prime) and prime[max_index] < (1 << (bit // 2)):
        max_index += 1
    q = prime[rn(0, max_index - 1)]
    while True:
        epsilon = rd(0, 1)
        n0 = (2 ** (bit - 1) / q) + (2 ** (bit - 1) * epsilon / q)
        n = math.ceil(n0)
        if n % 2 != 0:
            n += 1
        for u in range(0, 1000, 2):
            p = (n + u) * q + 1
            if p > (1 << bit):
                break
            if pow_mod(2, p - 1, p) == 1 and pow_mod(2, n + u, p) != 1:
                return p

def calc_iterations(error_probability: float) -> int:
    return math.ceil(math.log(error_probability) / math.log(0.25))


primes = is_prime_list(500)
bits = int(input("Введите размер чисел в битах (2-31): "))

if bits < 2 or bits > 31:
    print("Ошибка: некорректный размер!")
    exit(1)

target_error = 0.1
test_iterations = calc_iterations(target_error)
print(f"Используем {test_iterations} итераций теста для вероятности ошибки <= {target_error}")

# Метод Миллера
print("\nМетод Миллера:")
numbers, rejected = [], []
k = 0
while len(numbers) < 10:
    n, q = build_miller(primes, bits)
    if test_miller(n, test_iterations, q):
        if n not in numbers:
            numbers.append(n)
    else:
        rejected.append(n)
for num in rejected:
    if test_miller(num, test_iterations, primes):
        k += 1
print("|  №  |   Число   | Проверка |  k  |")
print("------------------------------------")
for i, num in enumerate(numbers):
    flag = test_miller(num, test_iterations, primes)
    print(f"| {i+1:3} | {num:9} |    {'+' if flag else '-'}     | {k:3} |")

# Метод Поклингтона
print("\nМетод Поклингтона:")
numbers, rejected = [], []
k = 0
while len(numbers) < 10:
    n, q = build_pocklington(primes, bits)
    if test_pocklington(n, test_iterations, q):
        if n not in numbers:
            numbers.append(n)
    else:
        rejected.append(n)
for num in rejected:
    if test_pocklington(num, test_iterations, primes):
        k += 1
print("|  №  |   Число   | Проверка |  k  |")
print("------------------------------------")
for i, num in enumerate(numbers):
    flag = test_pocklington(num, test_iterations, primes)
    print(f"| {i+1:3} | {num:9} |    {'+' if flag else '-'}     | {k:3} |")

# Метод ГОСТ
print("\nМетод ГОСТ:")
numbers, rejected = [], []
k = 0
while len(numbers) < 10:
    p = build_gost(primes, bits)
    if test_miller(p, test_iterations, primes):
        if p not in numbers:
            numbers.append(p)
    else:
        rejected.append(p)
for num in rejected:
    if test_miller(num, test_iterations, primes):
        k += 1
print("|  №  |   Число   | Проверка |  k  |")
print("------------------------------------")
for i, num in enumerate(numbers):
    flag = test_miller(num, test_iterations, primes)
    print(f"| {i+1:3} | {num:9} |    {'+' if flag else '-'}     | {k:3} |")
