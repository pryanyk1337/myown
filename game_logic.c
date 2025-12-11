#include "game.h"
#include <stdlib.h>

// Глобальные переменные
Player player;
Enemy enemies[MAX_ENEMIES];
Bullet bullets[MAX_BULLETS];
Bullet enemyBullets[MAX_ENEMY_BULLETS];
GameState gameState;
char screen[HEIGHT][WIDTH];

// Инициализация игры
void initGame() {
    player.x = WIDTH / 2;
    player.y = HEIGHT - 3;
    player.lives = 3;
    player.score = 0;
    
    gameState.level = 1;
    gameState.enemySpeed = 12;
    gameState.enemyCount = 12;
    gameState.enemyShootChance = 40;
    gameState.enemyShootDelay = 60;
    gameState.enemyShootCounter = 0;
    gameState.startTime = time(NULL);
    gameState.timeLimit = 180; // 3 минуты на уровень
    
    // Инициализация пуль
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = 0;
    }
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        enemyBullets[i].active = 0;
    }
}

void initLevel() {
    // Настройка сложности уровня
    gameState.enemyCount = 12 + (gameState.level - 1) * 6;
    if (gameState.enemyCount > MAX_ENEMIES) gameState.enemyCount = MAX_ENEMIES;
    
    gameState.enemySpeed = 12 - (gameState.level - 1);
    if (gameState.enemySpeed < 3) gameState.enemySpeed = 3;
    
    gameState.enemyShootChance = 30 + gameState.level * 8;
    if (gameState.enemyShootChance > 70) gameState.enemyShootChance = 70;
    
    // Уменьшаем задержку между выстрелами врагов с каждым уровнем
    gameState.enemyShootDelay = 45 - (gameState.level * 6);
    if (gameState.enemyShootDelay < 12) gameState.enemyShootDelay = 12;
    
    gameState.enemyShootCounter = 0;
    gameState.startTime = time(NULL);
    
    // Создание врагов с разнообразием
    int cols = 10;
    int rows = (gameState.enemyCount + cols - 1) / cols;
    int index = 0;
    
    for (int row = 0; row < rows && index < gameState.enemyCount; row++) {
        for (int col = 0; col < cols && index < gameState.enemyCount; col++) {
            enemies[index].x = 8 + col * 5;
            enemies[index].y = 3 + row * 3;
            enemies[index].active = 1;
            
            // Разнообразие врагов в зависимости от уровня и позиции
            if (gameState.level >= 3 && row == 0) {
                enemies[index].type = 2; // Сильные в первом ряду
                enemies[index].health = 2;
            } else if (gameState.level >= 2 && rand() % 3 == 0) {
                enemies[index].type = 1; // Быстрые враги
                enemies[index].health = 1;
            } else {
                enemies[index].type = 0; // Обычные
                enemies[index].health = 1;
            }
            index++;
        }
    }
    
    // Деактивация оставшихся врагов
    for (int i = index; i < MAX_ENEMIES; i++) {
        enemies[i].active = 0;
    }
    
    // Очистка пуль
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = 0;
    }
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        enemyBullets[i].active = 0;
    }
}

// Игровая логика
void shoot() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].x = player.x;
            bullets[i].y = player.y - 1;
            bullets[i].active = 1;
            break;
        }
    }
}

void enemyShoot(int enemyIndex) {
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (!enemyBullets[i].active) {
            enemyBullets[i].x = enemies[enemyIndex].x;
            enemyBullets[i].y = enemies[enemyIndex].y + 1;
            enemyBullets[i].active = 1;
            break;
        }
    }
}

void updateBullets() {
    static int playerBulletTick = 0;
    playerBulletTick++;

    // Обновление пуль игрока (замедлено: движение раз в 2 тика)
    if (playerBulletTick % 2 == 0) {
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i].active) {
                bullets[i].y--;
                if (bullets[i].y <= 0) {
                    bullets[i].active = 0;
                }
            }
        }
    }
    
    // Обновление пуль врагов
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (enemyBullets[i].active) {
            enemyBullets[i].y++;
            if (enemyBullets[i].y >= HEIGHT - 1) {
                enemyBullets[i].active = 0;
            }
        }
    }
}

void updateEnemies() {
    static int moveCounter = 0;
    moveCounter++;
    
    if (moveCounter >= gameState.enemySpeed) {
        moveCounter = 0;
        
        // Движение врагов
        static int direction = 1;
        int shouldMoveDown = 0;
        
        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (enemies[i].active) {
                int speed = (enemies[i].type == 1) ? 2 : 1; // Быстрые враги двигаются быстрее
                enemies[i].x += direction * speed;
                
                if (enemies[i].x <= 1 || enemies[i].x >= WIDTH - 2) {
                    shouldMoveDown = 1;
                }
            }
        }
        
        if (shouldMoveDown) {
            direction = -direction;
            for (int i = 0; i < MAX_ENEMIES; i++) {
                if (enemies[i].active) {
                    enemies[i].y++;
                    int speed = (enemies[i].type == 1) ? 2 : 1;
                    enemies[i].x += direction * speed;
                }
            }
        }
    }
    
    // Враги стреляют с контролируемой частотой
    gameState.enemyShootCounter++;
    if (gameState.enemyShootCounter >= gameState.enemyShootDelay) {
        gameState.enemyShootCounter = 0;
        
        // Выбираем случайного активного врага для выстрела
        int activeEnemies[MAX_ENEMIES];
        int activeCount = 0;
        
        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (enemies[i].active) {
                activeEnemies[activeCount++] = i;
            }
        }
        
        if (activeCount > 0 && rand() % 100 < gameState.enemyShootChance) {
            int randomEnemy = activeEnemies[rand() % activeCount];
            enemyShoot(randomEnemy);
        }
    }
}

