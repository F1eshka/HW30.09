#include <iostream>
#include <string>
#include <ctime>
#include <Windows.h> //окрашивание
#include <conio.h> // configurate input output
using namespace std;

enum Derections { LEFT = 75, RIGHT = 77, DOWN = 80, UP = 72 };
enum colors { BLACK = 0, GRAY = 8, YELLOW = 14, RED = 12, PINK = 13, GREEN = 2 };
enum elems { HALL = 0, WALL = 1, COIN = 2, ENEMY = 3, MEDICINE_KIT = 4 };

void SetCursor(int x, int y, int color, string text) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD position;
	position.X = x;
	position.Y = y;
	SetConsoleCursorPosition(h, position);
	SetConsoleTextAttribute(h, color);
	cout << text;
}