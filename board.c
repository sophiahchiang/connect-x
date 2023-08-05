#include "board.h"

//helper function to create a 2-D array of EMPTY cells (MATRIX)
cell** arr_new(unsigned int nrows, unsigned int ncols) {
    cell** a = (cell**)malloc(nrows * sizeof(cell*));
    if (a == NULL) {
        fprintf(stderr, "arr_new: allocation failed\n");
        exit(1);
    }
    unsigned int r, c;
    for (r = 0; r < nrows; r++) {
        a[r] = (cell*)malloc(ncols * sizeof(cell));
        for (c = 0; c < ncols; c++) {
           a[r][c] = EMPTY;
       }
    }
    return a;
}

//helper function to create a 1-D array of empty (00) cells (BITS)
unsigned int* bit_new(unsigned int nrows, unsigned int ncols) {
    unsigned int total_bits = 2 * nrows * ncols;
    unsigned int x = total_bits%32;
    unsigned int num_elements;

    if(x != 0) {
        num_elements = ((total_bits - x)/32) + 1;  
    } else {
        num_elements = total_bits/32;
    }
    unsigned int* a = (unsigned int*)malloc(num_elements * sizeof(int));
    if (a == NULL) {
        fprintf(stderr, "bit_new: allocation failed\n");
        exit(1);
    }
    for(unsigned int i = 0; i < num_elements; i++) {
        a[i] = 0;
    }
    return a;
}

//called to create a fully-empty board of the desired size
//for now, only implement matrix representation
//error if bits representation is called
board* board_new(unsigned int width, unsigned int height, enum type type) {

    board* out = (board*)malloc(sizeof(board));
    if (out == NULL) {
        fprintf(stderr, "board_new: allocation failed\n");
        exit(1);
    }

    out->width = width;
    out->height = height;
    out->type = type;

    if(type == MATRIX) {
        out->u.matrix = arr_new(height, width);
    } else if(type == BITS) {
        out->u.bits = bit_new(height,width);
    } else {
        fprintf(stderr, "board_new: invalid representation requested\n");
        exit(1);
    }    
    return out;
}

//helper function to free a 2-D array of cells (MATRIX) 
void arr_free(cell** a, unsigned int nrows) {
    unsigned int r;
    for(r = 0; r < nrows; r++) {
            free(a[r]);
    }
    free(a);
}

//frees the board, including whichever representation it uses
//for now, raise an error if bits representation is called
void board_free(board* b) {
    if(b->type == BITS) {
        free(b->u.bits);
        free(b);
    } else if(b->type == MATRIX) {
        arr_free(b->u.matrix,b->height);
        free(b);
    } else {
        fprintf(stderr, "board_new: invalid representation requested\n");
        exit(1);
    }   
}

//helper to print column header
void col_head(unsigned int ncols) {
    if (ncols <= 10) {
        for(unsigned int i = 0; i < ncols; i++) {
            printf("%u",i); }
    }
    if (ncols > 10 && ncols <= 36) {
        for(unsigned int i = 0; i < 10; i++) {
            printf("%u",i); }
        for(unsigned int j = 0; j < ncols - 10; j++) {
            char c = j+65;
            printf("%c",c); }
    } 
    if (ncols > 36 && ncols <= 62) {
        for(unsigned int i = 0; i < 10; i++) {
            printf("%u",i); }
        for(unsigned int j = 0; j < 26; j++) {
            char c = j+65;
            printf("%c",c); }
        for(unsigned int k = 0; k < ncols - 36; k++) {
            char c = k+97;
            printf("%c",c); }
    }
    if (ncols > 62) {
        for(unsigned int i = 0; i < 10; i++) {
            printf("%u",i); }
        for(unsigned int j = 0; j < 26; j++) {
            char c = j+65;
            printf("%c",c); }
        for(unsigned int k = 0; k < 26; k++) {
            char c = k+97;
            printf("%c",c); }
        for(unsigned int l = 0; l < ncols - 62; l++) {
            printf("?"); }
    }
}   

