#include "play.h"

//converts an input char to an unsigned int row or column
unsigned int char_to_uint(char in) {
    unsigned int x; 

    int i = (unsigned int)in; //puts ASCII int value of x into i
    if(i >= 48 && i <= 57) {
        x = i - 48;  
    } else if (i >= 65 && i <= 90) {
        x = i - 65 + 10; 
    } else if (i >= 97 && i <= 122) {
        x = i - 97 + 36;
    } else {
        x = 66; //arbitrary value chosen to designate invalid input,
                //change to negative 1?
    }
    return x;
}

//runs a new game
void run_new_game(game* g) {
    board_show(g->b);

    unsigned int width = g->b->width;
    unsigned int height = g->b->height;
    
    if(g->player == BLACKS_TURN) {
        printf("Black:");
    } else {
        printf("White:");
    }
    
    char r, c;
    scanf("%c%c%*c", &r, &c);

    unsigned int new_r = char_to_uint(r);
    unsigned int new_c = char_to_uint(c);

    if(new_r == 66 || new_c == 66) {
        printf("Can't place piece: invalid input cannot be parsed,"
                " please try again\n");
        run_new_game(g);
    } 

    if(new_r >= height || new_c >= width) {
        printf("Can't place piece: position is off the board,"
                " please try again\n");
        run_new_game(g);        
    }

    pos new = make_pos(new_r,new_c);
    bool placed = place_piece(g,new);

    if(!placed) {
        printf("Can't place piece: position is already occupied,"
                " please try again\n");
        run_new_game(g);
    }

    switch(game_outcome(g)) {
        case IN_PROGRESS:
            run_new_game(g);
            break;
        case BLACK_WIN:
            board_show(g->b);
            printf("\n");
            printf("Black won!\n");
            break;
        case WHITE_WIN:
            board_show(g->b);
            printf("\n");
            printf("White won!\n");
            break;
        case DRAW:
            board_show(g->b);
            printf("\n");
            printf("Draw\n");
            break;
    }
}

int main(int argc, char *argv[])
{
    unsigned int width; 
    unsigned int height; 
    unsigned int run; 
    unsigned int hangtime;
    enum type type; 

    for(int i = 1; i < argc; i+=2) {
        if (strcmp(argv[i], "-w") == 0) {
            width = atoi(argv[i+1]);
        } else if (strcmp(argv[i], "-h") == 0) {
            height = atoi(argv[i+1]);
        } else if (strcmp(argv[i], "-r") == 0) {
            run = atoi(argv[i+1]);
        } else if (strcmp(argv[i], "-t") == 0) {
            hangtime = atoi(argv[i+1]);
        } else if (strcmp(argv[i], "-b") == 0) {
            type = BITS;
        } else if (strcmp(argv[i], "-m") == 0) {
            type = MATRIX;
        }
    }

    if(width >= 62 || height >= 62) {
        fprintf(stderr, "invalid board size: " 
                        "height and width must be less than 62\n");
        exit(1);
    }

    game* g = new_game(run, hangtime, width, height, type);
    run_new_game(g);
}