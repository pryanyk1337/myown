#include "game.h"
#include <string.h>
#include <locale.h>

// Инициализация ncurses
void initNcurses() {
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);
    start_color();
    
    // Инициализация расширенных цветовых пар
    init_pair(COLOR_PLAYER, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_ENEMY, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_BULLET_PLAYER, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_BULLET_ENEMY, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(COLOR_UI, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLOR_TEXT, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_ENEMY_FAST, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_ENEMY_STRONG, COLOR_MAGENTA, COLOR_BLACK);
}

void closeNcurses() {
    endwin();
}

void clearScreen() {
    clear();
}

void initScreen() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (y == 0 || y == HEIGHT - 1) {
                screen[y][x] = '=';
            } else if (x == 0 || x == WIDTH - 1) {
                screen[y][x] = '|';
            } else {
                screen[y][x] = ' ';
            }
        }
    }
}

void drawPlayer() {
    if (player.y >= 1 && player.y < HEIGHT - 1) {
        if (player.x >= 1 && player.x < WIDTH - 1) screen[player.y][player.x] = 'A';
        if (player.x - 1 >= 1) screen[player.y][player.x - 1] = '<';
        if (player.x + 1 < WIDTH - 1) screen[player.y][player.x + 1] = '>';
    }
}

void drawEnemies() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active && enemies[i].x >= 1 && enemies[i].x < WIDTH - 1 
            && enemies[i].y >= 1 && enemies[i].y < HEIGHT - 1) {
            switch (enemies[i].type) {
                case 0: screen[enemies[i].y][enemies[i].x] = 'W'; break;
                case 1: screen[enemies[i].y][enemies[i].x] = 'M'; break;
                case 2: screen[enemies[i].y][enemies[i].x] = 'X'; break;
            }
        }
    }
}

void drawBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active && bullets[i].x >= 1 && bullets[i].x < WIDTH - 1 
            && bullets[i].y >= 1 && bullets[i].y < HEIGHT - 1) {
            screen[bullets[i].y][bullets[i].x] = '|';
        }
    }
    
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (enemyBullets[i].active && enemyBullets[i].x >= 1 && enemyBullets[i].x < WIDTH - 1 
            && enemyBullets[i].y >= 1 && enemyBullets[i].y < HEIGHT - 1) {
            screen[enemyBullets[i].y][enemyBullets[i].x] = '*';
        }
    }
}

void drawBox(int y, int x, int height, int width, int colorPair) {
    attron(COLOR_PAIR(colorPair) | A_BOLD);
    
    // Углы и границы
    mvaddch(y, x, ACS_ULCORNER);
    mvaddch(y, x + width - 1, ACS_URCORNER);
    mvaddch(y + height - 1, x, ACS_LLCORNER);
    mvaddch(y + height - 1, x + width - 1, ACS_LRCORNER);
    
    // Горизонтальные линии
    for (int i = 1; i < width - 1; i++) {
        mvaddch(y, x + i, ACS_HLINE);
        mvaddch(y + height - 1, x + i, ACS_HLINE);
    }
    
    // Вертикальные линии
    for (int i = 1; i < height - 1; i++) {
        mvaddch(y + i, x, ACS_VLINE);
        mvaddch(y + i, x + width - 1, ACS_VLINE);
    }
    
    attroff(COLOR_PAIR(colorPair) | A_BOLD);
}

