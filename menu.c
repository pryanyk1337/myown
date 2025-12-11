#include "game.h"
#include <stdlib.h>

int showMenu() {
    int selected = 0;
    const char *items[3] = {
        "Start Game",
        "Instructions",
        "Exit Game"
    };
    int choice = -1;
    nodelay(stdscr, TRUE);

    while (choice == -1) {
        clear();

        // Бокс для меню
        drawBox(1, 5, 30, 55, COLOR_UI);

        // Пользовательский баннер "Cowboys of Space"
        attron(COLOR_PAIR(COLOR_BULLET_PLAYER) | A_BOLD);
        mvprintw(2, 7, "▗▄▄▖ ▗▄▖ ▗▖ ▗▖▗▄▄▖  ▗▄▖▗▖  ▗▖▗▄▄▖     ▗▄▖ ▗▄▄▄▖");
        mvprintw(3, 7, "▐▌   ▐▌ ▐▌▐▌ ▐▌▐▌ ▐▌▐▌ ▐▌▝▚▞▘▐▌       ▐▌ ▐▌▐▌   ");
        mvprintw(4, 7, "▐▌   ▐▌ ▐▌▐▌ ▐▌▐▛▀▚▖▐▌ ▐▌ ▐▌  ▝▀▚▖    ▐▌ ▐▌▐▛▀▀▘");
        mvprintw(5, 7, "▝▚▄▄▖▝▚▄▞▘▐▙█▟▌▐▙▄▞▘▝▚▄▞▘ ▐▌ ▗▄▄▞▘    ▝▚▄▞▘▐▌   ");
        mvprintw(8, 7, " ▗▄▄▖▗▄▄▖  ▗▄▖  ▗▄▄▖▗▄▄▄▖                       ");
        mvprintw(9, 7, "▐▌   ▐▌ ▐▌▐▌ ▐▌▐▌   ▐▌                          ");
        mvprintw(10, 7, " ▝▀▚▖▐▛▀▘ ▐▛▀▜▌▐▌   ▐▛▀▀▘                       ");
        mvprintw(11, 7, "▗▄▄▞▘▐▌   ▐▌ ▐▌▝▚▄▄▖▐▙▄▄▖                       ");
        attroff(COLOR_PAIR(COLOR_BULLET_PLAYER) | A_BOLD);

        // Меню опций с подсветкой
        attron(COLOR_PAIR(COLOR_UI));
        mvprintw(20, 18, "===========================");
        attroff(COLOR_PAIR(COLOR_UI));

        for (int i = 0; i < 3; i++) {
            if (i == selected) {
                attron(COLOR_PAIR(COLOR_TEXT) | A_REVERSE | A_BOLD);
            } else {
                attron(COLOR_PAIR(COLOR_TEXT));
            }
            mvprintw(21 + i, 24, "[%d] %s", i + 1, items[i]);
            if (i == selected) {
                attroff(COLOR_PAIR(COLOR_TEXT) | A_REVERSE | A_BOLD);
            } else {
                attroff(COLOR_PAIR(COLOR_TEXT));
            }
        }

        attron(COLOR_PAIR(COLOR_UI));
        mvprintw(24, 18, "===========================");
        attroff(COLOR_PAIR(COLOR_UI));

        attron(COLOR_PAIR(COLOR_BULLET_PLAYER));
        mvprintw(26, 10, "Use ↑/↓ or W/S, Enter to select. ESC to quit");
        attroff(COLOR_PAIR(COLOR_BULLET_PLAYER));

        refresh();
        int ch = getch();
        switch (ch) {
            case KEY_UP:
            case 'w':
            case 'W':
                selected = (selected + 2) % 3;
                break;
            case KEY_DOWN:
            case 's':
            case 'S':
                selected = (selected + 1) % 3;
                break;
            case '1':
            case '2':
            case '3':
                choice = ch - '0';
                break;
            case '\n':
            case '\r':
                choice = selected + 1;
                break;
            case 27: // ESC
                choice = 0;
                break;
            default:
                break;
        }

        napms(16); // небольшой таймслот для плавности
    }
    return choice;
}

