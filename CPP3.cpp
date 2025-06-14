#include <iostream>
#include <cmath>
#include <numeric>
#include <iomanip>

using namespace std;

// Функция для вычисления НОД
int gcd(int a, int b) {
    return b == 0 ? a : gcd(b, a % b);
}

// Проверка, является ли число почти целым
bool is_rational(double value, int &num, int &den, int max_den = 1000000) {
    for (int d = 1; d <= max_den; ++d) {
        int n = round(value * d);
        if (fabs(value - (double)n / d) < 1e-10) {
            int g = gcd(n, d);
            num = n / g;
            den = d / g;
            return true;
        }
    }
    return false;
}

int main() {
    int a, b;
    cout << "Введите числа: ";
    cin >> a >> b;

    if (b <= 1) {
        cout << "infinity" << endl;
        return 0;
    }

    double sum = 0.0;
    const double EPS = 1e-15;

    for (int n = 1; n < 1000000; ++n) {
        double term = pow(n, a) / pow(b, n);
        sum += term;
        if (term < EPS) break;
    }

    // Проверка на рациональность
    int num, den;
    if (is_rational(sum, num, den)) {
        cout << num << "/" << den << endl;
    } else {
        cout << "irrational" << endl;
    }

    return 0;
}

