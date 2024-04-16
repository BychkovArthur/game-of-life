#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

// Field settings
#define FIELD_WIDTH 80
#define FIELD_HEIGHT 25
#define VERTICAL_BORDER_WIDTH 1
#define HORIZONTAL_BORDER_WIDTH 1
#define TOTAL_WIDTH FIELD_WIDTH + 2 * VERTICAL_BORDER_WIDTH
#define TOTAL_HEIGHT FIELD_HEIGHT + 2 * HORIZONTAL_BORDER_WIDTH

// Entity's
#define BORDER_SYMBOL '#'
#define EMPTY_SYMBOL ' '
#define CREATURE '@'
#define POSSIBLE_NUMBER_OF_NEIGHBORS 8

// Game rules
#define NEIGHBORS_FOR_BIRTH 3
#define NEIGHBORS_FOR_LEAVE_1 2
#define NEIGHBORS_FOR_LEAVE_2 3

// Delay
#define INITIAL_DELAY 153
#define DELAY_STEP 3
#define MINIMAL_DELAY 3
#define MAXIMUM_DELAY 300

// Speed block
#define SPEED_HEIGHT 9
#define SYMBOL_WIDTH 6
#define SPACE_BETWEEN_SYMBOLS 2
#define SPEED_TENS_MOVE_RIGHT (TOTAL_WIDTH) / 2 - SPACE_BETWEEN_SYMBOLS / 2 - SYMBOL_WIDTH
#define SPEED_HUNDREDS_MOVE_RIGHT SPEED_TENS_MOVE_RIGHT - SPACE_BETWEEN_SYMBOLS - SYMBOL_WIDTH
#define SPEED_UNITS_MOVE_RIGHT SPEED_TENS_MOVE_RIGHT + SYMBOL_WIDTH + SPACE_BETWEEN_SYMBOLS
#define SPEED_PERCENT_SYMBOL_MOVE_RIGHT SPEED_UNITS_MOVE_RIGHT + SYMBOL_WIDTH + SPACE_BETWEEN_SYMBOLS
#define SPEED_MOVE_UP SPEED_HEIGHT - 2

// Field and drawing field
void initAndReadFields(char[FIELD_HEIGHT][FIELD_WIDTH], char[FIELD_HEIGHT][FIELD_WIDTH]);
void copyField(char[FIELD_HEIGHT][FIELD_WIDTH], char[FIELD_HEIGHT][FIELD_WIDTH]);
void readInitialPosition(char[FIELD_HEIGHT][FIELD_WIDTH]);
void initEmptyField(char[FIELD_HEIGHT][FIELD_WIDTH]);
void drawField(char[FIELD_HEIGHT][FIELD_WIDTH]);
void drawPartOfVerticalBorder();
void drawHorizontalBorder();
void clearScreen();

// Iterations and neighbor count
void updateCell(int, int, char[FIELD_HEIGHT][FIELD_WIDTH], char[FIELD_HEIGHT][FIELD_WIDTH]);
void updateFrame(int, char[FIELD_HEIGHT][FIELD_WIDTH], char[FIELD_HEIGHT][FIELD_WIDTH]);
void iterate(char[FIELD_HEIGHT][FIELD_WIDTH], char[FIELD_HEIGHT][FIELD_WIDTH]);
int getNumberOfNeighbors(int, int, char[FIELD_HEIGHT][FIELD_WIDTH]);
int isGameEnd(char[FIELD_HEIGHT][FIELD_WIDTH]);
int mod(int, int);

// Speed drawing
void setCursorUp(int);
void setCursorRight(int);
void setCursorUpRight(int, int);
void setCursorBack(int);
void printZero(int, int);
void printOne(int, int);
void printTwo(int, int);
void printThree(int, int);
void printFour(int, int);
void printFive(int, int);
void printSix(int, int);
void printSeven(int, int);
void printEight(int, int);
void printNine(int, int);
void drawDigit(int, int, int);
int getSpeedByDelay(int);
void drawSpaceForSpeed();
void drawSpeed(int);

// Ncurses initialization
void initializeNcurses();
void initColorPairs();



void setCursorUp(int up) {
    int y, x;
    getyx(stdscr, y, x);  // Получить текущие координаты курсора
    move(y - up, x);      // Переместить курсор вверх на up строк
}

void setCursorRight(int right) {
    int y, x;
    getyx(stdscr, y, x);  // Получить текущие координаты курсора
    move(y, x + right);   // Переместить курсор вправо на right символов
}

void setCursorUpRight(int moveUp, int moveRight) {
    setCursorUp(moveUp);
    setCursorRight(moveRight);
}

