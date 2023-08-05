#include "logic.h"


//creates a new game with the specified size and parameters
game* new_game(unsigned int run, unsigned int hangtime, unsigned int width,
               unsigned int height, enum type type) {
    if(run > width || run > height) {
        fprintf(stderr, 
                "new_game: run parameter is incompatible with board size\n");
        exit(1);
    }

    game* out = (game*)malloc(sizeof(game));
    if (out == NULL) {
        fprintf(stderr, "new_game: allocation failed\n");
        exit(1);
    }

    out->run = run;
    out->hangtime = hangtime;
    out->b = board_new(width, height, type);
    out->hanging = posqueue_new();
    out->player = BLACKS_TURN;

    return out;
                        
}

//free the game
void game_free(game* g) {

    board_free(g->b);
    posqueue_free(g->hanging);
    free(g);

}

/*
1. Check that the column below the just removed piece is empty
    1. If yes, it will fall to the bottom
    2. If no, find the first piece that is beneath it in the 
       same column and add one to the row for the fallen piece’s position
2. Check that the column above the just removed piece is empty
    1. If yes, set the place where it used to be to empty… nothing else will move
    2. If no, all pieces above that are no longer in the queue 
    (i.e. all pieces that have lost their ability to hover) will also fall 
*/

//helper that checks if the column below the just removed piece is empty
//pos p = the original position of the fallen piece 
//color = player who's turn it is
void col_below(game* g, pos p, cell color) {
    for(unsigned int i = p.r + 1; i <= g->b->height-1; i++) {
        pos og = make_pos(i,p.c);
        if(board_get(g->b, og) != EMPTY) {
            pos new = make_pos(i-1,p.c);
            board_set(g->b,p,EMPTY);
            board_set(g->b,new,color);
            return;
        }
    }
    //if you make it through the whole for loop w/o exiting then you know 
    //column below just removed piece is empty, so you can place the 
    //piece in the bottomost row
    pos new1 = make_pos(g->b->height-1,p.c);
    board_set(g->b,p,EMPTY);
    board_set(g->b,new1,color);         
}

//if in the first row, there couldn't have been anything above it, 
//so immediately set spot to empty
void col_above(game* g, pos p) {
/*    if(p.r == 0) {
        board_set(g->b,p,EMPTY);
    }*/

    for(int i = p.r - 1; i >= 0; i--) {
        pos tmp = make_pos((unsigned int)i,p.c);
        if((board_get(g->b,tmp) == BLACK) 
            && !(posqueue_member(g->hanging, tmp))) {
                col_below(g,tmp,BLACK);               
        } else if ((board_get(g->b,tmp) == WHITE) 
            && !(posqueue_member(g->hanging, tmp))) {
                col_below(g,tmp,WHITE);
        } //else { //everywhere above the just removed piece is empty
            //board_set(g->b,p,EMPTY);
        }
    
}
//helper that simulates gravity... dequeues the piece that has
//exceeded its hangtime
//only one piece will ever fall at one time
void gravity(game* g) {
    unsigned int num_turns = g->hanging->len;
    if(num_turns <= g->hangtime) {
        return;
    } else {
        pos fallen = pos_dequeue(g->hanging); 
        col_below(g,fallen,board_get(g->b,fallen));
        col_above(g,fallen);
    }
}

/*
1. places a piece belonging to the player whose turn it is in specified pos
2. if spot is taken, do nothing and return false --> no changes to game struct
3. otherwise, simulate the effect of gravity on any unsupported pieces 
   that have lost their ability to hover
4. change the turn to the next player
5. after all of the above has been executed, now return true 
*/

bool place_piece(game* g, pos p) {
    if(board_get(g->b,p) != EMPTY) {
        return false;
    } else {
        if(g->player == BLACKS_TURN) {
            board_set(g->b,p,BLACK);
            pos_enqueue(g->hanging,p);
            gravity(g);             
            g->player = WHITES_TURN;
            return true;
        } else {
            board_set(g->b,p,WHITE);
            pos_enqueue(g->hanging,p);
            gravity(g);
            g->player = BLACKS_TURN;
            return true;
        }        
    }
}

//helper function that returns true if the board is full and false if 
//there is at least one space
bool full_board(game* g) {
    for(unsigned int i = 0; i < g->b->width; i++) {
        for(unsigned int j = 0; j < g->b->height; j++) {
            if (board_get(g->b, make_pos(j,i)) == EMPTY) {
                return false;
            }
        }
    }
    return true;
}

//returns the outcome of a completed game, or that the game is in progress
//DRAW if: 1. both players have runs, 2. board is full and neither have runs
outcome game_outcome(game* g) {
    unsigned int num_to_win = g->run; 
    unsigned int maxx = g->b->width;
    unsigned int maxy = g->b->height;

    bool white_won = false;
    bool black_won = false;

    int directions[4][2] = {{1,0}, {1,-1}, {1,1}, {0,1}};

    for(unsigned int i = 0; i < 4; i++) {
        int dx = directions[i][0];
        int dy = directions[i][1];

        for(unsigned int x = 0; x < maxx; x++) {
            for(unsigned int y = 0; y < maxy; y++) {
                
                int lastx = x + (num_to_win - 1) * dx;
                int lasty = y + (num_to_win - 1) * dy;
                if (0 <= lastx && lastx < maxx && 0 <= lasty && lasty < maxy) {
                    cell w = board_get(g->b, make_pos(y,x));
                    if ((!white_won && w == WHITE)
                        || (!black_won && w == BLACK)) {
                        int count = 1;
                        for(unsigned int j = 1; j < num_to_win; j++) {
                            if(w != board_get(g->b, make_pos(y + (j*dy), x + (j*dx)))) {
                                ;
                            } else {
                                count++;
                            }
                        }
                        if(count == num_to_win) {
                            if(w == WHITE) {
                                white_won = true;
                            } else {
                                black_won = true;
                            }
                        } 
                    }
                }
            }
        }
    }

    if(white_won && black_won) {
        return DRAW;
    } else if (white_won) {
        return WHITE_WIN;
    } else if (black_won) {
        return BLACK_WIN;
    } else if (full_board(g)) {
        return DRAW;
    } else {
        return IN_PROGRESS;
    }
}
