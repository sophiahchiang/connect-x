#ifndef BOARD_H
#define BOARD_H

#include "pos.h"


enum cell {
    EMPTY,
    BLACK,
    WHITE
};

typedef enum cell cell;


union board_rep {
    enum cell** matrix;
    unsigned int* bits;
};

typedef union board_rep board_rep;

enum type {
    MATRIX, BITS
};


struct board {
    unsigned int width, height;
    enum type type;
    board_rep u;
};

typedef struct board board;

//helper function to create a 2-D array of EMPTY cells
cell** arr_new(unsigned int nrows, unsigned int ncols);

//called to create a fully-empty board of the desired size
//for now, only implement matrix representation
//error if bits representation is called
board* board_new(unsigned int width, unsigned int height, enum type type);

//helper function to free a 2-D array of cells 
void arr_free(cell** a, unsigned int nrows);

//frees the board, including whichever representation it uses
//for now, raise an error if bits representation is called
void board_free(board* b);

//helper to print column header
void col_head(unsigned int ncols);

//helper to print row header
void row_head(unsigned int nrows);

//helper function to print either a black piece, white piece, or blank spot
void draw_space(cell);

//prints the board to the screen along with row and column headers
void board_show(board* b);

//retrieves a cell from the board
cell board_get(board* b, pos p);

//modifies a cell in the board
void board_set(board* b, pos p, cell c);

#endif /* BOARD_H */
