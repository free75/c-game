#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define WIDTH 20
#define HEIGHT 10

//맵 설정 * (나중에 오류 고치기)

char map[HEIGHT][WIDTH] = {
	"####################",
	"##X  .......##    ##",
	"##### ## ##....## ##",
	"##.......##.##.## ##",
	"## ##.##....##....##",
	"## ##.##.##.##.##.##",
	"#........##.......##",
	"#.### ##  ###  ##.##",
	"#.    ###X  ......##",
	"####################",
};

// 오브젝트 위치 * (플레이어 위치, 고스트(방해몹) 위치)

int x = 3;
int y = 2;
int gx = 10;
int gy = 10;

// 점수창 명령어

int score = 0;
HANDLE console;

// 색상 추가 명령어 (나중에)

// 초기화
void clear() {
	console = GetStdhandle(STD_OUTPUT_HANDLE);
    srand(time(NULL));

	// 콘솔 창의 커서를 숨기기 위해 사용한 명령어
	CONSOLE_CURSOR_INFO cursor;

	cursor.bVisible = FALSE;

	GetConsoleCursorInfo(console, &cursor);

}
// 커서 이동을 선언하는 명령어

void go(int x, int y) {
	COORD

}

int Move(int x, int y) {


}






int main()
{







	return 0;
}