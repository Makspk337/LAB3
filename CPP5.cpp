#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>

using namespace std;

void PrintTable(double Temp, double TempS, double r, int minutes) {
    setlocale(LC_ALL, "rus");
    cout << "| Время | Температура |" << endl;
    cout << "-----------------------" << endl;

    for (int minute = 0; minute <= minutes; ++minute) {
        double temperature = TempS + (Temp - TempS) * exp(-r * minute);
        cout << "| " << setw(5) << minute << " | " << setw(11) << fixed << setprecision(2) << temperature << " |" << endl;
    }
}

int main() {
    setlocale(LC_ALL, "rus");
    double Temp;
    double TempS;
    double r;
    int minutes;

    cout << "Введите исходную температуру кофе: ";
    cin >> Temp;

    cout << "Введите температуру окружающей среды: ";
    cin >> TempS;

    cout << "Введите значение коэффициента охлаждения: ";
    cin >> r;

    cout << "Введите количество минут для наблюдения: ";
    cin >> minutes;

    PrintTable(Temp, TempS, r, minutes);

    return 0;
}
