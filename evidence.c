#include "pos.h"
#include "board.h"
#include "logic.h"

//prints out a pos
void show_pos(pos p) {
    printf("%u,%u",p.r,p.c);
}

//prints out the elements of the posqueue to help me verify my functions
void show_posqueue(posqueue* q) {
    unsigned int length = q->len;
    if(length == 0) {
        printf("This is an empty queue\n");
    }

    pq_entry* front = q->head;

    for(unsigned int i = 0; i < length; i++) {
        if(i == length - 1) {
            show_pos(front->p);
        } else {
            show_pos(front->p); 
            printf("->");
            front = front->next;
        }
    }
}

void evidence_make_pos()
{
    printf("---- testing make_pos ----\n");
    pos test_position = make_pos(4,7);
    printf("expecting 4 7: %u,%u\n",test_position.r,test_position.c);
}

void evidence_posqueue_new() 
{
    printf("---- testing posqueue_new ----\n");
    posqueue* test_posqueue = posqueue_new();
    show_posqueue(test_posqueue);
    free(test_posqueue);
}

void evidence_pos_enqueue() 
{
    printf("---- testing pos_enqueue ----\n");
    posqueue* q = posqueue_new();
    pos p = make_pos(4,7);
    pos_enqueue(q, p);
    show_posqueue(q);
    printf("\n");
    pos p1 = make_pos(4,7);
    pos_enqueue(q, p1);
    show_posqueue(q);
    free(q);
    printf("\n");
}

void evidence_pos_dequeue() 
{
    printf("---- testing pos_dequeue ----\n");
    posqueue* q = posqueue_new();
    pos p = make_pos(0,0);
    pos p1 = make_pos(1,1);
    pos p2 = make_pos(2,2);
    pos_enqueue(q, p);
    pos_enqueue(q, p1);
    pos_enqueue(q, p2);
    show_posqueue(q);
    pos_dequeue(q);
    printf("\n");
    show_posqueue(q);
    printf("\n");
    free(q);
}

void evidence_pos_equal() 
{
    printf("---- testing pos_equal ----\n");
    pos p1 = make_pos(1,1);
    pos p2 = make_pos(2,2);
    printf("expecting false: ");
    if(pos_equal(p1,p2)) {
        printf("true\n");
    } else {
        printf("false\n");
    }    
}

void evidence_posqueue_member()
{
    printf("---- testing posqueue_member ----\n");
    posqueue* q = posqueue_new();
    pos p = make_pos(0,0);
    pos p1 = make_pos(1,1);
    pos p2 = make_pos(2,2);
    pos_enqueue(q, p);
    pos_enqueue(q, p1);
    pos_enqueue(q, p2);
    printf("expecting true: ");
    if(posqueue_member(q, p)) {
        printf("true\n");
    } else {
        printf("false\n");
    }
    free(q);
}

//prints out the elements of the board matrix to help me verify my functions
void print_board(cell** a, unsigned int nrows, unsigned int ncols) {
    unsigned int r, c;
    for (r = 0; r < nrows; r++) {
        for (c = 0; c < ncols; c++) {
            printf("%d",a[r][c]);
        }
        printf("\n");
    }
}

void evidence_board_new()
{
    printf("---- testing board_new ----\n");
    board* b = board_new(5,4,MATRIX);
    print_board(b->u.matrix,b->height,b->width);
    //returns array of 0's becuase val of enum EMPTY is 0
    board_free(b);
}

void evidence_col_head()
{
    col_head(5);
    printf("\n");
    col_head(14);
    printf("\n");
    col_head(37);
    printf("\n");
    col_head(63);
    printf("\n");
}

void evidence_row_head()
{
    row_head(61);
    printf("\n");
}

void evidence_draw_space()
{
    cell test = EMPTY;
    cell test1 = BLACK;
    cell test2 = WHITE;
    draw_space(test);
    printf("\n");
    draw_space(test1);
    printf("\n");
    draw_space(test2);
    printf("\n");
}

void evidence_board_show()
{
    board* b = board_new(5,4,MATRIX);
    board* b1 = board_new(12,5,MATRIX);
    board_show(b);
    printf("\n");
    board_show(b1);
    board_free(b);
    board_free(b1);
}

void evidence_board_get()
{
    board* b1 = board_new(5,4,MATRIX);
    pos pos1 = make_pos(2,2);
    //will return 0 because that is how the enum EMPTY is stored in c
    printf("%d\n",board_get(b1,pos1));
    board_free(b1);
}

void evidence_board_set() 
{
    board* b1 = board_new(5,4,MATRIX);
    pos pos1 = make_pos(2,2);
    cell cell1 = BLACK;
    board_set(b1, pos1, cell1);
    board_show(b1);
    printf("\n");
    board_free(b1);
}

void evidence_new_game()
{
    game* g = new_game(4, 1, 6, 6, MATRIX);
    game_free(g);
}

