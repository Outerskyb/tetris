#pragma warning(disable:4996)
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<time.h>
#include<Windows.h>

#define WIDTH 10
#define HEIGHT 20
#define SPACE 32
#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80

int diff;
int score = 0;
int deleted_line = 0;
int dead_count = 0;
int status[WIDTH][HEIGHT];//해당칸에 블럭 있음 나타내기 있음:!0 없음:0

typedef struct point {
    int x;
    int y;
}point;

point current;

int  current_shape;

void gotoxy(int x, int y)//gotoxy
{
    COORD pos = { x - 1, y - 1 };//커서가 X좌표에서 -1 한값. Y좌표에서 -1한 값으로 이동
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void init_status(void) {
    int i, j;
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            status[i][j] = 0;
        }
    }
}

void difficult(int n) {
    int i, j;
    srand(time(NULL));
    for (i = 16; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            status[i][j] = rand() % n;
        }
    }
    system("CLS");
}

void init_tetromino(void) {
    current_shape = rand()%7;
    current.x = 4;//01234|중심|56789
    current.y = 0;
    if (down_check()) {
        exit(1);
    }
    make_tetromino();
}

void remove_cursor(void) {
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    curInfo.bVisible = 0;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

int make_tetromino(void) {
    switch (current_shape) {
    case 0://네모 만들기
    case 7:
    case 14:
    case 21:
        status[current.y + 0][current.x + 1] = 11;//1 0
        status[current.y + 0][current.x + 0] = 11;
        status[current.y + 1][current.x + 1] = 11;
        status[current.y + 1][current.x + 0] = 11;
        break;
    case 1: //작대기 만들기
    case 15:
        status[current.y + 0][current.x + 2] = 11;//2 -1
        status[current.y + 0][current.x + 1] = 11;
        status[current.y + 0][current.x + 0] = 11;
        status[current.y + 0][current.x - 1] = 11;
        break;
    case 2: //ㄴ만들기
        status[current.y + 0][current.x - 1] = 11;//1 -1
        status[current.y + 1][current.x - 1] = 11;
        status[current.y + 1][current.x + 0] = 11;
        status[current.y + 1][current.x + 1] = 11;
        break;
    case 3://역 ㄴ만들기
        status[current.y + 0][current.x + 1] = 11;//1 -1
        status[current.y + 1][current.x + 1] = 11;
        status[current.y + 1][current.x + 0] = 11;
        status[current.y + 1][current.x - 1] = 11;
        break;
    case 4:
    case 18:
        status[current.y + 0][current.x + 1] = 11;//1 -1
        status[current.y + 0][current.x + 0] = 11;
        status[current.y + 1][current.x + 0] = 11;
        status[current.y + 1][current.x - 1] = 11;
        break;
    case 5:// z
    case 19:
        status[current.y + 0][current.x + 0] = 11;//1 -1
        status[current.y + 0][current.x - 1] = 11;
        status[current.y + 1][current.x + 0] = 11;
        status[current.y + 1][current.x + 1] = 11;
        break;
    case 6://ㅗ만들기
        status[current.y + 0][current.x + 0] = 11;//1 -1
        status[current.y + 1][current.x + 0] = 11;
        status[current.y + 1][current.x + 1] = 11;
        status[current.y + 1][current.x - 1] = 11;
        break;
    case 8: //|
    case 22:
        status[current.y + 2][current.x + 0] = 11;// 0
        status[current.y + 1][current.x + 0] = 11;
        status[current.y + 0][current.x + 0] = 11;
        status[current.y - 1][current.x + 0] = 11;
        break;
    case 9: //ㄴ만들기
        status[current.y + 0][current.x + 1] = 11;//1 0
        status[current.y + 1][current.x + 1] = 11;
        status[current.y + 2][current.x + 1] = 11;
        status[current.y + 2][current.x + 0] = 11;
        break;
    case 10://역 ㄴ만들기
        status[current.y + 0][current.x + 1] = 11;//1 0
        status[current.y + 1][current.x + 1] = 11;
        status[current.y + 2][current.x + 1] = 11;
        status[current.y + 0][current.x + 0] = 11;
        break;
    case 11://세운 ㄹ만들기
    case 25:
        status[current.y + 0][current.x + 0] = 11;//1 0
        status[current.y + 1][current.x + 0] = 11;
        status[current.y + 1][current.x + 1] = 11;
        status[current.y + 2][current.x + 1] = 11;
        break;

    case 12://세운 역 s만들기
    case 26:
        status[current.y + 0][current.x + 1] = 11;//1 0
        status[current.y + 1][current.x + 1] = 11;
        status[current.y + 1][current.x + 0] = 11;
        status[current.y + 2][current.x + 0] = 11;
        break;
    case 13://ㅓ 만들기
        status[current.y + 0][current.x + 1] = 11;// 1 0
        status[current.y + 1][current.x + 0] = 11;
        status[current.y + 1][current.x + 1] = 11;
        status[current.y + 2][current.x + 1] = 11;
        break;
    case 16: //ㄴ만들기
        status[current.y + 0][current.x - 1] = 11;//1 -1
        status[current.y + 0][current.x + 0] = 11;
        status[current.y + 0][current.x + 1] = 11;
        status[current.y + 1][current.x + 1] = 11;
        break;
    case 17://역 ㄴ만들기
        status[current.y + 0][current.x + 1] = 11;//1 -1
        status[current.y + 0][current.x + 0] = 11;
        status[current.y + 0][current.x - 1] = 11;
        status[current.y + 1][current.x - 1] = 11;
        break;
    case 20://ㅜ만들기
        status[current.y + 1][current.x + 0] = 11;//1 -1
        status[current.y + 0][current.x + 0] = 11;
        status[current.y + 0][current.x + 1] = 11;
        status[current.y + 0][current.x - 1] = 11;
        break;

    case 23: //ㄴ만들기
        status[current.y + 0][current.x + 0] = 11;//1 0
        status[current.y + 0][current.x + 1] = 11;
        status[current.y + 1][current.x + 0] = 11;
        status[current.y + 2][current.x + 0] = 11;
        break;
    case 24://역 ㄴ만들기
        status[current.y + 0][current.x + 0] = 11;//1 0
        status[current.y + 1][current.x + 0] = 11;
        status[current.y + 2][current.x + 0] = 11;
        status[current.y + 2][current.x + 1] = 11;
        break;
    case 27://ㅏ만들기
        status[current.y + 0][current.x + 0] = 11;//1 0
        status[current.y + 1][current.x + 0] = 11;
        status[current.y + 1][current.x + 1] = 11;
        status[current.y + 2][current.x + 0] = 11;
        break;

    }
}

void erase_tetromino(void) {
    switch (current_shape) {
    case 0://네모 만들기
    case 7:
    case 14:
    case 21:
        status[current.y + 0][current.x + 1] = 0;
        status[current.y + 0][current.x + 0] = 0;
        status[current.y + 1][current.x + 1] = 0;
        status[current.y + 1][current.x + 0] = 0;
        break;
    case 1: //작대기 만들기
    case 15:
        status[current.y + 0][current.x + 2] = 0;//6
        status[current.y + 0][current.x + 1] = 0;//5
        status[current.y + 0][current.x + 0] = 0;//4
        status[current.y + 0][current.x - 1] = 0;//3
        break;
    case 2: //ㄴ만들기
        status[current.y + 0][current.x - 1] = 0;
        status[current.y + 1][current.x - 1] = 0;
        status[current.y + 1][current.x + 0] = 0;
        status[current.y + 1][current.x + 1] = 0;
        break;
    case 3://역 ㄴ만들기
        status[current.y + 0][current.x + 1] = 0;
        status[current.y + 1][current.x + 1] = 0;
        status[current.y + 1][current.x + 0] = 0;
        status[current.y + 1][current.x - 1] = 0;
        break;
    case 4://s
    case 18:
        status[current.y + 0][current.x + 1] = 0;
        status[current.y + 0][current.x + 0] = 0;
        status[current.y + 1][current.x + 0] = 0;
        status[current.y + 1][current.x - 1] = 0;
        break;
    case 5:// z
    case 19:
        status[current.y + 0][current.x + 0] = 0;
        status[current.y + 0][current.x - 1] = 0;
        status[current.y + 1][current.x + 0] = 0;
        status[current.y + 1][current.x + 1] = 0;
        break;
    case 6://ㅗ만들기
        status[current.y + 0][current.x + 0] = 0;
        status[current.y + 1][current.x + 0] = 0;
        status[current.y + 1][current.x + 1] = 0;
        status[current.y + 1][current.x - 1] = 0;
        break;
    case 8: //|
    case 22:
        status[current.y + 2][current.x + 0] = 0;//6
        status[current.y + 1][current.x + 0] = 0;//5
        status[current.y + 0][current.x + 0] = 0;//4
        status[current.y - 1][current.x + 0] = 0;//3
        break;
    case 9: //ㄴ만들기
        status[current.y + 0][current.x + 1] = 0;
        status[current.y + 1][current.x + 1] = 0;
        status[current.y + 2][current.x + 1] = 0;
        status[current.y + 2][current.x + 0] = 0;
        break;
    case 10://역 ㄴ만들기
        status[current.y + 0][current.x + 1] = 0;
        status[current.y + 1][current.x + 1] = 0;
        status[current.y + 2][current.x + 1] = 0;
        status[current.y + 0][current.x + 0] = 0;
        break;
    case 11://세운 ㄹ만들기
    case 25:
        status[current.y + 0][current.x + 0] = 0;
        status[current.y + 1][current.x + 0] = 0;
        status[current.y + 1][current.x + 1] = 0;
        status[current.y + 2][current.x + 1] = 0;
        break;
    case 12://세운 역 s만들기
    case 26:
        status[current.y + 0][current.x + 1] = 0;
        status[current.y + 1][current.x + 1] = 0;
        status[current.y + 1][current.x + 0] = 0;
        status[current.y + 2][current.x + 0] = 0;
        break;
    case 13://ㅓ 만들기
        status[current.y + 0][current.x + 1] = 0;
        status[current.y + 1][current.x + 0] = 0;
        status[current.y + 1][current.x + 1] = 0;
        status[current.y + 2][current.x + 1] = 0;
        break;

    case 16: //ㄴ만들기
        status[current.y + 0][current.x - 1] = 0;
        status[current.y + 0][current.x + 0] = 0;
        status[current.y + 0][current.x + 1] = 0;
        status[current.y + 1][current.x + 1] = 0;
        break;
    case 17://역 ㄴ만들기
        status[current.y + 0][current.x + 1] = 0;
        status[current.y + 0][current.x + 0] = 0;
        status[current.y + 0][current.x - 1] = 0;
        status[current.y + 1][current.x - 1] = 0;
        break;

    case 20://ㅜ만들기
        status[current.y + 1][current.x + 0] = 0;
        status[current.y + 0][current.x + 0] = 0;
        status[current.y + 0][current.x + 1] = 0;
        status[current.y + 0][current.x - 1] = 0;
        break;

    case 23: //ㄴ만들기
        status[current.y + 0][current.x + 0] = 0;
        status[current.y + 0][current.x + 1] = 0;
        status[current.y + 1][current.x + 0] = 0;
        status[current.y + 2][current.x + 0] = 0;
        break;
    case 24://역 ㄴ만들기
        status[current.y + 0][current.x + 0] = 0;
        status[current.y + 1][current.x + 0] = 0;
        status[current.y + 2][current.x + 0] = 0;
        status[current.y + 2][current.x + 1] = 0;
        break;
    case 27://ㅏ만들기
        status[current.y + 0][current.x + 0] = 0;
        status[current.y + 1][current.x + 0] = 0;
        status[current.y + 1][current.x + 1] = 0;
        status[current.y + 2][current.x + 0] = 0;
        break;

    }
}

int move_tetromino(int direction) { // 1왼쪽 2아래 3오른쪽
    int down = down_check();
    if (down == 1) {
        return 1;
    }
    erase_tetromino();

    //if ()//방향에 따라 current.x y변경 
    if (direction == 1) {
        current.x--;
    }
    if (direction == 2) {
        current.y++;
    }
    if (direction == 3) {
        current.x++;
    }

    make_tetromino();

    return 0;
}

void rotate(void) {
    erase_tetromino();
    if (current_shape <= 21) {
        current_shape += 7;
    }
    else {
        current_shape %= 7;
    }
    make_tetromino();
}

void show_map(void) { //맵표현 gotoxy
    int i, j;
    gotoxy(1, 1);
    printf("\n");
    for (i = 0; i < HEIGHT + 1; i++) { //메인창의 높이
        if (i != 20) {
            for (j = 0; j < 15; j++) { //메인창 왼쪽공간설정
                printf(" ");
            }
            printf("┃"); //메인창 테두리
            for (j = 0; j < WIDTH; j++) { //메인창 너비
                if (status[i][j] == 0) {
                    printf(". ");
                }
                else {
                    printf("□");//ㅁ한자에 있음
                }
            }
            printf("┃");// 테두리
            if (i == 4) {
                printf("         score:%d", score * 10);
            }
            else if (i == 6) {
                printf("    line clear:%d", deleted_line);
            }
            printf("\n");
        }
        else {
            for (j = 0; j < 15; j++) {
                printf(" ");
            }
            printf("┃");
            for (j = 0; j < WIDTH; j++) { //메인창 바닥
                printf("■");
            }
            printf("┃");
            printf("\n");
        }
    }

}

int down_check(void) {
    switch (current_shape) {
    case 0://네모 확인
    case 7:
    case 14:
    case 21:
        if (status[current.y + 2][current.x + 1] != 0 || status[current.y + 2][current.x + 0] != 0 || current.y + 2 == HEIGHT) {
            return 1;
        }
        break;
    case 1: //작대기 확인
    case 15:
        if (status[current.y + 1][current.x + 2] != 0 || status[current.y + 1][current.x + 1] != 0 || status[current.y + 1][current.x + 0] != 0 || status[current.y + 1][current.x - 1] != 0 || current.y + 1 == HEIGHT) {
            return 1;
        }
        break;
    case 2: //ㄴ확인
        if (status[current.y + 2][current.x + 0] != 0 || status[current.y + 2][current.x + 1] != 0 || status[current.y + 2][current.x - 1] != 0 || current.y + 2 == HEIGHT) {
            return 1;
        }
        break;
    case 3://역 ㄴ확인
        if (status[current.y + 2][current.x + 1] != 0 || status[current.y + 2][current.x + 0] != 0 || status[current.y + 2][current.x - 1] != 0 || current.y + 2 == HEIGHT) {
            return 1;
        }
        break;
    case 4://ㄹ확인
    case 18:
        if (status[current.y + 1][current.x + 1] != 0 || status[current.y + 2][current.x + 0] != 0 || status[current.y + 2][current.x - 1] != 0 || current.y + 2 == HEIGHT) {
            return 1;
        }
        break;
    case 5://역 ㄹ확인
    case 19:
        if (status[current.y + 2][current.x + 1] != 0 || status[current.y + 2][current.x + 0] != 0 || status[current.y + 1][current.x - 1] != 0 || current.y + 2 == HEIGHT) {
            return 1;
        }
        break;
    case 6://ㅗ확인
        if (status[current.y + 2][current.x + 0] != 0 || status[current.y + 2][current.x + 1] != 0 || status[current.y + 2][current.x - 1] != 0 || current.y + 2 == HEIGHT) {
            return 1;
        }
        break;
    case 8: //작대기 확인 
    case 22:
        if (status[current.y + 3][current.x + 0] != 0 || current.y + 3 == HEIGHT) {
            return 1;
        }
        break;
    case 9: //ㄴ확인
        if (status[current.y + 3][current.x + 1] != 0 || status[current.y + 3][current.x + 0] != 0 || current.y + 3 == HEIGHT) {
            return 1;
        }
        break;
    case 10://역 ㄴ확인
        if (status[current.y + 1][current.x + 0] != 0 || status[current.y + 3][current.x + 1] != 0 || current.y + 3 == HEIGHT) {
            return 1;
        }
        break;
    case 11://세운 ㄹ확인
    case 25:
        if (status[current.y + 2][current.x + 0] != 0 || status[current.y + 3][current.x + 1] != 0 || current.y + 3 == HEIGHT) {
            return 1;
        }
        break;
    case 12://세운 역 s확인
    case 26:
        if (status[current.y + 2][current.x + 1] != 0 || status[current.y + 3][current.x + 0] != 0 || current.y + 3 == HEIGHT) {
            return 1;
        }
        break;
    case 13://ㅓ 확인
        if (status[current.y + 2][current.x + 0] != 0 || status[current.y + 3][current.x + 1] != 0 || current.y + 3 == HEIGHT) {
            return 1;
        }
        break;
    case 16: //ㄴ확인
        if (status[current.y + 1][current.x - 1] != 0 || status[current.y + 1][current.x + 0] != 0 || status[current.y + 2][current.x + 1] != 0 || current.y + 2 == HEIGHT) {
            return 1;
        }
        break;
    case 17://역 ㄴ확인
        if (status[current.y + 1][current.x + 1] != 0 || status[current.y + 1][current.x + 0] != 0 || status[current.y + 2][current.x - 1] != 0 || current.y + 2 == HEIGHT) {
            return 1;
        }
        break;
    case 20://ㅜ확인
        if (status[current.y + 2][current.x + 0] != 0 || status[current.y + 1][current.x + 1] != 0 || status[current.y + 1][current.x - 1] != 0 || current.y + 2 == HEIGHT) {
            return 1;
        }
        break;

    case 23: //ㄴ확인
        if (status[current.y + 1][current.x + 1] != 0 || status[current.y + 3][current.x + 0] != 0 || current.y + 3 == HEIGHT) {
            return 1;
        }
        break;
    case 24://역 ㄴ확인
        if (status[current.y + 3][current.x + 0] != 0 || status[current.y + 3][current.x + 1] != 0 | current.y + 3 == HEIGHT) {
            return 1;
        }
        break;

    case 27://ㅏ확인
        if (status[current.y + 2][current.x + 1] != 0 || status[current.y + 3][current.x + 0] != 0 || current.y + 3 == HEIGHT) {
            return 1;
        }
        break;
    }
    return 0;
}

int side_check(int way) {
    switch (current_shape) {
    case 0://1 0
    case 7:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 21:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
        if (way == LEFT) {
            return current.x == 0;
        }
        else {
            return current.x == WIDTH - 2;
        }
        break;
    case 2:  //1 -1
    case 3:
    case 4:
    case 5:
    case 6:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
        if (way == LEFT) {
            return current.x == 1;
        }
        else {
            return current.x == WIDTH - 2;
        }
        break;
    case 1: //2 -1 
    case 15:
        if (way == LEFT) {
            return current.x == 1;
        }
        else {
            return current.x == WIDTH - 3;
        }
        break;
    case 8: //|
    case 22:
        if (way == LEFT) {
            return current.x == 0;
        }
        else {
            return current.x == WIDTH - 1;
        }
        break;
    }
}


void erasing_check(int* map[HEIGHT]) {//줄지우기 동작 
    
}

int main(void) {
    int tick = 0;
    srand(time(NULL));
    remove_cursor();
    init_status();
    show_map();
    score = 0;
    deleted_line = 0;
    init_tetromino();
    make_tetromino();
    show_map();
    //while 죽을때 까지
    while (1) {
        show_map();
        // 키보드 입력시 동작//
        while (_kbhit()) {
            int key = getch();
            if (key == LEFT) {
                if (!side_check(LEFT))
                    move_tetromino(1);
                break;
            }if (key == DOWN) {
                move_tetromino(2);

                break;
            }if (key == RIGHT) {
                if (!side_check(RIGHT))
                    move_tetromino(3);
                break;
            }if (key == UP) {
                rotate();
                break;
            }if (key == SPACE) {
                while (!down_check()) {
                    move_tetromino(2);
                }
            }
            fflush(stdin);
        }
        /////시간의 흐름/////
        if (tick == 5) {
            if (down_check()) {
                erasing_check(status);
                init_tetromino();
            }
            move_tetromino(2);
            show_map();
            tick = 0;
        }
        ////////////////////

        show_map();
        tick++;
        _sleep(1);
    }

    return 0;
}
