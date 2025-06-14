#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;
int main() {
    cout << "|   x  |    y   |" << endl;
    cout << "-----------------" << endl;

    for (double x = -4; x <= 10 + 1e-6; x += 0.5) {
        double y = 0.0;

        if (x >= -4 && x <= -2) {
            y = x + 3;
        } else if (x > -2 && x <= 4) {
            y = (-0.5) * (x + 2) + 1;
        } else if (x > 4 && x <= 6) {
            y = -2;
        } else {
            double under_sqrt = 4 - pow(x - 8, 2);
            y = sqrt(under_sqrt) - 2;
        }

        cout << "| " << setw(4) << fixed << setprecision(1) << x << " | " << setw(6) << setprecision(3) << y << " |" << endl;
    }

    return 0;
}
