Цели и задачи работы: изучение циклических алгоритмов, операторов цикла, программирование циклического вычислительного процесса.
Задание к работе:
Реализовать циклический вычислительный процесс. Самостоятельно решить задачи в соответствии с индивидуальным вариантом на двух языках программирования (С++ и другой (по согласованию с преподавателем)).
Задание 1. Вычислить и вывести на экран или в файл в виде таблицы значения функции, заданной графически, на интервале от Xнач до Xкон с шагом dx. Интервал и шаг задать таким образом, чтобы проверить все ветви программы.
Задание 2. Реализовать тесты на простоту согласно заданию.
Задание 3. Математическая сумма.
Задание 4. Увлекательная игра.
Задание 5*. Решить задачу об остывании чашки кофе из кофейни НГТУ.
Написать программу, моделирующую процесс остывания кофе. Программа должна позволять задавать все необходимые параметры. Построить графики или таблицы зависимостей величин. Считаем, что температуру кофе измеряли через некоторые, необязательно равномерные промежутки времени. Написать программу, которая бы по данным измерений строила линейную модель, вычисляя статистические критерии их адекватности.
Методика выполнения работы:
	определить типы используемых в программе данных;
	описать переменные;
	написать функции ввода-вывода;
	разработать алгоритм решения задачи по индивидуальному заданию;
	написать и отладить программу с вводом-выводом информации;
	протестировать работу программы на различных исходных данных;
	изменить формат вывода, проверить работу программы при другом формате вывода.
Требования к отчету:
	отчет должен содержать титульный лист, задание, текст программы с комментариями, тесты;
	контрольные вопросы по лабораторной преподаватель задает в процессе демонстрации студентом полученных навыков работы в интегрированной среде.
Перечень понятий к защите лабораторной работы 3:
Концепция типа данных. Переменная. Тип данных. Диапазон типов по стандарту. Описание идентификатора. Пространство имен. Классы идентификаторов. Основные правила использования переменных. Конвенции именования С++. Основные операции С++. Тернарная операция. Условный оператор. Обработка запросов по множеству enum. Оператор Switch.
Оператор цикла: тело цикла, параметр цикла, условие цикла, итерация, счетчик цикла. Условие цикла. Цикл с предусловием. Цикл с постусловием. Цикл с параметром (формат и определение каждого элемента условия). Правила использования циклов. 5 операторов передачи управления в цикле.
Задание №1
Вариант 25:
Параметры, необходимые для решения задания следует получить из графика и определить в программе.
 
Задание №2
Теоретическая часть
Доказуемо простые числа, как правило, используются в качестве модулей криптосистем, основанных на проблеме дискретного логарифмирования, таких как шифр Шамира, криптосистема Эль-Гамаля и связанные с ней стандарты цифровой подписи ГОСТ Р 34.11-94, ГОСТ Р 34.11- 2001, DSA и EСDSA. Рассмотрим тесты на простоту Миллера, Поклингтона, ГОСТ.
Задание для самостоятельного решения
	Построить таблицу простых чисел, меньших 500, с помощью решета Эратосфена. С использованием этой таблицы:
	реализовать процедуру получения простых чисел заданной длины на основе теста Миллера;
	реализовать процедуру получения простых чисел заданной длины на основе теста Поклингтона;
	реализовать процедуру генерации простых чисел заданной длины ГОСТ Р 34.10-94.
	Построить 10 простых чисел с помощью полученной процедуры.
	Каждое построенное число проверить на простоту вероятностным тестом, реализованным в задании к разделу 2. Количество итераций вероятностного теста должно быть таково, чтобы вероятность ошибки не превышала 0,1.
	Каждое отвергнутое тестом из пункта 1 число проверить вероятностным тестом. Подсчитать k – количество отвергнутых чисел, определенных вероятностным тестом как простые.
	Результат оформить в виде таблицы:
№	1	2	…	10
P	101		…	
Результат проверки вероятностным тестом	+	-	…	
K	2		…	

Здесь № - номер эксперимента, p – построенное простое число, в третьей строке результат проверки построенного числа вероятностным тестом (+ или -), k – количество отвергнутых чисел, определенных вероятностным тестом как простые.
Количество итераций вероятностного теста должно быть таково, чтобы вероятность ошибки не превышала 0,1.
Задание №3 «Математическая сумма»
На вход подается два целых числа a и b, каждое из которых находится в диапазоне от 1 до 10 включительно. На основе этих чисел необходимо вычислить сумму заданного бесконечного ряда:
Σ_(n=1)^∞   n^a/b^n 
Если ряд расходится (то есть его сумма стремится к бесконечности), выведите слово infinity. Если ряд сходится, и его сумма является рациональным числом, выведите её в виде несократимой дроби (например, 3/2). Если сумма ряда является иррациональным числом, выведите слово irrational.
Пример №1:                                                Пример №2:
Ввод – 1 2                                                Ввод – 2 1
Вывод – 2/1                                               Вывод – infinity
Задание №4 «Увлекательная игра»
Павел и Вика придумали увлекательную игру. Сначала они выписывают в тетрадь последовательность из n целых чисел (5 ≤ n ≤ 50 000). Игроки ходят по очереди: на каждом ходу игрок может выбрать от одного до m чисел (4 ≤ m ≤ 100) из начала последовательности, стереть их, и прибавить сумму стёртых чисел к своему счёту.
Игра заканчивается, когда все числа удалены из тетради, а победителем становится тот, у кого итоговый счёт больше. Гарантируется, что ничья невозможна.
Павел ходит первым, и оба игрока действуют оптимально. Необходимо вывести 1 в случае победы Павла и 0 — в случае победы Вики.
Пример №1:                                            Пример №2:
Ввод – 4 3                                               Ввод – 4 3
1 2 3 10                                                 1 2 3 -10
Вывод – 0                                                Вывод – 1
Задание №5* «Задача об остывании чашки кофе в кофейне НГТУ»
Написать программу, моделирующую процесс остывания кофе. Программа должна позволять задавать все необходимые параметры. Построить графики или таблицы зависимостей величин. Считаем, что температуру кофе измеряли через некоторые, необязательно равномерные промежутки времени. Написать программу, которая бы по данным измерений строила линейную модель, вычисляя статистические критерии их адекватности.
Природа переноса тепла от кофе к окружающему пространству сложна и в общем случае включает в себя механизмы конвекции, излучения, испарения и теплопроводности. В том случае, когда разность температур между объектом и окружающей средой не очень велика, скорость изменения Т объекта можно считать пропорциональной этой разности температур. Это утверждение формулируется на языке дифференциальных уравнений так:
dT⁄dt= -r(T-T_S ).
где Т – температура кофе,  T_S – температура окружающей среды, r – коэффициент остывания. Коэффициент остывания зависит от механизма передачи тепла, площади тела, находящегося в контакте со средой и тепловых свойств самого тела. Знак (–) появляется во избежание нефизического эффекта увеличения температуры тела, когда T > Ts. Данное соотношение называется законом теплопроводности Ньютона.


Структура проекта: CPP1.cpp/Python1.py - выполнение первого индивидуального задания на языке C++/Python (с остальными аналогично).
