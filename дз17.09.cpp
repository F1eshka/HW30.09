#include "superkroshka.h"

int main()
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); //уникальный номер окна консоли (дескриптор)
	system("title Maze"); // меняем название файла в консоли

	// скрываем мигающий курсор в консоли
	CONSOLE_CURSOR_INFO info;
	info.bVisible = false;
	info.dwSize = 100;
	SetConsoleCursorInfo(h, &info);

	srand(time(0));
	const int width = 60, height = 20;
	int maze[height][width] = {};

	// коридоры - 0
	// стены - 1
	// монетки - 2
	// враги - 3
	// аптечки - 4

	///////////////////////////////////////////////////////////////////////
	// установка лабиринта

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			maze[y][x] = rand() % 5;

			// установка стен
			if (x == 0 || y == 0 || x == width - 1 || y == height - 1) {
				maze[y][x] = WALL;
			}

			// определение точки входа и выхода
			if (x == 0 && y == 2 || x == 1 && y == 2 || x == 2 && y == 2 || x == width - 1 && y == height - 3 || x == width - 2 && y == height - 3 || x == width - 3 && y == height - 3) {
				maze[y][x] = HALL;
			}

			if (maze[y][x] == ENEMY)
			{
				int r = rand() % 10; // 0...9
				// если выпадает 0 - враг остаётся (10% случаев)
				if (r != 0) // если выпадает не 0 (а число от 1 до 9) - врага убираем (вероятность 90%)
				{
					maze[y][x] = HALL; // убираем врага
				}
			}
			else if (maze[y][x] == MEDICINE_KIT)
			{
				int v = rand() % 20; // 0...19
				// если выпадает 0 - аптечка остаётся (5% случаев)
				if (v != 0) // если выпадает не 0 (а число от 1 до 19) - аптечку убираем (вероятность 95%)
				{
					maze[y][x] = HALL; // убираем аптечку
				}
			}

			if (maze[y][x] == HALL) {
				SetCursor(x, y, 0, " "); // черные
			}
			else if (maze[y][x] == WALL) {
				SetCursor(x, y, 8, "#");
			}
			else if (maze[y][x] == COIN) {
				SetCursor(x, y, 14, ".");
			}
			else if (maze[y][x] == ENEMY) {
				SetCursor(x, y, 12, "o");
			}
			else if (maze[y][x] == MEDICINE_KIT) {
				SetCursor(x, y, 2, "+");
			}
			else {
				cout << maze[y][x];
			}
		}
		cout << endl;
	}

	///////////////////////////////////////////////////////////////////////
	// управление персонажем

	// установка курсора в любую координату консоли
	COORD position; // координаты гг
	position.X = 0;
	position.Y = 2;
	SetConsoleCursorPosition(h, position);
	SetConsoleTextAttribute(h, PINK); // розовый
	cout << "o";

	int coins = 0; // количество подобраных монет
	int health = 100; // здоровье
	bool lose = false; // индикатор проигрыша
	bool write = false; // показатель прошлого шага (нужно ли вернуть аптечку или нет)

	while (true) {

		///////////////////////////////////////////////////////////////////////
		// показатель здоровья

		COORD health_info;
		health_info.X = width + 1;
		health_info.Y = 3;
		SetConsoleCursorPosition(h, health_info);
		SetConsoleTextAttribute(h, YELLOW); // розовый
		cout << "HEALTH - " << health << "%";

		///////////////////////////////////////////////////////////////////////
		// передвижение

		int code = _getch(); // get character - получение кода нажатой клавиши
		if (code == 224) // если была нажата стрелочка
		{
			code = _getch();
		}

		SetConsoleCursorPosition(h, position);
		cout << " ";

		COORD kit_pos; // создание пустых координат аптечки
		kit_pos.X = 0;
		kit_pos.Y = 0;
		if (write) // 2 этап процесса возврата аптечки
		{
			kit_pos.X = position.X; // записывание координат аптечки (до шага)
			kit_pos.Y = position.Y;
			write = false; // обнуление переменной после записи координат
		}

		if (code == RIGHT && maze[position.Y][position.X + 1] != WALL) {
			position.X++; // смещение вправо
		}
		else if (code == LEFT && maze[position.Y][position.X - 1] != WALL) {
			position.X--; // смещение влево
		}
		else if (code == UP && maze[position.Y - 1][position.X] != WALL) {
			position.Y--; // смещение вверх
		}
		else if (code == DOWN && maze[position.Y + 1][position.X] != WALL) {
			position.Y++; // смещение вниз
		}

		if (kit_pos.X > 0) // если доступны координаты аптечки, возвращаем ее на место (3 этап процесса возврата аптечки)
		{
			SetConsoleCursorPosition(h, kit_pos);
			SetConsoleTextAttribute(h, GREEN);
			cout << "+";
		}

		if (maze[position.Y][position.X] == ENEMY) // проверка наличия врага
		{
			health -= 25;
			maze[position.Y][position.X] = HALL;
			health_info.X += 12;
			SetConsoleCursorPosition(h, health_info);
			SetConsoleTextAttribute(h, YELLOW);
			cout << " ";
			if (health == 0) {
				lose = true;
				MessageBoxA(0, "Закончилось здоровье!", "Поражение :(", MB_OK);
				break;
			}
		}

		if (maze[position.Y][position.X] == MEDICINE_KIT && health != 100) // использование аптечки
		{
			health += 25;
			maze[position.Y][position.X] = HALL;
		}
		else if (maze[position.Y][position.X] == MEDICINE_KIT && health == 100) // 1 этап процесса возврата аптечки
		{
			write = true;
		}

		SetConsoleCursorPosition(h, position); // установка курсора в новые коорды
		SetConsoleTextAttribute(h, PINK); // розовый
		cout << "o";

		///////////////////////////////////////////////////////////////////////
		// проверка на выход из лабиринта

		if (position.X == width - 1 && position.Y == height - 3) {
			MessageBoxA(0, "Найден выход из лабиринта!", "ПОБЕДА!", MB_OK);
			break;
		}

		///////////////////////////////////////////////////////////////////////
		// проверка на подбор монетки

		if (maze[position.Y][position.X] == COIN) {
			coins++;
			maze[position.Y][position.X] = HALL;
			COORD coins_info; // отдельная переменная отвечающая за координаты сообщения о количестве монет
			coins_info.X = width + 1; // х = 61
			coins_info.Y = 1; // y = 1
			SetConsoleCursorPosition(h, coins_info);
			SetConsoleTextAttribute(h, YELLOW); // розовый
			cout << "COINS - " << coins;
		}
	}

	system("cls"); // очистка экрана
	if (lose == false) {
		cout << "ТЫ ВЫИГРАЛ МИЛЛИОН!!!\n\n"; // main() - перезапуск приложения
	}
	else {
		cout << "НЕ СУДЬБА:(\n\n"; // main() - перезапуск приложения
	}
	Sleep(INFINITE); // пауза до того момента пока пользователь не закроет программу
}

// удалось с божьей помощью сократить код на 20 строк