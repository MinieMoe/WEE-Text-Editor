#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <stdbool.h>
#include "gap_buffer.h"

// Each line is a node with a gap buffer
typedef struct line {
    GapBuffer* gbuf;
    struct line* next;
    struct line* previous;
} Line;

// Create a line node with an empty gap buffer
Line* line_create();

// Create a line node with the given gap buffer
Line* line_create_gap(GapBuffer* gbuf);


// A document is a doubly linked list of line nodes
typedef struct {
    Line* head;
    Line* tail;
    int num_lines;
} Document;

// Make a new empty document
Document* document_create();

// Get the Line at the given row from the document.
// Iterate the list from the head till you find the right Line.
Line* document_get(Document* document, int row);

// Insert a new Line into the document, after the given node.
// InsertAfter should only be null if the document is empty
// with no lines.
void document_insert_after(Document* document, Line* insertAfter,
    Line* newline);

// Insert a new Line into the document, before the given node.
// InsertBefore should only be null if the document is empty
// with no lines.
void document_insert_before(Document* document, Line* insertBefore,
    Line* newline);

// Remove a line node from the document
void document_remove(Document* document, Line* line);

// Read from a file from the given filename.
// Create a document object and the lines for each line in the file,
// and return the document object.
Document* document_read(const char* filename);

// Write all lines of the document out to the given filename.
// Return true on success, or false if there was an error
bool document_write(Document* document, const char* filename);

// Print the document out. This is very useful for debugging
void document_print(Document* document);

#endif // DOCUMENT_H