void setCursorBack(int down) {
    int y, x;
    getyx(stdscr, y, x);  // Получить текущие координаты курсора
    move(y + down, x);    // Переместить курсор вниз на down строк
}

void printZero(int moveUp, int moveRight) {
    setCursorUpRight(moveUp, moveRight);
    printw(" 0000 \n");
    setCursorRight(moveRight);
    for (int i = 0; i < 3; ++i) {
        printw("00  00\n");
        setCursorRight(moveRight);
    }
    printw(" 0000 \n");
    setCursorBack(moveUp - 5);
}

void printOne(int moveUp, int moveRight) {
    setCursorUpRight(moveUp, moveRight);
    printw("1111  \n");
    setCursorRight(moveRight);
    for (int i = 0; i < 3; ++i) {
        printw("  11  \n");
        setCursorRight(moveRight);
    }
    printw("111111\n");
    setCursorBack(moveUp - 5);
}

void printTwo(int moveUp, int moveRight) {
    setCursorUpRight(moveUp, moveRight);
    printw(" 2222 \n");
    setCursorRight(moveRight);
    printw("22  22\n");
    setCursorRight(moveRight);
    printw("   22 \n");
    setCursorRight(moveRight);
    printw("  22  \n");
    setCursorRight(moveRight);
    printw("222222\n");
    setCursorBack(moveUp - 5);
}

void printThree(int moveUp, int moveRight) {
    setCursorUpRight(moveUp, moveRight);
    printw(" 3333 \n");
    setCursorRight(moveRight);
    printw("33  33\n");
    setCursorRight(moveRight);
    printw("   333\n");
    setCursorRight(moveRight);
    printw("33  33\n");
    setCursorRight(moveRight);
    printw(" 3333 \n");
    setCursorBack(moveUp - 5);
}

void printFour(int moveUp, int moveRight) {
    setCursorUpRight(moveUp, moveRight);
    printw("44  44\n");
    setCursorRight(moveRight);
    printw("44  44\n");
    setCursorRight(moveRight);
    printw("444444\n");
    setCursorRight(moveRight);
    printw("    44\n");
    setCursorRight(moveRight);
    printw("    44\n");
    setCursorBack(moveUp - 5);
}

void printFive(int moveUp, int moveRight) {
    setCursorUpRight(moveUp, moveRight);
    printw("555555\n");
    setCursorRight(moveRight);
    printw("55    \n");
    setCursorRight(moveRight);
    printw("55555 \n");
    setCursorRight(moveRight);
    printw("    55\n");
    setCursorRight(moveRight);
    printw("55555 \n");
    setCursorBack(moveUp - 5);
}

void printSix(int moveUp, int moveRight) {
    setCursorUpRight(moveUp, moveRight);
    printw(" 6666 \n");
    setCursorRight(moveRight);
    printw("66    \n");
    setCursorRight(moveRight);
    printw("66666 \n");
    setCursorRight(moveRight);
    printw("66  66\n");
    setCursorRight(moveRight);
    printw(" 6666 \n");
    setCursorBack(moveUp - 5);
}

void printSeven(int moveUp, int moveRight) {
    setCursorUpRight(moveUp, moveRight);
    printw("777777\n");
    setCursorRight(moveRight);
    printw("   77 \n");
    setCursorRight(moveRight);
    printw("  77  \n");
    setCursorRight(moveRight);
    printw(" 77   \n");
    setCursorRight(moveRight);
    printw("77    \n");
    setCursorBack(moveUp - 5);
}

void printEight(int moveUp, int moveRight) {
    setCursorUpRight(moveUp, moveRight);
    printw(" 8888 \n");
    setCursorRight(moveRight);
    printw("88  88\n");
    setCursorRight(moveRight);
    printw(" 8888 \n");
    setCursorRight(moveRight);
    printw("88  88\n");
    setCursorRight(moveRight);
    printw(" 8888 \n");
    setCursorBack(moveUp - 5);
}

void printNine(int moveUp, int moveRight) {
    setCursorUpRight(moveUp, moveRight);
    printw(" 9999 \n");
    setCursorRight(moveRight);
    printw("99  99\n");
    setCursorRight(moveRight);
    printw(" 99999\n");
    setCursorRight(moveRight);
    printw("    99\n");
    setCursorRight(moveRight);
    printw(" 9999 \n");
    setCursorBack(moveUp - 5);
}