void showInstructions() {
    clear();

    drawBox(1, 5, 28, 70, COLOR_UI);

    attron(COLOR_PAIR(COLOR_BULLET_PLAYER) | A_BOLD);
    mvprintw(2, 28, "+-------------------+");
    mvprintw(3, 28, "|   INSTRUCTIONS    |");
    mvprintw(4, 28, "+-------------------+");
    attroff(COLOR_PAIR(COLOR_BULLET_PLAYER) | A_BOLD);

    attron(COLOR_PAIR(COLOR_TEXT) | A_BOLD);
    mvprintw(6, 8, "OBJECTIVE:");
    attroff(COLOR_PAIR(COLOR_TEXT) | A_BOLD);
    attron(COLOR_PAIR(COLOR_TEXT));
    mvprintw(7, 10, "Destroy all alien invaders before time runs out!");
    mvprintw(8, 10, "Protect Earth from the invasion across 5 challenging levels!");
    attroff(COLOR_PAIR(COLOR_TEXT));
    
    attron(COLOR_PAIR(COLOR_TEXT) | A_BOLD);
    mvprintw(10, 8, "CONTROLS:");
    attroff(COLOR_PAIR(COLOR_TEXT) | A_BOLD);
    attron(COLOR_PAIR(COLOR_TEXT));
    mvprintw(11, 10, "[A] or [<-] - Move Ship Left");
    mvprintw(12, 10, "[D] or [->] - Move Ship Right");
    mvprintw(13, 10, "[SPACE]     - Fire Weapon");
    mvprintw(14, 10, "[ESC]       - Pause Game");
    mvprintw(15, 10, "[Q]         - Quit to Main Menu");
    attroff(COLOR_PAIR(COLOR_TEXT));
    
    attron(COLOR_PAIR(COLOR_TEXT) | A_BOLD);
    mvprintw(17, 8, "ENEMY TYPES:");
    attroff(COLOR_PAIR(COLOR_TEXT) | A_BOLD);
    
    attron(COLOR_PAIR(COLOR_ENEMY) | A_BOLD);
    mvprintw(18, 10, "W");
    attroff(COLOR_PAIR(COLOR_ENEMY) | A_BOLD);
    attron(COLOR_PAIR(COLOR_TEXT));
    printw(" - Normal Alien    (10 pts, 1 hit)");
    attroff(COLOR_PAIR(COLOR_TEXT));
    
    attron(COLOR_PAIR(COLOR_ENEMY_FAST) | A_BOLD);
    mvprintw(19, 10, "M");
    attroff(COLOR_PAIR(COLOR_ENEMY_FAST) | A_BOLD);
    attron(COLOR_PAIR(COLOR_TEXT));
    printw(" - Fast Alien      (20 pts, 1 hit, moves faster)");
    attroff(COLOR_PAIR(COLOR_TEXT));
    
    attron(COLOR_PAIR(COLOR_ENEMY_STRONG) | A_BOLD);
    mvprintw(20, 10, "X");
    attroff(COLOR_PAIR(COLOR_ENEMY_STRONG) | A_BOLD);
    attron(COLOR_PAIR(COLOR_TEXT));
    printw(" - Strong Alien    (30 pts, 2 hits, armored)");
    attroff(COLOR_PAIR(COLOR_TEXT));
    
    attron(COLOR_PAIR(COLOR_TEXT) | A_BOLD);
    mvprintw(22, 8, "GAMEPLAY:");
    attroff(COLOR_PAIR(COLOR_TEXT) | A_BOLD);
    attron(COLOR_PAIR(COLOR_TEXT));
    mvprintw(23, 10, "- Complete all 5 levels with increasing difficulty");
    mvprintw(24, 10, "- Each level has more enemies and faster gameplay");
    mvprintw(25, 10, "- Score multiplier increases with each level!");
    mvprintw(26, 10, "- Don't let enemies reach the bottom or hit you!");
    attroff(COLOR_PAIR(COLOR_TEXT));
    
    attron(COLOR_PAIR(COLOR_BULLET_PLAYER) | A_BOLD);
    mvprintw(28, 22, "Press any key to return to menu...");
    attroff(COLOR_PAIR(COLOR_BULLET_PLAYER) | A_BOLD);
    
    refresh();
    nodelay(stdscr, FALSE);
    getch();
    nodelay(stdscr, TRUE);
}

void showGameOver(int won) {
    clear();
    
    drawBox(8, 15, 12, 50, COLOR_UI);
    
    if (won) {
        attron(COLOR_PAIR(COLOR_BULLET_PLAYER) | A_BOLD);
        mvprintw(10, 28, "+-------------------+");
        mvprintw(11, 28, "|   LEVEL CLEAR!    |");
        mvprintw(12, 28, "+-------------------+");
        attroff(COLOR_PAIR(COLOR_BULLET_PLAYER) | A_BOLD);
        
        attron(COLOR_PAIR(COLOR_TEXT) | A_BOLD);
        mvprintw(14, 25, "Level %d Completed!", gameState.level);
        mvprintw(15, 25, "Score: %d", player.score);
        attroff(COLOR_PAIR(COLOR_TEXT) | A_BOLD);
    } else {
        attron(COLOR_PAIR(COLOR_ENEMY) | A_BOLD);
        mvprintw(10, 28, "+-------------------+");
        mvprintw(11, 28, "|    GAME OVER!     |");
        mvprintw(12, 28, "+-------------------+");
        attroff(COLOR_PAIR(COLOR_ENEMY) | A_BOLD);
        
        attron(COLOR_PAIR(COLOR_TEXT));
        mvprintw(14, 25, "Earth has been invaded!");
        mvprintw(15, 25, "Final Score: %d", player.score);
        mvprintw(16, 25, "Level Reached: %d", gameState.level);
        attroff(COLOR_PAIR(COLOR_TEXT));
    }
    
    attron(COLOR_PAIR(COLOR_UI));
    mvprintw(18, 24, "Press any key to continue...");
    attroff(COLOR_PAIR(COLOR_UI));
    
    refresh();
    nodelay(stdscr, FALSE);
    getch();
    nodelay(stdscr, TRUE);
}

void pauseGame() {
    clear();
    
    drawBox(8, 20, 10, 40, COLOR_UI);
    
    attron(COLOR_PAIR(COLOR_BULLET_PLAYER) | A_BOLD);
    mvprintw(10, 32, "+---------------+");
    mvprintw(11, 32, "|  GAME PAUSED  |");
    mvprintw(12, 32, "+---------------+");
    attroff(COLOR_PAIR(COLOR_BULLET_PLAYER) | A_BOLD);
    
    attron(COLOR_PAIR(COLOR_TEXT));
    mvprintw(14, 28, "Take a break, Commander!");
    mvprintw(16, 24, "Press any key to resume...");
    attroff(COLOR_PAIR(COLOR_TEXT));
    
    refresh();
    
    // Сохраняем время паузы
    time_t pauseStart = time(NULL);
    nodelay(stdscr, FALSE);
    getch();
    nodelay(stdscr, TRUE);
    time_t pauseEnd = time(NULL);
    
    // Корректируем время начала уровня
    gameState.startTime += (pauseEnd - pauseStart);
}