#ifndef BATTLESHIP_H
#define BATTLESHIP_H

#define BOARD_SIZE 10
#define MAX_SHIPS 5

enum CellState{
    CELL_EMPTY,
    CELL_SHIP,
    CELL_HIT,
    CELL_MISS
};

struct Position {
    int x;
    int y;
};

struct Ship {
    struct Position start;
    int length;
    int horizontal : 1;
    int hits;
};

struct Board {
    enum CellState grid[BOARD_SIZE][BOARD_SIZE];
    struct Ship ships[MAX_SHIPS];
    int shipCount;
};

void init_board(struct Board *board);
char* board_to_string(const struct Board *board);
void display(struct Board *board);

#endif