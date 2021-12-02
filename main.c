#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gap_buffer.h"
#include "document.h"
#include "window.h"
#include "terminal.h"
#include "log.h"
#include "status.h"

int readTest(){
    const char* filename = "testRead.txt";
    Document* document = document_read(filename);
    int i =1;
    for (Line* line = document->head; line != NULL; line = line->next) {
        printf("Line %d: %s\n",i, gap_to_string(line->gbuf));
        i++;
    }



}
int main(int argc, char* argv[]) {

    //test
    readTest();
    exit(0);

    Document* document = NULL;
    char* filename = NULL;
    if (argc > 1) {
        // Read filename if provided
        filename = argv[1];
        document = document_read(filename);
    } else {
        // Otherwise create a new document with one empty line
        document = document_create();
        Line* line = line_create();
        document_insert_after(document, document->tail, line);
    }

    // Initialize the terminal screen
    terminal_init();
    // Get the width and height of the terminal so we know how much to display
    int width;
    int height;
    terminal_size(&width, &height);

    // Create a window which is a view of the document.
    // Reduce the height by 1 so that we can show the status bar at the bottom
    Window* window = window_create(document, width, height - 1);
    for (;;) {
        // Clear the screen
        terminal_clear();

        // Draw the contents of the window
        window_draw(window);
        // Draw the status bar message here
        // ...

        // Get the current line that the cursor is on
        Line* currentline = document_get(document, window->current);
        // Move the cursor to the correct location on the screen
        terminal_move(currentline->gbuf->insert_position,
            window->current - window->first_row);

        // Show everything to the user all at once
        terminal_refresh();

        // Wait for user input
        int ch = terminal_read();
        // Handle user input here
        // ...
    }

    terminal_end();

    // On quit, write the document to output file here
}
