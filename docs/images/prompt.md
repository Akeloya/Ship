# Промпты монтажной иллюстрации

Инструмент: встроенный image_gen. Итог — assembly-guide.png. Первая версия заменена после проверки соединений.

## Исходный промпт

Use case: infographic-diagram.
Create a high resolution landscape Russian illustrated electronics assembly poster for a model ship workbench, white background, professional clear technical layout, recognizable top-down component illustrations but explicitly schematic terminal blocks rather than unreliable real header positions. Large readable labels and orthogonal colored wiring, no decorative cables. Title "Стенд корабля: UNO + Nano". Four distinct panels: motor signal wiring upper left; servo signal wiring upper right; power distribution lower left; essential notes lower right. Keep all content within margins.
Exact electrical specification:
Panel "1. Моторы — UNO": joystick 1 terminals VCC, GND, VRx, VRy, SW linked respectively to UNO 5V, GND, A0, A1, D2. UNO to L298N table alongside illustrated boards: D5→ENA; D7→IN1; D8→IN2; D6→ENB; D9→IN3; D10→IN4. Show each route with matching labels, avoid ambiguity. L298N OUT1 and OUT2 go to two separate terminals of left DC motor. OUT3 and OUT4 go to right DC motor. Motors marked "12 В • 12 000 об/мин". No propellers drawn. Two resistors each 10 кОм, one ENA to GND, other ENB to GND. Note "Снять ENA, ENB и 5V-EN". Small note "SW: удерживать для хода".
Panel "2. Серво — Nano": joystick 2 VCC→Nano 5V, GND→GND, VRx→A0, VRy→A1, SW→D2. Nano D3 through 1 кОм resistor→servo 1 signal, Nano D4 through 1 кОм→servo 2 signal. Servo red positive wires labelled "+5V_SERVO", brown grounds labelled GND, never connect servo power to Nano. Servo labels "Руль" and "Механизм". Note "SW: вернуть оба серво в центр".
Panel "3. Питание": distinct flow diagram: source +12V→F1→S0 (аварийный выключатель)→split. Branch 1 F2→L298N VS (+12V). Branch 2 F3→DC/DC 12→5 В→+5V_SERVO. USB 1→UNO and USB 2→Nano separately. UNO 5V→L298N 5V logic ONLY with 5V-EN removed, annotate "Логика, 5V-EN снята". Both joysticks powered by their corresponding board only. Source minus, UNO GND, Nano GND, L298N GND, converter IN−/OUT− and servo grounds all join a BLACK ground bus "Общая GND"; do not connect positive rails together. Show capacitors as labelled small callouts: "VS–GND: 470–1000 мкФ, ≥25 В"; "+5V_SERVO–GND: 470–1000 мкФ, ≥10 В"; "На выводах каждого мотора: 100 нФ". Do not accidentally short capacitor leads.
Panel "Перед включением": exact short notes "Первый запуск — без винтов"; "Не соединять +5V_SERVO с 5V Arduino"; "Не соединять 5V двух Arduino"; "Ток моторов неизвестен: проверить пригодность L298N"; "Контакты показаны условно: сверять маркировку платы и README". Legend: red "+12 В", orange "+5 В", black "GND", blue/green "Сигналы". Use endpoint net labels to simplify wiring instead of long crossing cables. Diagram must be clear enough for novice assembly. If space constrained, prioritize exact pin tables and power isolation over photorealistic detail. No unlisted modules, no breadboard power wiring, no spurious extra connections.

## Уточняющий промпт итоговой версии

Edit this assembly poster to eliminate misleading electrical wiring. Keep Russian title, component illustrations, clear tables, four-panel layout. CRITICAL: remove ALL drawn connecting wires between components everywhere, including all wires to terminals and all power bus lines. This is an illustrated connection-table poster; no circuit wires except isolated component symbols. D3 and D4 must NEVER share a line. Power 5V and GND must NEVER appear shorted. Tables exclusively define connectivity.
Panel 1 keep joystick/UNO/L298N/motors as disconnected recognizable illustrations with no cables; keep both correct pin tables. Add compact table "L298N → моторы": OUT1/OUT2 → левый мотор; OUT3/OUT4 → правый мотор. Keep independent isolated resistor symbols ENA–10 кОм–GND and ENB–10 кОм–GND only, separated. Keep "Снять ENA, ENB и 5V-EN".
Panel 2 disconnected joystick/Nano/servo illustrations no wires. Keep pin tables. Add "Красный провод серво → +5V_SERVO", "Минус серво → общая GND". Signal table D3 → 1 кОм → сигнал серво 1; D4 → 1 кОм → сигнал серво 2.
COMPLETELY REDESIGN lower left power panel. No schematic routes or bus lines, only large numbered colored text cards with EXACT unambiguous connections:
"1  Источник +12 В → F1 → S0 → узел +12V_SW"
"2  +12V_SW → F2 → L298N VS"
"3  +12V_SW → F3 → DC/DC IN+"
"4  DC/DC OUT+ (5,0 В) → плюс обоих серво"
"5  USB 1 → UNO; USB 2 → Nano"
"6  UNO 5V → L298N 5V (5V-EN снята)"
"7  UNO 5V → VCC джойстика 1"
"8  Nano 5V → VCC джойстика 2"
"Общая GND: минус источника, UNO GND, Nano GND, L298N GND, DC/DC IN− и OUT−, минусы серво, GND обоих джойстиков."
Then smaller capacitor box text only: "VS–GND: 470–1000 мкФ, ≥25 В"; "OUT+–OUT− DC/DC: 470–1000 мкФ, ≥10 В"; "Между выводами каждого мотора: 100 нФ".
Keep lower right safety panel with no changes except source note "Первый запуск: источник с ограничением тока". This must be readable, uncluttered, all text exact. No electrical lines except arrows WITHIN text/table entries. Do not invent wiring, do not draw a battery as preferred supply.

