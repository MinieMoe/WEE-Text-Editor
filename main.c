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

    // //test
    // readTest();
    // exit(0);

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
        filename = "unamed.txt"; //or *filename?
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
        status_message(0,height-1,"--Wee--");
        
        // Get the current line that the cursor is on
        Line* currentline = document_get(document, window->current);
        // Move the cursor to the correct location on the screen
        terminal_move(currentline->gbuf->insert_position,
            window->current - window->first_row);

        // Show everything to the user all at once
        terminal_refresh();

        /*Wait for user input
            GOAL of this part: updating window->first_row and current so we know what to print for the next iteration
        */
        int ch = terminal_read();
        // Handle user input here
        if (strcmp(terminal_keyname(ch), "^Q") == 0) {
            break; //end the loop that's running the program
        }else if(ch == KEY_DOWN){
            //move the cursor down; make sure cursor doesn't go out of bound
            if(window->current < window->document->num_lines -1){
                window->current ++;
                //if cursor is at the bottom of the screen, scroll down
                if(window->current - window->first_row == window->height){
                    window->first_row ++;
                }
            }
        }else if(ch == KEY_UP){
            //move the cursor up; make sure the cursor doesn't go out of bound
            if(window->current > 0){
                window->current --;
                //if cursor is at the top of the screen, scroll up
                if(window->current == window->first_row - 1){
                    window->first_row --;
                }
            }
        }else if(ch == KEY_RIGHT){
            //if the cursor (second_position) in the current line is not at the end of the line, can move right
            if(currentline->gbuf->second_position < currentline->gbuf->size){//*second or insert?

                //NOTE: this method below will updata insert and second_position so no need to update them again in main.c
                gap_set_insert_position(currentline->gbuf,currentline->gbuf->insert_position+1);//move right by 1 letter

                // //scroll right if the line is too long for the screen size
                // if(currentline->gbuf->second_position - window->first_col == window->width){//*check this condition
                //     window->first_col +=1;
                // }
            }
        }else if(ch == KEY_LEFT){
            //if the cursor (insert_position) in the current line is not at the beginning of the line, can move left
            if(currentline->gbuf->insert_position > 0){
                gap_set_insert_position(currentline->gbuf,currentline->gbuf->insert_position -1);
                
                // //scroll left if the line is too long for the screen size
                // if(currentline->gbuf->insert_position == window->first_col -1){
                //     window->first_col -=1;
                // }
            }
        }
        /*user hitting the ENTER_KEY "^J":
            at the end of a line - open up a new line AFTER the current line
                add a new line to the document (document_insert_after())
                change the Window's current line to the new line
            at the beginning of a line - open up a new line BEFORE the current line
                add a new line to the document (document_insert_before())
                change the Window's current line to the new line
        */
        // }else if(strcmp(terminal_keyname(ch), "^J") == 0){
        //     Line* newLine = line_create();
        //     if(currentline->gbuf->second_position == currentline->gbuf->size){
        //         document_insert_after(window->document,currentline,newLine);
        //     }else if(currentline->gbuf->insert_position == 0){
        //         document_insert_before(window->document,currentline,newLine);
        //     }
        //     currentline = newLine;
        // }else{//if the user is entering a character, insert it
        //     gap_insert_char(currentline->gbuf,ch);
        // }
    }

    terminal_end();

    // On quit, write the document to output file here
    document_write(document,filename);

    /*free everything
    For every line in the document, free line->gbuf->data, then free line->gbuf, then line
    Then free document itself and window
    */
    //Line** headpp = &(window->document->head);
    // Line* cur = window->document->head;
    // while(cur!=NULL){
    //     Line* next = cur->next;
    //     free(cur->gbuf->data);
    //     free(cur->gbuf);
    //     free(cur);
    //     cur = next;
    // }
    // window->document->head = NULL; //is this neccesary?
    // free(window->document);
    // free(window);

    

}
