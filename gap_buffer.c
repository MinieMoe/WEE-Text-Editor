#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gap_buffer.h"


GapBuffer* gap_create() {
    // Your code here
    return NULL;
}

void gap_set_insert_position(GapBuffer* gbuf, int new_position) {
    // Your code here
}

void gap_insert_char(GapBuffer* gbuf, char c) {
    // Your code here
}

void gap_insert_string(GapBuffer* gbuf, int count, const char* s) {
    // Your code here
}

void gap_remove_char(GapBuffer* gbuf) {
    // Your code here
}

char* gap_to_string(GapBuffer* gbuf) {
    // Your code here
    return NULL;
}

int gap_length(GapBuffer* gbuf) {
    // Your code here
    return 0;
}

GapBuffer* gap_break(GapBuffer* gbuf) {
    // Your code here
    return NULL;
}

void gap_print(GapBuffer* gbuf) {
    printf("%p|", gbuf);
    for (int i = 0; i < gbuf->insert_position; i++) {
        printf("%c", gbuf->data[i]);
    }
    printf("[");
    for (int i = gbuf->insert_position; i < gbuf->second_position; i++) {
        printf(" ");
    }
    printf("]");
    for (int i = gbuf->second_position; i < gbuf->size; i++) {
        printf("%c", gbuf->data[i]);
    }
    printf("|\n");
}