void printPercent(int moveUp, int moveRight) {
    setCursorUpRight(moveUp, moveRight);
    printw("%%%%  %%%%\n");
    setCursorRight(moveRight);
    printw("%%%% %%%% \n");
    setCursorRight(moveRight);
    printw("  %%%%  \n");
    setCursorRight(moveRight);
    printw(" %%%% %%%%\n");
    setCursorRight(moveRight);
    printw("%%%%  %%%%\n");
    setCursorBack(moveUp - 5);
}


void drawDigit(int moveUp, int moveRight, int digit) {
    switch (digit)
    {
        case 0:
            printZero(moveUp, moveRight);
            break;
        case 1:
            printOne(moveUp, moveRight);
            break;
        case 2:
            printTwo(moveUp, moveRight);
            break;
        case 3:
            printThree(moveUp, moveRight);
            break;
        case 4:
            printFour(moveUp, moveRight);
            break;
        case 5:
            printFive(moveUp, moveRight);
            break;
        case 6:
            printSix(moveUp, moveRight);
            break;
        case 7:
            printSeven(moveUp, moveRight);
            break;
        case 8:
            printEight(moveUp, moveRight);
            break;
        case 9:
            printNine(moveUp, moveRight);
            break;
    }
}




int mod(int a, int b) { return a >= 0 ? a % b : b + a % b; }



int isGameEnd(char field[FIELD_HEIGHT][FIELD_WIDTH]) {
    int isEnding = 1;
    for (int i = 0; i < FIELD_HEIGHT; ++i) {
        for (int j = 0; j < FIELD_WIDTH; ++j) {
            if (field[i][j] == CREATURE) {
                isEnding = 0;
                break;
            }
        }
        if (!isEnding) {
            break;
        }
    }
    return isEnding;
}

int getNumberOfNeighbors(int x, int y, char field[FIELD_HEIGHT][FIELD_WIDTH]) {
    const int moveX[POSSIBLE_NUMBER_OF_NEIGHBORS] = {1, 1, 1, 0, 0, -1, -1, -1};
    const int moveY[POSSIBLE_NUMBER_OF_NEIGHBORS] = {1, 0, -1, 1, -1, 1, 0, -1};
    int count = 0;

    for (int i = 0; i < POSSIBLE_NUMBER_OF_NEIGHBORS; ++i) {
        int newX = mod(x + moveX[i], FIELD_WIDTH);
        int newY = mod(y + moveY[i], FIELD_HEIGHT);
        if (field[newY][newX] == CREATURE) {
            ++count;
        }
    }

    return count;
}

void updateCell(int x, int y, char field[FIELD_HEIGHT][FIELD_WIDTH],
                char bufferField[FIELD_HEIGHT][FIELD_WIDTH]) {
    int neighborsCount = getNumberOfNeighbors(x, y, field);
    if (field[y][x] == CREATURE &&
        !(neighborsCount == NEIGHBORS_FOR_LEAVE_1 || neighborsCount == NEIGHBORS_FOR_LEAVE_2)) {
        bufferField[y][x] = EMPTY_SYMBOL;
    } else if (field[y][x] == EMPTY_SYMBOL && neighborsCount == NEIGHBORS_FOR_BIRTH) {
        bufferField[y][x] = CREATURE;
    }
}

void iterate(char field[FIELD_HEIGHT][FIELD_WIDTH], char bufferField[FIELD_HEIGHT][FIELD_WIDTH]) {
    for (int y = 0; y < FIELD_HEIGHT; ++y) {
        for (int x = 0; x < FIELD_WIDTH; ++x) {
            updateCell(x, y, field, bufferField);
        }
    }
}

void readInitialPosition(char matrix[FIELD_HEIGHT][FIELD_WIDTH]) {
    for (int i = 0; i < FIELD_HEIGHT; ++i) {
        for (int j = 0; j < FIELD_WIDTH; ++j) {
            int value;
            scanf("%d", &value);
            matrix[i][j] = value ? CREATURE : EMPTY_SYMBOL;
        }
    }
}

void initEmptyField(char field[FIELD_HEIGHT][FIELD_WIDTH]) {
    for (int y = 0; y < FIELD_HEIGHT; ++y) {
        for (int x = 0; x < FIELD_WIDTH; ++x) {
            field[y][x] = EMPTY_SYMBOL;
        }
    }
}

