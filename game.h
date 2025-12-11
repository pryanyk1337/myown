#ifndef GAME_H
#define GAME_H

#include <ncurses.h>
#include <time.h>

// Константы игры
#define WIDTH 60
#define HEIGHT 30
#define MAX_ENEMIES 40
#define MAX_BULLETS 25
#define MAX_ENEMY_BULLETS 20

// Цветовые пары
#define COLOR_PLAYER 1
#define COLOR_ENEMY 2
#define COLOR_BULLET_PLAYER 3
#define COLOR_BULLET_ENEMY 4
#define COLOR_UI 5
#define COLOR_TEXT 6
#define COLOR_ENEMY_FAST 7
#define COLOR_ENEMY_STRONG 8

// Структуры данных
typedef struct {
    int x, y;
    int active;
} Bullet;

typedef struct {
    int x, y;
    int active;
    int type; // 0 - обычный, 1 - быстрый, 2 - сильный
    int health;
} Enemy;

typedef struct {
    int x, y;
    int lives;
    int score;
} Player;

typedef struct {
    int level;
    int enemySpeed;
    int enemyCount;
    int enemyShootChance;
    int enemyShootDelay;
    int enemyShootCounter;
    time_t startTime;
    int timeLimit;
} GameState;

// Глобальные переменные
extern Player player;
extern Enemy enemies[MAX_ENEMIES];
extern Bullet bullets[MAX_BULLETS];
extern Bullet enemyBullets[MAX_ENEMY_BULLETS];
extern GameState gameState;
extern char screen[HEIGHT][WIDTH];

// Функции из game_logic.c
void initGame();
void initLevel();
void shoot();
void enemyShoot(int enemyIndex);
void updateBullets();
void updateEnemies();
void checkCollisions();
int checkWin();
int checkLose();
void gameLoop();

// Функции из render.c
void initNcurses();
void closeNcurses();
void clearScreen();
void initScreen();
void drawPlayer();
void drawEnemies();
void drawBullets();
void renderScreen();
void drawBorder(int y, int x, int height, int width);
void drawBox(int y, int x, int height, int width, int colorPair);

// Функции из menu.c
int showMenu();
void showInstructions();
void showGameOver(int won);
void pauseGame();

#endif
