#ifndef POS_H
#define POS_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

struct pos {
    unsigned int r, c;
};

typedef struct pos pos;


typedef struct pq_entry pq_entry;

struct pq_entry {
    pos p;
    pq_entry* next;
};


struct posqueue {
    pq_entry *head, *tail;
    unsigned int len;
};

typedef struct posqueue posqueue;

//creates a pos value 
pos make_pos(unsigned int r, unsigned int c);

//allocate a new, empty queue
posqueue* posqueue_new();

//add a position to the queue, at the back
void pos_enqueue(posqueue* q, pos p);

//remove and return the element at the front of the queue, 
//or raise an error if the queue is empty
pos pos_dequeue(posqueue* q);

//checks if two positions are equal
bool pos_equal(pos p1, pos p2);

//determine if the given position is in the queue
bool posqueue_member(posqueue* q, pos p);

//entirely deallocate an existing queue
void posqueue_free(posqueue* q);

#endif /* POS_H */
