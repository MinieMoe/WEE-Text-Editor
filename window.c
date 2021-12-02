#include <stdio.h>
#include <stdlib.h>
#include "window.h"
#include "terminal.h"
#include "log.h"


Window* window_create(Document* document, int width, int height) {
    Window* window = (Window*) malloc (sizeof(Window));
    window->document = document;
    window->width = width;
    window->height = height;
    window->current = 0;
    window->first_row = 0;
    window->first_col= 0;

    return window;
}

/*Drawing out the document
    row: moving down the lines - number of lines
    column: moving through each letters in a line - gap_length

    Window shows what's currently on screen
        start to print from first row of the window
        the number of lines to print is the same as the height of the window
        if the current line to print is a tail, stop printing

*/
void window_draw(Window* window) {
    
}
