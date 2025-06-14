#include <iostream>
#include <vector>
#include <tuple>
#include <cmath>
#include <random>
#include <algorithm>
#include <iomanip>

using namespace std;

mt19937 mtRand(random_device{}());

vector<int> isPrime(int n) {
    vector<bool> isPrime(n + 1, true);
    vector<int> primesList;

    for (int p = 2; p * p <= n; ++p) {
        if (isPrime[p]) {
            for (int i = p * p; i <= n; i += p)
                isPrime[i] = false;
        }
    }

    for (int p = 2; p <= n; ++p)
        if (isPrime[p])
            primesList.push_back(p);

    return primesList;
}

int powMod(int a, int b, int n) {
    long long result = 1;
    a %= n;

    while (b > 0) {
        if (b & 1) result = (result * a) % n;
        a = (a * 1LL * a) % n;
        b >>= 1;
    }

    return static_cast<int>(result);
}

int rn(int a, int b) {
    return uniform_int_distribution<int>(a, b)(mtRand);
}

double rd(double a, double b) {
    return uniform_real_distribution<double>(a, b)(mtRand);
}

bool testMiller(int n, int t, const vector<int>& q) {
    if (n <= 1) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;

    int d = n - 1;
    int s = 0;
    while (d % 2 == 0) {
        d /= 2;
        s++;
    }

    for (int i = 0; i < t; ++i) {
        int a = rn(2, n - 2);
        long long x = powMod(a, d, n);

        if (x == 1 || x == n - 1) continue;

        bool isPrime = false;
        for (int r = 1; r < s; ++r) {
            x = powMod(x, 2, n);
            if (x == n - 1) {
                isPrime = true;
                break;
            }
        }

        if (!isPrime) return false;
    }

    return true;
}

bool testPocklington(int n, int t, const vector<int>& q) {
    vector<int> a;
    while (a.size() < t) {
        int aj = rn(1, n - 1);
        if (find(a.begin(), a.end(), aj) == a.end())
            a.push_back(aj);
    }

    for (int aj : a)
        if (powMod(aj, n - 1, n) != 1)
            return false;

    for (int aj : a) {
        bool ok = true;
        for (int qi : q)
            if (powMod(aj, (n - 1) / qi, n) == 1) {
                ok = false;
                break;
            }
        if (ok) return true;
    }

    return false;
}

pair<int, vector<int>> buildMiller(const vector<int>& prime, int bit) {
    int maxIndex = 0;
    while (maxIndex < prime.size() && prime[maxIndex] < (1 << (bit - 1)))
        maxIndex++;

    int m = 1;
    vector<int> q;

    while (true) {
        int num = rn(0, maxIndex - 1);
        int power = rn(1, (int)log2(1 << (bit - 1)));
        long long temp = pow(prime[num], power);

        if (temp > 0 && m <= INT32_MAX / temp) {
            m *= temp;
            q.push_back(prime[num]);
        }

        if (m > (1 << (bit - 2))) {
            if (m >= (1 << (bit - 1))) {
                m = 1;
                q.clear();
            } else break;
        }
    }

    return {2 * m + 1, q};
}

pair<int, vector<int>> buildPocklington(const vector<int>& prime, int bit) {
    int maxIndex = 0;
    while (maxIndex < prime.size() && prime[maxIndex] < (1 << ((bit / 2) + 1)))
        maxIndex++;

    int f = 1;
    vector<int> q;

    while (true) {
        int num = rn(0, maxIndex - 1);
        int power = rn(1, (int)log2(1 << ((bit / 2) + 1)));
        long long temp = pow(prime[num], power);

        if (temp > 0 && f <= INT32_MAX / temp) {
            f *= temp;
            q.push_back(prime[num]);
        }

        if (f > (1 << (bit / 2))) {
            if (f >= (1 << ((bit / 2) + 1))) {
                f = 1;
                q.clear();
            } else break;
        }
    }

    int R;
    do {
        R = rn((1 << ((bit / 2) - 1)) + 1, (1 << (bit / 2)) - 1);
    } while (R % 2 != 0);

    return {R * f + 1, q};
}

