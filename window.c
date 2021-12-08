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

   cur->gbuf->data
        x - column
        y - row
    -get the string of the current line: using gap_to string
    -print out the string: terminal_prints(x, y, string)
    NOTE: don't need to update x
    -update the printing coordinate so we know where
    on the screen to print the next line
        x += gap_length(size of the current line)
    -if x is > width, we move to the next row
        y++
*/
void window_draw(Window* window) {
    //get the first line
    Line* cur = document_get(window->document,window->first_row);
    int x =0;                           //always print lines at col 0
    for (int y = 0; y < window->height; y++){
        if(cur != NULL){
            char* string = gap_to_string(cur->gbuf);
            terminal_prints(x,y,string);
            cur = cur->next;
            free(string);
        }else{//if we reach a tail line, stop printing
            break;
        }
    }
    
}
