#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "document.h"
#include "log.h"

/*NOTE about pointers:
document->head is NOT the head Node, it's a POINTER to a head node
    ,aka holding the address of the actual head node
when we're making changes to document->head, we're following where it's pointing to, the head Node, and change the head Node
    NO changes are made to the pointer
    for example:
        document->head->next = sth; - we're making changes to the head Node's next 


*/
// Create a line node with an empty gap buffer
Line* line_create() {
    GapBuffer* gbuf = gap_create(DEFAULT_GAP_SIZE);
    return line_create_gap(gbuf);
}
// Create a line node with the given gap buffer
Line* line_create_gap(GapBuffer* gbuf) {
    Line* line = (Line*) malloc(sizeof(Line));
    line->gbuf = gbuf;
    line->next = line->previous = NULL;
    return line;
}

// Make a new document
Document* document_create() {
    Document* document = (Document*) malloc (sizeof(Document));
    //create the head and tail node and link them together to make a doubly linked list
    document->head = document->tail = NULL;              //the head and tail is the same when first initiated
    document->num_lines = 0;
    return document;
}

Line* document_get(Document* document, int row) {
    Line* target = document->head;
    for(int i = 0; i < row; i++){
        target = target->next;
    }
    return target;
}

/*testcases:
    InsertAfter1: check what happens if insert a new line to an empty document
*/
void document_insert_after(Document* document, Line* insertAfter, Line* newline) {
    //make newLine the new head and tail if document is empty?
    if(insertAfter == NULL){
        //making head and tail points to the place newLine is pointing at, a new line Node
        document->head = newline;
        document->tail = newline;
        newline->next = newline->previous = NULL;

    }else{//NOTE: no need to traverse through linked list because the given parameters are already pointing to where we need to be
        Line* insertAfter_next = insertAfter->next;
        insertAfter->next = newline;
        newline->previous = insertAfter;
        newline->next = insertAfter_next;
        //if insertAfter is NOT a tail node, then make its successor's previous become the newLine
        if(insertAfter_next != NULL){
            insertAfter_next->previous = newline;
        }//if insertAfter is a tail node, then make newLine the new tail
        else{
            document->tail = newline;
        }
    }
    document->num_lines ++;
    

}

void document_insert_before(Document* document, Line* insertBefore, Line* newline) {
    //make newLine the new head and tail if document is empty
    if(insertBefore == NULL){
        //making head and tail points to the place newLine is pointing at, a new line Node
        document->head = newline;
        document->tail = newline;
        newline->next = newline->previous = NULL;
    //same as insertAfter but in reverse
    }else{
        Line* insertBefore_prev = insertBefore->previous;
        insertBefore->previous = newline;
        newline->next = insertBefore;
        newline->previous = insertBefore_prev;
        //if insertBefore is NOT a head node, then make its predecessor's next points to newLine
        if(insertBefore->previous != NULL){
            insertBefore_prev->next = newline;
        //if insertBefore is a head node, then make newLine the new head
        }else{
            document->head = newline;
        }
    }
    document->num_lines ++;
}

void document_remove(Document* document, Line* line) {
    //case: line to remove is a head
    if(line == document->head){
        document->head = document->head->next;      //make the head's successor the new head
        document->head->previous = NULL;            //remove the old head by making the new head (successor) points to null
        //free the removed line node?
    }
    //case: line to remove is a tail
    else if(line == document->tail){
        document->tail = document->tail->previous;
        document->tail->next = NULL;
    }
    //case: line to remove is inbetween head and tail
    else{
        line->previous->next = line->next;          //make its predecessor points next to its successor, remove the line by jumping over it
        line->next->previous = line->previous;      //make its successor points back to its predecessor
    }
    document->num_lines --;
}

Document* document_read(const char* filename) {
    // Your code here
    return NULL;
}

bool document_write(Document* document, const char* filename) {
    // Your code here
    return false;
}

void document_print(Document* document) {
    printf("Document %p lines=%d [\n", document, document->num_lines);
    for (Line* line = document->head; line != NULL; line = line->next) {
        printf("\t");
        gap_print(line->gbuf);
    }
    printf("]\n");
}