int buildGOST(const vector<int>& prime, int bit) {
    int maxIndex = 0;
    while (maxIndex < prime.size() && prime[maxIndex] < (1 << (bit / 2)))
        maxIndex++;
    
    int q = prime[rn(0, maxIndex - 1)];

    while (true) {
        double epsilon = rd(0, 1);
        double n0 = (pow(2, bit - 1) / q) + (pow(2, bit - 1) * epsilon / q);
        int n = ceil(n0);
        if (n % 2 != 0) n++;

        for (int u = 0; u < 1000; u += 2) {
            int p = (n + u) * q + 1;
            if (p > (1 << bit)) break;

            if (powMod(2, p - 1, p) == 1 && powMod(2, n + u, p) != 1)
                return p;
        }
    }
}

int calcIterations(double errorProbability) {
    return static_cast<int>(ceil(log(errorProbability) / log(0.25)));
}

int main() {
    vector<int> primes = isPrime(500);
    int bits;

    cout << "Введите размер чисел в битах (2-31): ";
    cin >> bits;

    if (bits < 2 || bits > 31) {
        cout << "Ошибка: некорректный размер!" << endl;
        return 1;
    }

    const double targetError = 0.1;
    const int testIterations = calcIterations(targetError);
    cout << "Используем " << testIterations << " итераций теста для вероятности ошибки <= " << targetError << endl;

    // Метод Миллера
    {
        vector<int> numbers;
        vector<int> rejectedNumbers;
        int k = 0;

        while (numbers.size() < 10) {
            auto [n, q] = buildMiller(primes, bits);
            bool isPrime = testMiller(n, testIterations, q);
            if (isPrime) {
                if (find(numbers.begin(), numbers.end(), n) == numbers.end()) {
                    numbers.push_back(n);
                }
            } else {
                rejectedNumbers.push_back(n);
            }
        }

        for (int num : rejectedNumbers) {
            if (testMiller(num, testIterations, primes)) {
                k++;
            }
        }

        cout << "\nМетод Миллера:\n";
        cout << "|  №  |   Число   | Проверка |  k  |\n";
        cout << "------------------------------------\n";

        for (size_t i = 0; i < numbers.size(); ++i) {
            bool isPrime = testMiller(numbers[i], testIterations, primes);
            cout << "| " << setw(3) << i + 1 << " | " << setw(9) << numbers[i]
                 << " |    " << (isPrime ? "+" : "-") << "     | " << setw(3) << k << " |\n";
        }
    }

    // Метод Поклингтона
    {
        vector<int> numbers;
        vector<int> rejectedNumbers;
        int k = 0;

        while (numbers.size() < 10) {
            auto [n, q] = buildPocklington(primes, bits);
            bool isPrime = testPocklington(n, testIterations, q);
            if (isPrime) {
                if (find(numbers.begin(), numbers.end(), n) == numbers.end()) {
                    numbers.push_back(n);
                }
            } else {
                rejectedNumbers.push_back(n);
            }
        }

        for (int num : rejectedNumbers) {
            if (testPocklington(num, testIterations, primes)) {
                k++;
            }
        }

        cout << "\nМетод Поклингтона:\n";
        cout << "|  №  |   Число   | Проверка |  k  |\n";
        cout << "------------------------------------\n";

        for (size_t i = 0; i < numbers.size(); ++i) {
            bool isPrime = testPocklington(numbers[i], testIterations, primes);
            cout << "| " << setw(3) << i + 1 << " | " << setw(9) << numbers[i]
                 << " |    " << (isPrime ? "+" : "-") << "     | " << setw(3) << k << " |\n";
        }
    }

    // Метод ГОСТ
    {
        vector<int> numbers;
        vector<int> rejectedNumbers;
        int k = 0;

        while (numbers.size() < 10) {
            int p = buildGOST(primes, bits);
            bool isPrime = testMiller(p, testIterations, primes);
            if (isPrime) {
                if (find(numbers.begin(), numbers.end(), p) == numbers.end()) {
                    numbers.push_back(p);
                }
            } else {
                rejectedNumbers.push_back(p);
            }
        }

        for (int num : rejectedNumbers) {
            if (testMiller(num, testIterations, primes)) {
                k++;
            }
        }

        cout << "\nМетод ГОСТ:\n";
        cout << "|  №  |   Число   | Проверка |  k  |\n";
        cout << "------------------------------------\n";

        for (size_t i = 0; i < numbers.size(); ++i) {
            bool isPrime = testMiller(numbers[i], testIterations, primes);
            cout << "| " << setw(3) << i + 1 << " | " << setw(9) << numbers[i]
                 << " |    " << (isPrime ? "+" : "-") << "     | " << setw(3) << k << " |\n";
        }
    }

    return 0;
}
