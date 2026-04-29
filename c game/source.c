#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define EMPTY 0
#define WALL 1
#define COIN 2

#define WIDTH 20
#define HEIGHT 11

HANDLE aBuffer[2];
int buffer = 0;

// 오브젝트 위치 * (플레이어 위치, 고스트(방해몹) 위치)
// 배열문 선언 후에 콘솔 창에 나올 배열문에 함수를 포함해서
// 플레이어의 위치와 방해몹의 위치를 x,y로 선언

int px = 1;
int py = 1;
int gx = 10;
int gy = 5;
int score = 0;

//맵 설정 * (나중에 오류 고치기)

int map[HEIGHT][WIDTH] = {
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,1},
	{1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,1,2,1},
	{1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,1},
	{1,2,1,1,1,2,1,1,1,1,1,1,1,2,1,1,1,1,2,1},
	{1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,1},
	{1,2,1,1,1,2,1,1,1,1,1,1,1,2,1,1,1,1,2,1},
	{1,2,2,2,2,2,2,2,2,0,2,2,2,2,2,2,2,2,2,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

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

void render(HANDLE Buffer, int x, int y, const char* character, int color) {

	COORD position = { x, y };

	SetConsoleCursorPosition(Buffer, position);

	SetConsoleTextAttribute(Buffer, color);

	DWORD dword;

	WriteFile(Buffer, character, strlen(character), &dword, NULL);

}

void renderText(HANDLE Buffer, int x, int y, int score, int color) {
	char character[2];
	character[0] = score + '0';
	character[1] = '\0';
	render(Buffer, x, y, character, color);
}

void renderNumber(HANDLE Buffer, int x, int y, int num, int color) {

	if (num == 0) {
		renderText(Buffer, x, y, 0, color);
		return;
	}

	int scores[10];
	int count = 0;

	while (num > 0) {
		scores[count++] = num % 10;
		num /= 10;
	}

	for (int i = count - 1; i >= 0; i--) {
		renderText(Buffer, x++, y, scores[i], color);
	}
}

// 커서 이동을 선언하는 명령어
// 어디에서 실행할지 결정하기 위해서 설정한 명령어

int go(int x, int y) {
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return 0;
	return map[y][x] != WALL;
}

// player가 방향키(UP, DOWN, RIGHT, LEFT)에 따라 어떻게 움직여야하는지 적을 명령어
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

			if (map[py][px] == COIN) {
				map[py][px] = EMPTY;
				score++;
			}
		}
	}
}

void ghostmove() {

	int ghost[4][2] = {            // 이전 방해몹 코드 보완 while문으로 대체함
		{0,-1},{0,1},{-1,0},{1,0}  // 따로 분리하지 않고 while 반복문으로 통일
		                           // while문 그리고 if문 사용
	};                             // while문 사용시 추적 방해몹이 위 아래로만 움직이는 것을 확인함
	                               // for문으로 대체

	int MAP = 999999;
	int GHOSTX = gx;
	int GHOSTY = gy;


	// 앞으로만 움직이는 거 방지차기 위해 만든 코드
	for (int i = 0; i < 4; i++) { // For문을 하나 더 선언함
		                          // 이유 방지를 위해서 continue와 함께 사용
		                          // rand와 배열문으로 방지 가능함

		int r = rand() % 4;

		int tx = ghost[i][0];
		int ty = ghost[i][1];

		ghost[i][0] = ghost[r][0];
		ghost[i][1] = ghost[r][1];
		
		ghost[r][0] = tx;
		ghost[r][1] = ty;




	}
	for (int i = 0; i < 4; i++) {

		int r = rand() % 4;

		int nx = gx + ghost[i][0];
		int ny = gy + ghost[i][1];

		if (!go(nx, ny)) continue;

		int dx = px - nx;
		int dy = py - ny;

		int Map = dx * dx + dy * dy;

		if (Map < MAP) {
			MAP = Map;
			GHOSTX = nx;
			GHOSTY = ny;

		}

		gx = GHOSTX;
		gy = GHOSTY;
	}

}

void initialize() {
	HANDLE Buffer = aBuffer[buffer];
	SetConsoleActiveScreenBuffer(Buffer);

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {

			if (map[y][x] == WALL) // if문 안에서 행렬을 설정할 때에는 설정한 배열문이 있다면 [y]를 y축을 시작으로, [x] x축으로 끝나야 정상 작동하게 됩니다.
				render(Buffer, x, y, "#", 8);
			else if (map[y][x] == COIN)
				render(Buffer, x, y, ".", 10);
			else
				render(Buffer, x, y, " ", 7);

		}
	}

	render(Buffer, px, py, "P", 14);
	render(Buffer, gx, gy, "G", 12);

	render(Buffer, 0, HEIGHT - 1, "Score", 7);
	renderNumber(Buffer, 7, HEIGHT - 1, score, 7);

	buffer = 1 - buffer;
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

		initialize();

		if (touch()) 
			break;

		Sleep (60);
	}


	return 0;
}