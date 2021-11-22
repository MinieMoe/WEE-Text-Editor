#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "document.h"
#include "log.h"

// Create a line node with an empty gap buffer
Line* line_create() {
    GapBuffer* gbuf = gap_create(DEFAULT_GAP_SIZE);
    return line_create_gap(gbuf);
}

Line* line_create_gap(GapBuffer* gbuf) {
    Line* line = (Line*) malloc(sizeof(Line));
    line->gbuf = gbuf;
    line->next = line->previous = NULL;
    return line;
}

// Make a new document
Document* document_create() {
    // Your code here
    return NULL;
}

Line* document_get(Document* document, int row) {
    // Your code here
    return NULL;
}

void document_insert_after(Document* document, Line* insertAfter, Line* newline) {
    // Your code here
}

void document_insert_before(Document* document, Line* insertBefore, Line* newline) {
    // Your code here
}

void document_remove(Document* document, Line* line) {
    // Your code here
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
