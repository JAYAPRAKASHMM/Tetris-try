#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "gameState.h" // Include the header file that defines the GameState struct
#ifndef GAME_H
#define GAME_H

void lockTetromino(GameState *gameState);
int checkGameOver(GameState *gameState);

#endif // GAME_H

// Define constants for game parameters
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define NUM_TETROMINOES 7

// Define data structures for game objects
typedef struct {
    int x;
    int y;
} Point;
typedef struct {
    int board[BOARD_HEIGHT][BOARD_WIDTH];
    Tetromino currentTetromino;
    int score;
    int level;
    // Add additional game state variables as needed
} GameState;
typedef struct {
    Point points[4];
    int shape;
} Tetromino;

typedef struct {
    int board[BOARD_HEIGHT][BOARD_WIDTH];
    Tetromino currentTetromino;
    int score;
    int level;
    // Add additional game state variables as needed
} GameState;
void handleInput(GameState *gameState, char input);
void updateGame(GameState *gameState);
void renderGame(GameState *gameState);
bool checkGameOver(GameState *gameState);
void initializeGame(GameState *gameState);
void spawnTetromino(GameState *gameState);
void moveTetromino(GameState *gameState, int dx, int dy);
void rotateTetromino(GameState *gameState);
void clearLines(GameState *gameState);
bool checkCollision(GameState *gameState, int dx, int dy);
bool checkGameOver(GameState *gameState);
void renderGame(GameState *gameState);
void updateGame(GameState *gameState);
void handleInput(GameState *gameState, char input);
int main() {
    // Initialize game state
    GameState gameState;
    initializeGame(&gameState);

    // Game loop
    while (!checkGameOver(&gameState)) {
        // Handle user input
        char input;
        scanf("%c", &input);
        handleInput(&gameState, input);

        // Update game state
        updateGame(&gameState);

        // Render game
        renderGame(&gameState);
    }

    // Game over
    printf("Game Over!\n");

    return 0;
}

// Tetromino shapes
int tetrominoShapes[NUM_TETROMINOES][4][2] = {
    {{0, 1}, {1, 1}, {2, 1}, {3, 1}},   // I shape
    {{1, 0}, {0, 1}, {1, 1}, {2, 1}},   // T shape
    {{0, 0}, {0, 1}, {1, 1}, {2, 1}},   // L shape
    {{2, 0}, {0, 1}, {1, 1}, {2, 1}},   // J shape
    {{0, 1}, {1, 1}, {1, 0}, {2, 0}},   // S shape
    {{0, 0}, {1, 0}, {1, 1}, {2, 1}},   // Z shape
    {{0, 0}, {0, 1}, {1, 0}, {1, 1}}    // O shape
};

// Function implementations
void initializeGame(GameState *gameState) {
    // Initialize game state
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            gameState->board[i][j] = 0;
        }
    }
    gameState->score = 0;
    gameState->level = 1;

    // Seed the random number generator
    srand(time(0));

    // Spawn initial tetromino
    spawnTetromino(gameState);
}
void spawnTetromino(GameState *gameState) {
    // Create a new tetromino at the top center of the game board
    Tetromino newTetromino;
    newTetromino.points[0].x = BOARD_WIDTH / 2;
    newTetromino.points[0].y = 0;

    // Choose a random shape for the new tetromino
    newTetromino.shape = rand() % NUM_TETROMINOES;

    // Copy the new tetromino to the current tetromino in the game state
    gameState->currentTetromino = newTetromino;
}

void moveTetromino(GameState *gameState, int dx, int dy) {
    // Check for collision before moving the tetromino
    if (!checkCollision(gameState, dx, dy)) {
        // Update the x and y coordinates of each point in the tetromino
        for (int i = 0; i < 4; i++) {
            gameState->currentTetromino.points[i].x += dx;
            gameState->currentTetromino.points[i].y += dy;
        }
    }
}

void rotateTetromino(GameState *gameState) {
    // Save the current shape of the tetromino
    int currentShape = gameState->currentTetromino.shape;

    // Update the shape of the tetromino
    gameState->currentTetromino.shape = (currentShape + 1) % NUM_TETROMINOES;

    // Check for collision after rotating the tetromino
    if (checkCollision(gameState, 0, 0)) {
        // If there is collision, revert the shape back to the original shape
        gameState->currentTetromino.shape = currentShape;
    }
}

