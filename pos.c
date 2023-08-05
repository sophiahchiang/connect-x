#include "pos.h"

//creates a pos value
pos make_pos(unsigned int r, unsigned int c) {
    
    pos out;
    out.r = r;
    out.c = c;

    return out;
}

//allocate a new, empty queue
posqueue* posqueue_new() {
    posqueue* out = (posqueue*)malloc(sizeof(posqueue));
    if (out == NULL) {
        fprintf(stderr, "posqueue_new: allocation failed\n");
        exit(1);
    } 
    out->len = 0;
    out->head = NULL;
    out->tail = NULL;

    return out;
}

//add a position to the queue, at the back
//check if the function was appropriately given a p to insert
//if not, raise an error **************
void pos_enqueue(posqueue* q, pos p) {
    
//malloc space for pq_entry with pos p
    pq_entry* out = (pq_entry*)malloc(sizeof(pq_entry));
        if (out == NULL) {
            fprintf(stderr, "pos_enqueue: pq_entry allocation failed\n");
            exit(1);
        }
    out->p = p;

//since out becomes the end of the list, the next entry it points to is NULL
    out->next = NULL;

//case where q is empty, so entry with pos p is the only entry
    if(q->len == 0) {
        q->head = out;
        q->tail = out;
//add entry to the end of the queue, head pointer stays the same
    } else {
        q->tail->next = out;
        q->tail = out;
    }
    q->len++;
}

//remove and return the element at the front of the queue, 
//or raise an error if the queue is empty
//free the removed first element (??)**************
pos pos_dequeue(posqueue* q) {
    if(q == NULL) {
        fprintf(stderr, "pos_dequeue: given empty queue\n");
        exit(1);
    }

    pos res = q->head->p;
    pq_entry* temp = q->head;
    q->head = q->head->next;
    free(temp);
    q->len--;

    if(q->len == 0) {
        q->head = NULL;
        q->tail = NULL;
    }

    return res;
}

//checks if two positions are equal
bool pos_equal(pos p1, pos p2) {
    if(p1.r == p2.r && p1.c == p2.c) {
        return true;
    } else {
        return false;
    }
}

//determine if the given position is in the queue
bool posqueue_member(posqueue* q, pos p) {
    unsigned int length = q->len;
    pq_entry* front = q->head;
    for(unsigned int i = 0; i < length; i++) {
        if(pos_equal(front->p,p)) {
            return true;
        } else {
            front = front->next;
        }
    }
    return false;
}

//entirely deallocate an existing queue
void posqueue_free(posqueue* q) {
    unsigned int length = q->len;
    for(unsigned int i = 0; i < length; i++) {
        pq_entry* tmp = q->head; 
        q->head = q->head->next;
        free(tmp);
    }
    free(q);
}

