import math

def print_table(Temp, TempS, r, minutes):
    print("| Время | Температура |")
    print("-----------------------")
    for minute in range(minutes + 1):
        temperature = TempS + (Temp - TempS) * math.exp(-r * minute)
        print(f"| {minute:5.2f} | {temperature:11.2f} |")
        minute += 1

Temp = float(input("Введите исходную температуру кофе: "))
TempS = float(input("Введите температуру окружающей среды: "))
r = float(input("Введите значение коэффициента охлаждения: "))
minutes = int(input("Введите время: "))

print_table(Temp, TempS, r, minutes)