void clearLines(GameState *gameState) {
    // Check each row of the game board for completed lines
    for (int i = BOARD_HEIGHT - 1; i >= 0; i--) {
        bool isLineComplete = true;
        for (int j = 0; j < BOARD_WIDTH; j++) {
            if (gameState->board[i][j] == 0) {
                isLineComplete = false;
                break;
            }
        }
        if (isLineComplete) {
            // Shift all lines above the completed line by one row down
            for (int k = i; k > 0; k--) {
                for (int j = 0; j < BOARD_WIDTH; j++) {
                    gameState->board[k][j] = gameState->board[k - 1][j];
                }
            }
            // Clear the top row
            for (int j = 0; j < BOARD_WIDTH; j++) {
                gameState->board[0][j] = 0;
            }
            gameState->score += 100; // Add score for clearing a line
        }
    }
}

bool checkCollision(GameState *gameState, int dx, int dy) {
// Check each point of the current tetromino for collision
for (int i = 0; i < 4; i++) {
int x = gameState->currentTetromino.points[i].x + dx;
int y = gameState->currentTetromino.points[i].y + dy;

    // Check if the point is outside the game board
    if (x < 0 || x >= BOARD_WIDTH || y >= BOARD_HEIGHT) {
        return true; // Collision detected
    }

    // Check if the point collides with an existing cell on the game board
    if (y >= 0 && gameState->board[y][x] != 0) {
        return true; // Collision detected
    }
}

return false; // No collision
}

void mergeTetromino(GameState *gameState) {
// Merge the current tetromino with the game board
for (int i = 0; i < 4; i++) {
int x = gameState->currentTetromino.points[i].x;
int y = gameState->currentTetromino.points[i].y;
    // Set the corresponding cell on the game board to the shape of the tetromino
    gameState->board[y][x] = gameState->currentTetromino.shape + 1;
}
}
void gameOver(GameState *gameState) {
// Game over logic
printf("Game Over\n");
printf("Final Score: %d\n", gameState->score);
// Additional game over logic here
}
void handleInput(GameState *gameState, char input) {
    switch (input) {
        case 'a': // Move left
            moveTetromino(gameState, -1, 0);
            break;
        case 'd': // Move right
            moveTetromino(gameState, 1, 0);
            break;
        case 's': // Move down
            moveTetromino(gameState, 0, 1);
            break;
        case 'w': // Rotate
            rotateTetromino(gameState);
            break;
        case ' ': // Hard drop
            while (!checkCollision(gameState, 0, 1)) {
                moveTetromino(gameState, 0, 1);
            }
            updateGame(gameState);
            break;
        case 'q': // Quit
            exit(0);
            break;
        default:
            // Invalid input, do nothing
            break;
    }
}
void updateGame(GameState *gameState) {
    // Move the current tetromino down by one unit
    moveTetromino(gameState, 0, 1);

    // Check for collision after moving the tetromino
    if (checkCollision(gameState, 0, 1)) {
        // If there is collision, lock the current tetromino in place
        lockTetromino(gameState);

        // Check for completed lines and clear them
        clearLines(gameState);

        // Spawn a new tetromino
        spawnTetromino(gameState);

        // Check for game over
        if (checkGameOver(gameState)) {
            // If game over, print game over message
            printf("Game Over!\n");
            exit(0); // Exit the program
        }
    }
}
void renderGame(GameState *gameState) {
    // Clear the screen
    system("clear");

    // Render the game board
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            if (gameState->board[i][j] == 0) {
                printf(" . "); // Empty cell
            } else {
                printf(" # "); // Tetromino cell
            }
        }
        printf("\n");
    }

    // Render the current tetromino
    Tetromino currentTetromino = gameState->currentTetromino;
    for (int i = 0; i < 4; i++) {
        int x = currentTetromino.points[i].x;
        int y = currentTetromino.points[i].y;
        printf("(%d,%d) ", x, y); // Render the tetromino points as (x,y) coordinates
    }

    // Render game state information
    printf("\n");
    printf("Score: %d\n", gameState->score);
    printf("Level: %d\n", gameState->level);
}







