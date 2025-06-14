import math

print(f"| {'x':>4} | {'y':>6} |")
print("-----------------")

x = -4
while x <= 10 + 1e-6:
    if -4 <= x <= -2:
        y = x + 3
    elif -2 < x <= 4:
        y = (-0.5) * (x + 2) + 1
    elif 4 < x <= 6:
        y = -2
    else:
        under_sqrt = 4 - (x - 8) ** 2
        if under_sqrt >= 0:
            y = math.sqrt(under_sqrt) - 2

    print(f"| {x:4.1f} | {y:6.3f} |")
    x += 0.5
