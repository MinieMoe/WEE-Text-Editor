#ifndef WINDOW_H
#define WINDOW_H

#include <stdbool.h>
#include "document.h"

// A window into a document
typedef struct {
    // The document itself that this window represents
    Document* document;

    // The size of the window
    int width;
    int height;

    // The first row and column being shown
    int first_row;
    int first_col;

    // The current line we are editing
    int current;
} Window;


// Create a Window
Window* window_create(Document* document, int width, int height);

// Draw the window
void window_draw(Window* window);

#endif // WINDOW_H
