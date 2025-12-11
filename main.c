#include "game.h"
#include <stdlib.h>

int main() {
    srand(time(NULL));
    
    initNcurses();
    
    int running = 1;
    
    while (running) {
        int choice = showMenu();
        
        switch (choice) {
            case 1:
                initGame();
                initLevel();
                gameLoop();
                break;
            case 2:
                showInstructions();
                break;
            case 3:
            case 0:
                running = 0;
                break;
            default:
                attron(COLOR_PAIR(COLOR_ENEMY));
                mvprintw(25, 28, "Invalid option! Try again.");
                attroff(COLOR_PAIR(COLOR_ENEMY));
                refresh();
                napms(1000);
        }
    }
    
    clear();
    drawBox(10, 20, 8, 40, COLOR_UI);
    attron(COLOR_PAIR(COLOR_BULLET_PLAYER) | A_BOLD);
    mvprintw(12, 25, "Thanks for playing!");
    mvprintw(15, 25, "See you, Space Cowboy...");
    attroff(COLOR_PAIR(COLOR_BULLET_PLAYER) | A_BOLD);
    refresh();
    napms(2000);
    
    closeNcurses();
    
    return 0;
}