void renderScreen() {
    clear();
    
    // Красивая рамка вокруг всего экрана
    attron(COLOR_PAIR(COLOR_UI) | A_BOLD);
    for (int i = 0; i < WIDTH + 6; i++) {
        mvaddch(0, i, ACS_HLINE);
        mvaddch(HEIGHT + 7, i, ACS_HLINE);
    }
    for (int i = 1; i < HEIGHT + 7; i++) {
        mvaddch(i, 0, ACS_VLINE);
        mvaddch(i, WIDTH + 5, ACS_VLINE);
    }
    mvaddch(0, 0, ACS_ULCORNER);
    mvaddch(0, WIDTH + 5, ACS_URCORNER);
    mvaddch(HEIGHT + 7, 0, ACS_LLCORNER);
    mvaddch(HEIGHT + 7, WIDTH + 5, ACS_LRCORNER);
    attroff(COLOR_PAIR(COLOR_UI) | A_BOLD);
    
    // Заголовок
    attron(COLOR_PAIR(COLOR_UI) | A_BOLD);
    mvprintw(1, 3, "+");
    for (int i = 0; i < WIDTH; i++) mvaddch(1, 4 + i, '=');
    mvprintw(1, WIDTH + 4, "+");
    attroff(COLOR_PAIR(COLOR_UI) | A_BOLD);
    
    attron(COLOR_PAIR(COLOR_BULLET_PLAYER) | A_BOLD);
    mvprintw(2, (WIDTH - 20) / 2 + 3, "* COWBOYS OF SPACE *");
    attroff(COLOR_PAIR(COLOR_BULLET_PLAYER) | A_BOLD);
    
    attron(COLOR_PAIR(COLOR_UI));
    mvprintw(2, WIDTH - 10, "Level: %d", gameState.level);
    attroff(COLOR_PAIR(COLOR_UI));
    
    attron(COLOR_PAIR(COLOR_UI) | A_BOLD);
    mvprintw(3, 3, "+");
    for (int i = 0; i < WIDTH; i++) mvaddch(3, 4 + i, '=');
    mvprintw(3, WIDTH + 4, "+");
    attroff(COLOR_PAIR(COLOR_UI) | A_BOLD);
    
    // Информационная панель
    int timeLeft = gameState.timeLimit - (int)(time(NULL) - gameState.startTime);
    if (timeLeft < 0) timeLeft = 0;
    
    attron(COLOR_PAIR(COLOR_TEXT));
    mvprintw(4, 3, "Lives: ");
    attroff(COLOR_PAIR(COLOR_TEXT));
    
    attron(COLOR_PAIR(COLOR_ENEMY) | A_BOLD);
    for (int i = 0; i < player.lives; i++) {
        printw("♥ ");
    }
    attroff(COLOR_PAIR(COLOR_ENEMY) | A_BOLD);
    
    attron(COLOR_PAIR(COLOR_TEXT));
    mvprintw(4, 25, "Score: ");
    attroff(COLOR_PAIR(COLOR_TEXT));
    
    attron(COLOR_PAIR(COLOR_BULLET_PLAYER) | A_BOLD);
    printw("%d", player.score);
    attroff(COLOR_PAIR(COLOR_BULLET_PLAYER) | A_BOLD);
    
    attron(COLOR_PAIR(COLOR_TEXT));
    mvprintw(4, 45, "Time: ");
    attroff(COLOR_PAIR(COLOR_TEXT));
    
    if (timeLeft < 30) {
        attron(COLOR_PAIR(COLOR_ENEMY) | A_BOLD | A_BLINK);
    } else {
        attron(COLOR_PAIR(COLOR_UI) | A_BOLD);
    }
    printw("%02d:%02d", timeLeft / 60, timeLeft % 60);
    if (timeLeft < 30) {
        attroff(COLOR_PAIR(COLOR_ENEMY) | A_BOLD | A_BLINK);
    } else {
        attroff(COLOR_PAIR(COLOR_UI) | A_BOLD);
    }
    
    // Отрисовка игрового поля
    for (int y = 0; y < HEIGHT; y++) {
        move(y + 5, 3);
        for (int x = 0; x < WIDTH; x++) {
            char c = screen[y][x];
            
            if (c == '<' || c == 'A' || c == '>') {
                attron(COLOR_PAIR(COLOR_PLAYER) | A_BOLD);
                addch(c);
                attroff(COLOR_PAIR(COLOR_PLAYER) | A_BOLD);
            } else if (c == 'W') {
                attron(COLOR_PAIR(COLOR_ENEMY) | A_BOLD);
                addch(c);
                attroff(COLOR_PAIR(COLOR_ENEMY) | A_BOLD);
            } else if (c == 'M') {
                attron(COLOR_PAIR(COLOR_ENEMY_FAST) | A_BOLD);
                addch(c);
                attroff(COLOR_PAIR(COLOR_ENEMY_FAST) | A_BOLD);
            } else if (c == 'X') {
                attron(COLOR_PAIR(COLOR_ENEMY_STRONG) | A_BOLD);
                addch(c);
                attroff(COLOR_PAIR(COLOR_ENEMY_STRONG) | A_BOLD);
            } else if (c == '|') {
                attron(COLOR_PAIR(COLOR_BULLET_PLAYER) | A_BOLD);
                addch(c);
                attroff(COLOR_PAIR(COLOR_BULLET_PLAYER) | A_BOLD);
            } else if (c == '*') {
                attron(COLOR_PAIR(COLOR_BULLET_ENEMY) | A_BOLD);
                addch(c);
                attroff(COLOR_PAIR(COLOR_BULLET_ENEMY) | A_BOLD);
            } else if (c == '|' || c == '=') {
                attron(COLOR_PAIR(COLOR_UI));
                addch(c);
                attroff(COLOR_PAIR(COLOR_UI));
            } else {
                addch(c);
            }
        }
    }
    
    // Нижняя панель управления
    attron(COLOR_PAIR(COLOR_UI) | A_BOLD);
    mvprintw(HEIGHT + 5, 3, "+");
    for (int i = 0; i < WIDTH; i++) mvaddch(HEIGHT + 5, 4 + i, '=');
    mvprintw(HEIGHT + 5, WIDTH + 4, "+");
    attroff(COLOR_PAIR(COLOR_UI) | A_BOLD);
    
    attron(COLOR_PAIR(COLOR_TEXT));
    mvprintw(HEIGHT + 6, 5, "[A/D/←/→]Move");
    mvprintw(HEIGHT + 6, 25, "[SPACE]Shoot");
    mvprintw(HEIGHT + 6, 45, "[ESC]Pause ");
    mvprintw(HEIGHT + 6, WIDTH - 5, "[Q]Quit");
    attroff(COLOR_PAIR(COLOR_TEXT));
    
    attron(COLOR_PAIR(COLOR_UI) | A_BOLD);
    mvprintw(HEIGHT + 7, 3, "+");
    for (int i = 0; i < WIDTH; i++) mvaddch(HEIGHT + 7, 4 + i, '=');
    mvprintw(HEIGHT + 7, WIDTH + 4, "+");
    attroff(COLOR_PAIR(COLOR_UI) | A_BOLD);
    
    refresh();
}
