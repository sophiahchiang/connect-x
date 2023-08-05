#ifndef LOGIC_H
#define LOGIC_H

#include <stdbool.h>
#include "board.h"


enum turn {
    BLACKS_TURN,
    WHITES_TURN
};

typedef enum turn turn;


enum outcome {
    IN_PROGRESS,
    BLACK_WIN,
    WHITE_WIN,
    DRAW
};

typedef enum outcome outcome;


struct game {
    unsigned int run, hangtime;
    board* b;
    posqueue* hanging;
    turn player;
};

typedef struct game game;

//creates a new game with the specified size and parameters
game* new_game(unsigned int run, unsigned int hangtime, unsigned int width,
               unsigned int height, enum type type);

//free the game
void game_free(game* g);

//places a piece belonging to the player whose turn it is  
//(the player field of the game struct) at the specificed position p
//if spot is taken, do nothing and return false
//otherwise return true
bool place_piece(game* g, pos p);

//helper to check if there are any empty spaces on the board
bool full_board(game* g);

//helper to return the cell color of a run that is long enough to win
cell game_help(game* g);

//returns the outcome of a completed game, or that the game is in progress
outcome game_outcome(game* g);

#endif /* LOGIC_H */
