# earth_observation_satellites
Расчет наблюдений заданной точки на поверхности Земли спутниками ДЗЗ.

## Дано
- начальные условия для системы дифференциальных уравнений движения заданного состава КА в гринвичской относительной системе координат (ГОСК);
- геодезические координаты точки наблюдения;
- ограничения наблюдения по углу склонения Солнца;
- время начала и окончания расчета.

## Задачи
- Моделирование движения КА на основе системы дифференциальных уравнений в ГОСК и расчет трассы полета (геодезической широты и долготы) на расчетном интервале времени;
- Расчет угловых размеров мгновенных зон наблюдения;
- Расчет зон наблюдения заданной точки на поверхности Земли на расчетном интервале времени с учетом ограничений по углу склонения Солнца и представление результатов расчета в виде таблицы;
- Отображение на фоне карты Земли зон наблюдения заданной точки на поверхности Земли и текущего положения КА

## Алгоритм
Для выполнения поставленных задач задание было разбито на следующие этапы:
- Моделирование движения КА в АГЭСК и перевод полученных координат в ГОСК
- Расчет видимости заданной точки с учётом ограничений по углу склонения Солнца
- Расчет зоны видимости спутника в заданный момент времени
- Запись полученных данных в заданном формате
- Визуализация 

### Моделирование движения КА
Для моделирования системы спутников итеративно будет применяться метод Рунге-Кутты 4 порядка с шагом в 30 секунд (рассматривая каждый спутник как независимую систему Земля-Спутник). 
Будут использованы следующие ДУ в АГЭСК: 

&emsp; $\ddot{x} = -\mu \frac{x}{r^3}$ \
&emsp; $\ddot{y} = -\mu \frac{y}{r^3}$ \
&emsp; $\ddot{z} = -\mu \frac{z}{r^3}$ 

где

&emsp; $\mu = G(m_{earth}+m_{SC}) ≈ G m_{earth}=398600.4418109 m^3 s^{-2}$  (согласно ПЗ-90.11) \
&emsp; $\omega$ – угловая скорость вращения Земли.

Для перевода из АГЭСК в ГОСК используются следующие формулы:

&emsp; $x = X cos(S) - Y sin(S)$ \
&emsp; $y = X sin(S) + Y cos(S)$ \
&emsp; $z = Z$ 

Из них выражаем X, Y, Z - координаты в ГОСК. Легко видеть, что для получения X и Y достаточно умножить вектор (x, y) на обратную матрицу поворота.

$S$ – Гринвичское звездное время, для вычисления которого применяются следующие формулы: \
$S=(S_0+UT)$ \
$S_0= 6h 41m 50.54841s + 8640184.812866s \cdot t + 0.093104s \cdot t^2 - 6.2*10^{-6} s \cdot t^3 $

Здесь $t=d/36525$, d - количество целых суток от эпохи J2000.0: $d = JD - 2451545$. Юлианская дата эпохи J2000.0 равна 2451545.0. UT - текущее время в секундах.

### Расчёт зон видимости и видимости точки
Методы расчёта приведены в отчёте по [ссылке](https://docs.google.com/document/d/1PuSHMyRQongVIhd1YL1GoUV1zVNPdF1jL89NMzoKtUs/edit?usp=sharing)

## Визуализация
Для визуализации написана программа на Python. По записанным в ходе выполнения программы данным в файлы, строятся траектории и трассы полёта спутников. На участках, на которых точка наблюдения была видима, трассы рисуется более яркими точками, чем остальная трасса. Зона видимости обрисовывается красным цветом. Наблюдаемая зона отмечается ярко-голубым цветом.

## Результаты
Предоставим несколько примеров работы программы. Необходимо собрать программу при помощи cmake, запустить её, после чего запустить скрипт отрисовки результатов. Так, находясь в папке build в директории программы, необходимо выполнить следующие команды: `cmake .. ; make ; ./run ; python3 ../visualize/visualize.py`

__Пример 1.__ \
Исходные данные для прогноза:
- Расчётное время прогноза: 3 часа
- Юлианская дата: JD = 2460206.90 
- Кеплеровские элементы орбиты
  - Спутник 1
    - Большая полуось: 6878136
    - Эксцентриситет: 0.01
    - Наклонение: 45
    - Долгота восходящего узла: 45
    - Аргумент перицентра: 60
    - Истинная аномалия: 0
  - Спутник 2
    - Большая полуось: 6978136
    - Эксцентриситет: 0.01
    - Наклонение: 45
    - Долгота восходящего узла: 240
    - Аргумент перицентра: 225
    - Истинная аномалия: 0
  - Спутник 3
    - Большая полуось: 7878136
    - Эксцентриситет: 0.01
    - Наклонение: 105
    - Долгота восходящего узла: 60
    - Аргумент перицентра: 45
    - Истинная аномалия: 0
- Геодезические координаты точки наблюдения 40, 48, 0
- Ограничения по углу склонения Солнца 10. 
- Угол обзора для всех спутников составляет 30

Результаты моделирования приведены ниже.

![satellites_orbit_1](https://github.com/u5ele55/earth_observation_satellites/assets/99137907/aad8e198-5c88-4926-8244-2cc7d908b9ea)

![satellites_trace_1](https://github.com/u5ele55/earth_observation_satellites/assets/99137907/90afb7a9-0485-4db0-93ff-3aa86aefba23)


__Пример 2.__ \
Исходные данные те же, за исключением Юлианской даты.
- JD = 2460206.369

![satellites_trace_2](https://github.com/u5ele55/earth_observation_satellites/assets/99137907/dcbd621d-e6d0-444f-835c-423eb5a0e2bd)

В данном примере ограничения по углу склонения Солнца препятствуют видимости зоны.