void drawField(char field[FIELD_HEIGHT][FIELD_WIDTH]) {
    attron(COLOR_PAIR(3));
    drawHorizontalBorder();
    attroff(COLOR_PAIR(3));
    for (int y = 0; y < FIELD_HEIGHT; ++y) {
        attron(COLOR_PAIR(3));
        drawPartOfVerticalBorder();
        attroff(COLOR_PAIR(3));
        for (int x = 0; x < FIELD_WIDTH; ++x) {
            attron(COLOR_PAIR(2));
            printw("%c", field[y][x]);
            attroff(COLOR_PAIR(2));
        }
        attron(COLOR_PAIR(3));
        drawPartOfVerticalBorder();
        attroff(COLOR_PAIR(3));
        printw("\n");
    }
    attron(COLOR_PAIR(3));
    drawHorizontalBorder();
    attroff(COLOR_PAIR(3));
    refresh();
}

void drawHorizontalBorder() {
    for (int i = 0; i < TOTAL_WIDTH; ++i) {
        printw("%c", BORDER_SYMBOL);
    }
    printw("\n");
}

void drawPartOfVerticalBorder() {
    for (int i = 0; i < HORIZONTAL_BORDER_WIDTH; ++i) {
        printw("%c", BORDER_SYMBOL);
    }
}

void clearScreen() {
    clear();
    refresh();
}

void copyField(char field[FIELD_HEIGHT][FIELD_WIDTH], char bufferField[FIELD_HEIGHT][FIELD_WIDTH]) {
    for (int y = 0; y < FIELD_HEIGHT; ++y) {
        for (int x = 0; x < FIELD_WIDTH; ++x) {
            bufferField[y][x] = field[y][x];
        }
    }
}

int getSpeedByDelay(int delay) {
    return 101 - (delay / DELAY_STEP);
}

void drawSpeed(int delay) {

    drawSpaceForSpeed();
    int speed = getSpeedByDelay(delay);
    int speedHundreds = speed / 100;
    int speedTens = (speed / 10) % 10;
    int speedUnits = speed % 10;

    
    attron(COLOR_PAIR(1));
    drawDigit(SPEED_MOVE_UP, SPEED_HUNDREDS_MOVE_RIGHT, speedHundreds);
    refresh();
    drawDigit(SPEED_MOVE_UP, SPEED_TENS_MOVE_RIGHT, speedTens);
    refresh();
    drawDigit(SPEED_MOVE_UP, SPEED_UNITS_MOVE_RIGHT, speedUnits);
    refresh();
    printPercent(SPEED_MOVE_UP, SPEED_PERCENT_SYMBOL_MOVE_RIGHT);
    attroff(COLOR_PAIR(1));
    refresh();
    
}

void drawSpaceForSpeed() {
    for (int i = 0; i < SPEED_HEIGHT; ++i) {
        printw("\n");
    }
}

void initializeNcurses() {
    initscr();              // Инициализация ncurses
    noecho();               // Отключение вывода нажатых клавиш
    cbreak();               // Включение режима посимвольного ввода
    curs_set(0);            // Скрытие курсора
    keypad(stdscr, TRUE);   // Включение обработки функциональных клавиш
    nodelay(stdscr, TRUE);  // Не блокировать ввод
    start_color();
    initColorPairs();
}

void initColorPairs() {
    init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    init_pair(3, COLOR_BLACK, COLOR_RED);
}

void initAndReadFields(char field[FIELD_HEIGHT][FIELD_WIDTH], char bufferField[FIELD_HEIGHT][FIELD_WIDTH]) {
    initEmptyField(field);
    initEmptyField(bufferField);
    readInitialPosition(field);
    copyField(field, bufferField);

    if (freopen("/dev/tty", "r", stdin) != NULL) {
        TRUE;
    }
}

void updateFrame(int delay, char field[FIELD_HEIGHT][FIELD_WIDTH], char bufferField[FIELD_HEIGHT][FIELD_WIDTH]) {
    clearScreen();
    drawSpeed(delay);
    drawField(field);
    iterate(bufferField, field);
    copyField(field, bufferField);
}

int main() {
    initializeNcurses();

    char field[FIELD_HEIGHT][FIELD_WIDTH];
    char bufferField[FIELD_HEIGHT][FIELD_WIDTH];

    initAndReadFields(field, bufferField);

    int delay = INITIAL_DELAY;
    while (1) {
        char userInput;
        userInput = getch();
        flushinp(); // очистка буффера
        if ((userInput == 'w' || userInput == 'W') && delay > MINIMAL_DELAY) {
            delay -= DELAY_STEP;
        } else if ((userInput == 's' || userInput == 'S') && delay < MAXIMUM_DELAY) {
            delay += DELAY_STEP;
        }

        updateFrame(delay, field, bufferField);
        
        if (userInput == 'q' || userInput == 'Q' || isGameEnd(field)) {
            clearScreen();
            break;
        }

        napms(delay);
    }

    return 0;
}