//helper to print row header
void row_head(unsigned int row) {
    if (row < 10) {
        printf("%u ",row); }
    if (row > 10 && row < 36) {
        char c = row-10+65;
        printf("%c ",c); 
    } 
    if (row > 36 && row < 62) {
        char c = row-36+97;
        printf("%c ",c); 
    }
    if (row >= 62) {
        printf("? "); 
    }
}

//helper function to print either a black piece, white piece, or blank spot
void draw_space(enum cell i) {
    switch (i) {
        case EMPTY:
            printf(".");
            break;
        case BLACK:
            printf("*");
            break;
        case WHITE:
            printf("o");
            break;
    }
}

//helper to give number of position in board
unsigned int pos_to_num(board* b, pos p) {
    unsigned int width = b->width;
    unsigned int row = p.r;
    unsigned int col = p.c;

    return (width * row) + col + 1;
}

//helper that retrieves a cell from the board at a given position (BITS)
cell board_get_bits(board* b, pos p) {
    unsigned int x = pos_to_num(b,p);
    unsigned int index; 
    if(x%16 == 0) {
        index = x/16 - 1;
    } else {
        index = x/16;
    }

    int tmp = b->u.bits[index];
    tmp = tmp >> 2*(x%16); //takes of __ num of bits on the right, fills in 0 on left    
    tmp &= 0X00000003; //leaves just the last two least significant bits

    if(tmp == 0) {
        return EMPTY;
    } else if (tmp == 1) {
        return BLACK;
    } else { //if (tmp == 2)
        return WHITE;
    }
}

//prints the board to the screen along with row and column headers
void board_show(board* b) {
    if(b->type == BITS) {
        unsigned int r, c;
        unsigned int nrows = b->height;
        unsigned int ncols = b->width;

        printf("  ");
        col_head(b->width);
        printf("\n");

        for (r = 0; r < nrows; r++) {
            row_head(r);
            for (c = 0; c < ncols; c++) {
                pos p = make_pos(r,c);
                draw_space(board_get_bits(b, p));
            }
            printf("\n");
        }
    } else if(b->type == MATRIX) {
        unsigned int r, c;
        unsigned int nrows = b->height;
        unsigned int ncols = b->width;
        cell** a = b->u.matrix;

        printf("  ");
        col_head(b->width);
        printf("\n");

        for (r = 0; r < nrows; r++) {
            row_head(r);
            for (c = 0; c < ncols; c++) {
                draw_space(a[r][c]);
        }
        printf("\n");
        }
    } else {
        fprintf(stderr, "board_show: invalid representation requested\n");
        exit(1); 
    }  
}

//retrieves a cell from the board at a given position
//return an error if specificed position p is not inside the board 
cell board_get(board* b, pos p) {
    if(p.r > b->height-1 || p.c > b->width-1) {
        fprintf(stderr, 
                "board_get: position is incompatible with board size\n");
        exit(1);
    }

    if(b->type == MATRIX) {
        return b->u.matrix[p.r][p.c];
    } else if (b->type == BITS) {
        return board_get_bits(b,p);
    } else {
        fprintf(stderr, "board_get: invalid representation requested\n");
        exit(1);
    } 
}

//helper function to modify a cell in the board (BITS)
void board_set_bits(board* b, pos p, cell c) {
    unsigned int x = pos_to_num(b,p);
    unsigned int index; 
    if(x%16 == 0) {
        index = x/16 - 1; //indexing starts at 0
    } else {
        index = x/16; //bc will round down, truncated division
    }

    if(c == BLACK) {
        unsigned int black = 0X00000001;
        black = black << 2*(x%16);
        b->u.bits[index] |= black;
    } else if (c == WHITE) {
        unsigned int white = 0X00000002;
        white = white << 2*(x%16);
        b->u.bits[index] |= white;
    } else if (c == EMPTY) {
        unsigned int empty = 0X00000003;
        empty = empty << 2*(x%16);
        empty = ~empty; 
        b->u.bits[index] &= empty;
    }
}
//modifies a cell in the board
void board_set(board* b, pos p, cell c) {
    if(b->type == MATRIX) {
        b->u.matrix[p.r][p.c] = c;
    } else if (b->type == BITS) {
        board_set_bits(b, p, c);
    } else {
        fprintf(stderr, "board_set: invalid representation requested\n");
        exit(1); 
    }
}