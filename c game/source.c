#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define EMPTY 0
#define WALL 1
#define COIN 2

#define WIDTH 20
#define HEIGHT 12

HANDLE aBuffer[2];
int buffer = 0;

enum Gmaestate {
	MENU,
	PLAYING,
	CLEAR,
	FAIL
};

int gameState = PLAYING;


// pacman의 입과 몸을 C 와 O 로 표현하고 2프레임으로 구현할 명령어
char PacmanAnim[2] = {'C', 'O'};
int anim = 0;

// 오브젝트 위치 * (플레이어 위치, 고스트(방해몹) 위치)
// 배열문 선언 후에 콘솔 창에 나올 배열문에 함수를 포함해서
// 플레이어의 위치와 방해몹의 위치를 x,y로 선언

// 방해몹 확장
// 빨간색으로 표현한 G 방해몹 외에
// 조건부로 움직이며 플레이어를 방해는 방해몹 추가

// 팩맨 에니메이션 추가 이후
// 방향 관련 명령어 추가

int px = 1;
int py = 1;
int gx = 10;
int gy = 5;
int score = 0;

//맵 설정 * (나중에 오류 고치기)

int map[HEIGHT][WIDTH] = {
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
	{1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,1,2,1},
	{1,2,0,0,0,2,0,0,0,0,0,0,0,2,0,0,0,0,2,1},
	{1,2,1,1,1,2,1,1,1,1,1,1,1,2,1,1,1,1,2,1},
	{1,2,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,2,1},
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


	// 콘솔 창의 커서를 숨기기 위해 사용한 명령어
	CONSOLE_CURSOR_INFO cursor = { 1, FALSE };

	GetConsoleCursorInfo(aBuffer[0], &cursor);

	SetConsoleCursorInfo(aBuffer[1], &cursor);

	srand(time(NULL));

}

void render(HANDLE Buffer, int x, int y, char character, int color) {

	COORD position = { x, y };

	SetConsoleCursorPosition(Buffer, position);

	SetConsoleTextAttribute(Buffer, color);

	DWORD dword;

	WriteConsole(Buffer, &character, 1, &dword, NULL);
}

//void renderText(HANDLE Buffer, int x, int y, int score, int color) {
//	char character[2];
//	character[0] = score + '0';
//	character[1] = '\0';
//	render(Buffer, x, y, character, color);
//}

void renderNumber(HANDLE Buffer, int x, int y, int num) {

	if (num == 0) {
		render(Buffer, x, y, '0', 7);
		return;
	}

	char temp[10];
	int i = 0;

	while (num > 0) {
		temp[i++] = (num % 10) + '0';
		num /= 10;
	}

	for (int j = i - 1; j >= 0; j--) {
		render(Buffer, x++, y, temp[i], 7);
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
void player() {
    if (_kbhit()) {

		int key = _getch();
		int nx = px;
		int ny = py;

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

		// VK_SPACE 사용 후에 스테이지 넘어갈때 사용할 코드]
		if (key == 32) {
			if (gameState != PLAYING) {
				gameState = PLAYING;
				px = 1; py = 1;
				gx = 10; gy = 5;
				score = 0;
				
			}
		}
	}
}

void ghostmove() {

	int dx[4] = { 1, -1, 0, 0 };
	int dy[4] = { 0, 0, 1, -1 };

	int memory = 9999;
	int Memory = -1;

	for (int i = 0; i < 4; i++) {
		int nx = gx + dx[i];
		int ny = gy + dy[i];

		if (!go(nx, ny)) continue;

		int GHOST = (px - nx) * (px - nx) + (py - ny) * (py - ny);


		if (GHOST < memory) {
			memory = GHOST;
			Memory = i;
		}
	}

	if (Memory != -1) {
		gx += dx[Memory];
		gy += dy[Memory];
	}
}

// touch 코드의 명령어 방해몹과 플레이어가 붙었을 경우 실행되는 명령어
int touch() {
	if (px == gx && py == gy)
		gameState = FAIL;

	if (score >= 50)
		gameState = CLEAR;
}

void initialize() {

	HANDLE Buffer = aBuffer[buffer];

	DWORD dword;
	COORD position = { 0, 0 };
	FillConsoleOutputCharacter(Buffer, ' ', WIDTH * HEIGHT, position, &dword);
	FillConsoleOutputAttribute(Buffer, 7, WIDTH * HEIGHT, position, &dword);

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {

			if (map[y][x] == WALL) // if문 안에서 행렬을 설정할 때에는 설정한 배열문이 있다면 [y]를 y축을 시작으로, [x] x축으로 끝나야 정상 작동하게 됩니다.
				render(Buffer, x, y, '#', 8);
			else if (map[y][x] == COIN)
				render(Buffer, x, y, '.', 10);
			else
				render(Buffer, x, y, ' ', 7);

		}
	}

	if (gameState == PLAYING) {
		render(Buffer, px, py, PacmanAnim[anim], 14);
		render(Buffer, gx, gy, 'G', 12);
	} 

	render(Buffer, 0, HEIGHT - 1, 'S', 7);
	renderNumber(Buffer, 2, HEIGHT - 1, score);
	
    if (gameState == CLEAR) 
		render(Buffer, 5, 5, 'C', 10);
	

	if (gameState == FAIL) 
		render(Buffer, 8, 5, 'F', 12);
	

	SetConsoleActiveScreenBuffer(Buffer);
	Buffer = !Buffer;
}




// 게임 승리 명령어 추가

// 게임 오버 명령어 추가

// 추가하고 싶은 것
// 스테이지를 하나로만 단정 짓지 않고
// 스테이지 클리어 시 새로운 스테이지 생성
// 스테이지 클리어 후엔 clear와 어느 특정한 키를 누를 경우
// 새로운 스테이지와 새로운 맵에서 플레이 하도록 설정
// 특정한 키는 백 스페이스 VK_SPACE 로 설정
// VK_SPACE는 스테이지 클리어 판정이 나왔을 경우
// clear가 출력됨가 동시에 실행이 가능하도록 설정

int main()
{
	clear();

	int ghostTimer = 0;
	int PlayerTimer = 0;
 
	while (1) {

		player();

		if (gameState == PLAYING) {
			
			ghostTimer++;
			if (ghostTimer >= 3) {
				ghostmove();
				ghostTimer = 0;
			}

			PlayerTimer++;
			if (PlayerTimer >= 5) {
				anim = !anim;
				PlayerTimer = 0;
			}

			touch();
		}

		initialize();

		Sleep(60);
	
	}

	// 지금까지 적어놓은 코드를 계속 실행해보면서 플레이 해본 결과
	// 자주 끊김
	// Sleep을 사용한 것과 delay 명령어를 사용하면서 생긴 끈김이라 생각함
	// Sleep은 ghost의 속도 저하를 위해 선언했기에 변경 불가
	// 추가적으로 ghost 방해몹의 아이콘에도 에니메이션 추가

	return 0;
}