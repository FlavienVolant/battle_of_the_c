#include "battleship.h"

#include <stdlib.h>
#include <stdio.h>

void init_board(struct Board *board)
{
    for(int i = 0; i < BOARD_SIZE; i++) {
        for(int j = 0; j < BOARD_SIZE; j++) {
            board->grid[i][j] = CELL_EMPTY;
        }
    }
    board->shipCount = 0;
}

char* board_to_string(const struct Board *board)
{   
    char* res = malloc(BOARD_SIZE * BOARD_SIZE + 1);
    res[BOARD_SIZE * BOARD_SIZE] = '\0';

    for(int i = 0; i < BOARD_SIZE; i++) {
        for(int j = 0; j < BOARD_SIZE; j++) {

            char symbol;

            switch (board->grid[i][j])
            {
                case CELL_EMPTY:
                    symbol = '~';
                    break;
                
                case CELL_HIT:
                    symbol = 'X';
                    break;
                
                case CELL_MISS:
                    symbol = '-';
                    break;
                
                case CELL_SHIP:
                    symbol = 'O';
                    break;
            }
            
            res[BOARD_SIZE * i + j] = symbol;
        }
    }

    return res;
}

void display(struct Board *board)
{
    char* str = board_to_string(board);
    char* abc = "ABCDEFGHIJ";

    printf("    0 1 2 3 4 5 6 7 8 9\n");

    for(int i = 0; i < BOARD_SIZE; i ++) {
        printf("%c |", abc[i]);
        for(int j = 0; j < BOARD_SIZE; j ++) {
            printf(" %c", str[i * BOARD_SIZE + j]);
        }
        printf(" |\n");
    }
}