void checkCollisions() {
    // Проверка попаданий пуль игрока по врагам
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            for (int j = 0; j < MAX_ENEMIES; j++) {
                if (enemies[j].active && 
                    bullets[i].x == enemies[j].x && bullets[i].y == enemies[j].y) {
                    bullets[i].active = 0;
                    enemies[j].health--;
                    
                    if (enemies[j].health <= 0) {
                        enemies[j].active = 0;
                        player.score += 10 * (enemies[j].type + 1) * gameState.level;
                    }
                    break;
                }
            }
        }
    }
    
    // Проверка попаданий пуль врагов по игроку
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (enemyBullets[i].active) {
            if ((enemyBullets[i].x == player.x || 
                 enemyBullets[i].x == player.x - 1 || 
                 enemyBullets[i].x == player.x + 1) &&
                enemyBullets[i].y == player.y) {
                enemyBullets[i].active = 0;
                player.lives--;
            }
        }
    }
}

int checkWin() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) return 0;
    }
    return 1;
}

int checkLose() {
    if (player.lives <= 0) return 1;
    
    int timeLeft = gameState.timeLimit - (int)(time(NULL) - gameState.startTime);
    if (timeLeft <= 0) return 1;
    
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active && enemies[i].y >= player.y - 1) return 1;
    }
    
    return 0;
}

// Диалог подтверждения перехода на следующий уровень с управлением стрелками/WS
static int confirmNextLevel(int nextLevel) {
    int selected = 0; // 0 - Yes, 1 - No
    int choice = -1;
    nodelay(stdscr, TRUE);
    while (choice == -1) {
        clear();
        drawBox(8, 10, 9, 50, COLOR_UI);
        attron(COLOR_PAIR(COLOR_BULLET_PLAYER) | A_BOLD);
        mvprintw(9, 15, "Continue to Level %d?", nextLevel);
        attroff(COLOR_PAIR(COLOR_BULLET_PLAYER) | A_BOLD);

        const char *options[2] = {"Yes", "No"};
        for (int i = 0; i < 2; i++) {
            if (i == selected) {
                attron(COLOR_PAIR(COLOR_TEXT) | A_REVERSE | A_BOLD);
            } else {
                attron(COLOR_PAIR(COLOR_TEXT));
            }
            mvprintw(11 + i, 18, "[%s]", options[i]);
            if (i == selected) {
                attroff(COLOR_PAIR(COLOR_TEXT) | A_REVERSE | A_BOLD);
            } else {
                attroff(COLOR_PAIR(COLOR_TEXT));
            }
        }

        attron(COLOR_PAIR(COLOR_UI));
        mvprintw(14, 14, "Use ↑/↓ or W/S to choose, Enter to confirm");
        attroff(COLOR_PAIR(COLOR_UI));

        refresh();
        int ch = getch();
        switch (ch) {
            case KEY_UP:
            case 'w':
            case 'W':
                selected = (selected + 1) % 2;
                break;
            case KEY_DOWN:
            case 's':
            case 'S':
                selected = (selected + 1) % 2;
                break;
            case '\n':
            case '\r':
            case ' ':
                choice = selected;
                break;
            case 27: // ESC
            case 'q':
            case 'Q':
                choice = 1; // No
                break;
            default:
                break;
        }
        napms(20);
    }
    return choice == 0;
}

// Основной игровой цикл
void gameLoop() {
    int running = 1;
    
    while (running) {
        // Обработка ввода
        int ch = getch();
        if (ch != ERR) {
            switch (ch) {
                case 'a':
                case 'A':
                case KEY_LEFT:
                    if (player.x > 2) player.x--;
                    break;
                case 'd':
                case 'D':
                case KEY_RIGHT:
                    if (player.x < WIDTH - 3) player.x++;
                    break;
                case ' ':
                    shoot();
                    break;
                case 27: // ESC
                    pauseGame();
                    break;
                case 'q':
                case 'Q':
                    running = 0;
                    break;
            }
        }
        
        // Обновление игры
        updateBullets();
        updateEnemies();
        checkCollisions();
        
        // Отрисовка
        initScreen();
        drawPlayer();
        drawEnemies();
        drawBullets();
        renderScreen();
        
        // Проверка условий победы/поражения
        if (checkWin()) {
            showGameOver(1);
            gameState.level++;
            
            if (gameState.level <= 5) {
                if (confirmNextLevel(gameState.level)) {
                    initLevel();
                } else {
                    running = 0;
                }
            } else {
                clear();
                drawBox(8, 10, 7, 50, COLOR_UI);
                attron(COLOR_PAIR(COLOR_BULLET_PLAYER) | A_BOLD);
                mvprintw(10, 12, "Congratulations! All levels completed!");
                mvprintw(11, 15, "Final Score: %d", player.score);
                attroff(COLOR_PAIR(COLOR_BULLET_PLAYER) | A_BOLD);
                refresh();
                napms(4000);
                running = 0;
            }
        }
        
        if (checkLose()) {
            showGameOver(0);
            running = 0;
        }
        
        napms(50); // 50ms задержка
    }
}
