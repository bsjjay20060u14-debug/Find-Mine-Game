#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 9
#define MINES 10

char board[SIZE][SIZE];
char visible[SIZE][SIZE];

int firstMove = 1;
int openedCells = 0;
int flagsPlaced = 0;

void initializeGame() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = '0';
            visible[i][j] = '#';
        }
    }
}

int isValid(int r, int c) {
    return r >= 0 && r < SIZE && c >= 0 && c < SIZE;
}

void placeMines(int safeRow, int safeCol) {
    int count = 0;

    while (count < MINES) {
        int r = rand() % SIZE;
        int c = rand() % SIZE;

        if ((r == safeRow && c == safeCol) || board[r][c] == '*')
            continue;

        board[r][c] = '*';
        count++;
    }
}

void calculateNumbers() {
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {

            if (board[r][c] == '*')
                continue;

            int mines = 0;

            for (int dr = -1; dr <= 1; dr++) {
                for (int dc = -1; dc <= 1; dc++) {

                    int nr = r + dr;
                    int nc = c + dc;

                    if (isValid(nr, nc) &&
                        board[nr][nc] == '*')
                        mines++;
                }
            }

            board[r][c] = mines + '0';
        }
    }
}

void printBoard() {
    printf("\n");

    printf("깃발: %d/%d\n\n", flagsPlaced, MINES);

    printf("   ");
    for (int c = 0; c < SIZE; c++)
        printf("%d ", c);

    printf("\n");

    for (int r = 0; r < SIZE; r++) {
        printf("%d  ", r);

        for (int c = 0; c < SIZE; c++) {
            printf("%c ", visible[r][c]);
        }

        printf("\n");
    }

    printf("\n");
}

void reveal(int r, int c) {

    if (!isValid(r, c))
        return;

    if (visible[r][c] != '#')
        return;

    visible[r][c] = board[r][c];
    openedCells++;

    if (board[r][c] != '0')
        return;

    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {

            if (dr == 0 && dc == 0)
                continue;

            reveal(r + dr, c + dc);
        }
    }
}

void toggleFlag(int r, int c) {

    if (!isValid(r, c))
        return;

    if (visible[r][c] == '#') {
        visible[r][c] = 'F';
        flagsPlaced++;
    }
    else if (visible[r][c] == 'F') {
        visible[r][c] = '#';
        flagsPlaced--;
    }
}

void revealAll() {
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            visible[r][c] = board[r][c];
        }
    }
}

int checkWin() {
    return openedCells == (SIZE * SIZE - MINES);
}

void firstClickSetup(int r, int c) {
    placeMines(r, c);
    calculateNumbers();
    firstMove = 0;
}

void printInstructions() {

    printf("=================================\n");
    printf("        CONSOLE MINESWEEPER      \n");
    printf("=================================\n");
    printf("명령어:\n");
    printf("o row col  -> 셀 열기\n");
    printf("f row col  -> 깃발 설치/제거\n");
    printf("q          -> 종료\n\n");
}

int main() {

    srand((unsigned int)time(NULL));

    initializeGame();
    printInstructions();

    while (1) {

        printBoard();

        char command;
        int row, col;

        printf("Input: ");

        if (scanf(" %c", &command) != 1)
            continue;

        if (command == 'q') {
            printf("Game ended.\n");
            break;
        }

        if (command != 'o' && command != 'f') {
            printf("유효하지 않은 명령어입니다.\n");
            continue;
        }

        if (scanf("%d %d", &row, &col) != 2) {
            printf("유효하지 않은 입력입니다.\n");

            while (getchar() != '\n');
            continue;
        }

        if (!isValid(row, col)) {
            printf("범위를 벗어났습니다.\n");
            continue;
        }

        if (command == 'f') {

            if (visible[row][col] != '#' &&
                visible[row][col] != 'F') {
                printf("이미 열린 셀이거나 깃발이 아닌 셀입니다.\n");
                continue;
            }

            toggleFlag(row, col);
            continue;
        }

        if (visible[row][col] == 'F') {
            printf("깃발을 먼저 제거해주세요.\n");
            continue;
        }

        if (visible[row][col] != '#') {
            printf("이미 열린 셀입니다.\n");
            continue;
        }

        if (firstMove)
            firstClickSetup(row, col);

        if (board[row][col] == '*') {

            revealAll();
            printBoard();

            printf("펑! 지뢰를 밟았습니다.\n");
            printf("GAME OVER\n");

            break;
        }

        reveal(row, col);

        if (checkWin()) {

            revealAll();
            printBoard();

            printf("축하합니다!\n");
            printf("승리!\n");

            break;
        }
    }

    return 0;
}