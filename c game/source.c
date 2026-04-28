#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define EMPTY 0
#define WALL 1
#define COIN 2

#define WIDTH 20
#define HEIGHT 10

HANDLE aBuffer[2];
int buffer = 0;


//맵 설정 * (나중에 오류 고치기)

int map[HEIGHT][WIDTH] = {
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,1},
	{1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,2,1},
	{1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,1},
	{1,2,1,1,1,2,1,1,1,1,1,1,1,2,1,1,1,2,2,1},
	{1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,1},
	{1,2,1,1,1,2,1,1,1,1,1,1,1,2,1,1,1,2,2,1},
	{1,2,2,2,2,2,2,2,2,0,2,2,2,2,2,2,2,2,2,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

// 오브젝트 위치 * (플레이어 위치, 고스트(방해몹) 위치)
// 배열문 선언 후에 콘솔 창에 나올 배열문에 함수를 포함해서
// 플레이어의 위치와 방해몹의 위치를 x,y로 선언

int px = 1;
int py = 1;
int gx = 10;
int gy = 10;

// 점수창 명령어
// 배열문에 포함된 . 을 얻을 때마다, 혹은 계속 움직일 때마다
// 1포인트씩 획득할 수 있도록 설정

int score = 0;
HANDLE console;

// 색상 추가 명령어 (나중에)

// 초기화
void clear() {
	aBuffer[0] = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	aBuffer[1] = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	SetConsoleActiveScreenBuffer(aBuffer[0]);

	// 콘솔 창의 커서를 숨기기 위해 사용한 명령어
	CONSOLE_CURSOR_INFO cursor;

	cursor.dwSize = 1;

	cursor.bVisible = FALSE;

	GetConsoleCursorInfo(aBuffer[0], &cursor);

	SetConsoleCursorInfo(aBuffer[1], &cursor);

	srand(time(NULL));

}

// 커서 이동을 선언하는 명령어
// 어디에서 실행할지 결정하기 위해서 설정한 명령어

int go(int x, int y) {
	return map[y][x] != WALL;
}

void color(HANDLE handle, int color) {
	SetConsoleTextAttribute(handle, color);
}



void player(char key) {
    if (_kbhit()) {
		int key = _getch();
		int nx = px, ny = py;

		if (key == 224) {
			key = _getch();

			if (key == 72) ny--;
			if (key == 75) nx--;
			if (key == 77) nx++;
			if (key == 80) ny++;
		}
		if (go(nx, ny)) {
			px = nx;
			py = ny;

			if (map[px][py] == COIN) {
				score++;
				map[py][px] = EMPTY;

			}
		}
	}
}

void ghost(int nx, int ny) {
	ghost(int x, int y)




		return 0;
}




void ghostmove() {
	if (px < gx && ghost(gx - 1, gy)) return;
	if (px > gx && ghost(gx + 1, gy)) return;
	if (py < gy && ghost(gx, gy - 1)) return;
	if (py > gy && ghost(gx, gy + 1)) return;

	int willmove = rand() % 4;
	if (willmove == 0) ghost(gx, gy - 1);
	if (willmove == 1) ghost(gx, gy + 1);
	if (willmove == 2) ghost(gx - 1, gy);
	if (willmove == 3) ghost(gx - 1, gy);
}

void render(HANDLE Buffer, int x, int y, const char* character, int color) {

	COORD position = { x, y };

	SetConsoleCursorPosition(Buffer, position);

	//setcolor(Buffer, color);

	DWORD dword;

	WriteFile(Buffer, character, strlen(character), &dword, NULL);

}

int touch() {
	return (px == gx && py == gy);
}


int main()
{
	clear();

	int delay = 0;

	while (1) {
		player(1);

		delay++;

		if (delay % 3 == 0)
			ghostmove();

		if (touch()) break;

		Sleep (60);
	}


	return 0;
}