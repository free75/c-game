#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 20
#define HEIGHT 12

#define EMPTY 0
#define WALL 1
#define COIN 2

HANDLE aBuffer[2];
int Buffer = 0;

int px = 1;
int py = 1;

int gx = 10;
int gy = 5;

int score = 0;
int frame = 0;
int gameover = 0;
int clear = 0;

int map[HEIGHT][WIDTH] = {
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
	{1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,2,1},
	{1,2,0,0,0,2,0,0,0,2,0,0,0,2,0,0,0,2,2,1},
	{1,2,1,1,1,2,1,1,1,1,1,1,1,2,1,1,1,2,2,1},
	{1,2,0,0,0,2,0,0,0,0,0,0,0,2,0,0,0,2,2,1},
	{1,2,1,1,1,2,1,1,1,1,1,1,1,2,1,1,1,2,2,1},
	{1,2,2,2,2,2,2,2,2,0,2,2,2,2,2,2,2,2,2,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

void clear() {
	aBuffer[0] = (
		GENERIC_READ | GENERIC_WRITE,
		0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL
		);
	aBuffer[1] = (
		GENERIC_READ | GENERIC_WRITE,
		0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL
		);

	SetConsoleActiveScreenBuffer(aBuffer[0]);

	CONSOLE_CURSOR_INFO cursor = { 1, FALSE };
	SetConsoleCursorInfo(aBuffer[0], &cursor);
	SetConsoleCursorInfo(aBuffer[1], &cursor);

	srand(time(NULL));
}

void render(HANDLE h, int x, int y, const char* character, int color) {
	COORD position = { x, y };
	SetConsoleCursorPosition(h, position);
	SetConsoleTextAttribute

}