void evidence_place_piece()
{
    
    game* g = new_game(4, 3, 6, 6, MATRIX);
    pos p = make_pos(2,2);
    pos p1 = make_pos(3,3);
    printf("expecting true: ");
    if(place_piece(g, p)) {
        printf("true\n");
    } else {
        printf("false\n");
    }
    board_show(g->b);
    printf("\n");
    place_piece(g,p1);
    board_show(g->b);
    pos p2 = make_pos(5,5);
    place_piece(g,p2);
    board_show(g->b);
    show_posqueue(g->hanging);
    printf("\n");

    game* og = new_game(3, 2, 8, 4, MATRIX);
    board_show(og->b);
    pos op = make_pos(1,1);
    place_piece(og,op);
    board_show(og->b);
    pos op1 = make_pos(0,3);
    place_piece(og,op1);
    board_show(og->b);
    pos op2 = make_pos(2,3);
    printf("\n");
    printf("expecting 0: %d\n",board_get(og->b,(make_pos(0,1))));
    printf("expecting 2: %d\n",board_get(og->b,op1));
    place_piece(og,op2); 
    board_show(og->b);
    //show_posqueue(og->hanging);
    pos op3 = make_pos(1,2);
    place_piece(og,op3);
    board_show(og->b);
    //show_posqueue(og->hanging);
    pos op4 = make_pos(0,2);
    place_piece(og,op4);
    board_show(og->b);
    //show_posqueue(og->hanging); 
    pos op5 = make_pos(1,4);
    place_piece(og,op5);
    board_show(og->b);
    //show_posqueue(og->hanging); 

    printf("---- testing game_outcome ----\n");
    printf("expecting 2: %d\n",game_outcome(og)); 
    printf("expecting 0: %d\n",game_outcome(g)); 

    game_free(og);
    game_free(g);

    game* g1 = new_game(3, 2, 5, 5, BITS);
    pos p4 = make_pos(1,1);
    pos p5 = make_pos(3,4);
    pos p6 = make_pos(2,2);
    pos p7 = make_pos(4,4);
    pos p8 = make_pos(4,3);
    place_piece(g1,p4);
    place_piece(g1,p5);
    place_piece(g1,p6);
    place_piece(g1,p7);
    place_piece(g1,p8);
    board_show(g1->b);
    printf("expecting 1: %d\n",game_outcome(g1));

    
    game* g2 = new_game(3, 2, 5, 5, MATRIX);
    pos p9 = make_pos(1,1);
    pos p10 = make_pos(3,4);
    pos p11 = make_pos(2,2);
    pos p12 = make_pos(4,4);
    pos p13 = make_pos(4,3);
    place_piece(g2,p9);
    place_piece(g2,p10);
    place_piece(g2,p11);
    place_piece(g2,p12);
    place_piece(g2,p13);
    board_show(g2->b);
    printf("expecting 1: %d\n",game_outcome(g2));

    game_free(g2);
    
    game* g3 = new_game(3, 2, 5, 5, MATRIX);
    pos p14 = make_pos(4,3);
    pos p15 = make_pos(4,2);
    pos p16 = make_pos(4,4);
    pos p17 = make_pos(3,2);
    pos p18 = make_pos(2,3);
    pos p19 = make_pos(0,2);
    pos p20 = make_pos(0,0);
    pos p21 = make_pos(1,1);
    place_piece(g3,p14);
    place_piece(g3,p15);
    place_piece(g3,p16);
    place_piece(g3,p17);
    place_piece(g3,p18);
    place_piece(g3,p19);
    place_piece(g3,p20);
    place_piece(g3,p21);
    board_show(g3->b);
    printf("expecting 2: %d\n",game_outcome(g3));

    game_free(g3);

    game* g4 = new_game(3, 2, 5, 5, MATRIX);
    pos p22 = make_pos(4,3);
    pos p23 = make_pos(2,2);
    pos p24 = make_pos(3,3);
    place_piece(g4,p22);
    place_piece(g4,p23);
    place_piece(g4,p24);
    board_show(g4->b);
    printf("expecting 0: %d\n",game_outcome(g4));

    game_free(g4);

    game* g5 = new_game(3, 2, 5, 5, BITS);
    pos p25 = make_pos(4,3);
    pos p26 = make_pos(2,2);
    pos p27 = make_pos(3,3);
    place_piece(g5,p25);
    place_piece(g5,p26);
    place_piece(g5,p27);
    board_show(g5->b);
    printf("expecting 0: %d\n",game_outcome(g5));

    game_free(g5);
}


int main(int argc, char *argv[])
{
    evidence_make_pos();
    evidence_posqueue_new();
    evidence_pos_enqueue();
    evidence_pos_dequeue();
    evidence_pos_equal();
    evidence_posqueue_member();
    evidence_board_new();
    evidence_col_head();
    evidence_row_head();
    evidence_draw_space();
    evidence_board_show();
    evidence_board_get();
    evidence_board_set();
    evidence_new_game();
    evidence_place_piece();
}