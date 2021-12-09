#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gap_buffer.h"
#include "document.h"
#include "window.h"
#include "terminal.h"
#include "log.h"
#include "status.h"
/*RORY: you cant CTRL + F "RORY" to see where I put my comments for you at
the core thing is that I don't know how to debug my terminal when it's running
to see why my cursor is not moving or why breaking line delete everything after the cursor, etc */
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
        //filename = "unamed.txt"; //or *filename?
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
        log_debug("ch= %d\n", ch);
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
        /*RORY - update: I FIXED IT!!: when I'm shifting right, in some lines, you can move right
        , but in some lines, the cursor is stuck and I can't move at all
        there is a few ocasion where it's adding weird chars and the cursor moving by itself without me typing anything
        */
        }else if(ch == KEY_RIGHT){
            //if the cursor (second_position) in the current line is not at the end of the line, can move right
            if(currentline->gbuf->second_position < currentline->gbuf->size){//*second or insert?
                //NOTE: this method below will updata insert and second_position so no need to update them again in main.c
                gap_set_insert_position(currentline->gbuf,currentline->gbuf->insert_position+1);//move right by 1 letter

                /*scroll right if the line is too long for the screen size
                if(currentline->gbuf->second_position - window->first_col == window->width){//*check this condition
                    window->first_col +=1;
                }*/
            }
        }else if(ch == KEY_LEFT){
            //if the cursor (insert_position) in the current line is not at the beginning of the line, can move left
            if(currentline->gbuf->insert_position > 0){
                gap_set_insert_position(currentline->gbuf,currentline->gbuf->insert_position -1);
                
                /*scroll left if the line is too long for the screen size
                if(currentline->gbuf->insert_position == window->first_col -1){
                    window->first_col -=1;
                }*/
            }
        }
        /*user hitting the ENTER_KEY "^J":
            at the end of a line - open up a new line AFTER the current line
                add a new line to the document (document_insert_after())
                change the Window's current line to the new line
            at the beginning of a line - open up a new line BEFORE the current line
                add a new line to the document (document_insert_before())
                change the Window's current line to the new line
            at the middle of a line - get the newLine by calling gap_break()
        NOTE: breaking in the middle delete everything after the gap
            break infront of the first line cause segfault
        */
        /*RORY: FIXED- breaking in the middle delete everything after the gap and the newLine prints out nothing
                NOT FIXED- break infront of the first line cause segfault
                NOT FIXED - when break at the beginning, the cursor move up twice
            
            Narrowing things down:
            when run gdb for middle breaking:
                window->doc->num_lines increases by 1 -> so a new line was inserted
            then newline->data has no value?
            UPDATE: I figured it out! i called line_create(new_gbuf) instead of line_create_gap(new_gbuf)
                line_create() return an empty data so that's why nothing is prints out

        */
        else if(strcmp(terminal_keyname(ch), "^J") == 0){
            Line* newLine;
            if(currentline->gbuf->second_position == currentline->gbuf->size){
                newLine = line_create();
                document_insert_after(window->document,currentline,newLine);
                window->current ++;
            }else if(currentline->gbuf->insert_position == 0){
                newLine = line_create();
                document_insert_before(window->document,currentline,newLine);
                window->current --;
            }else if(currentline->gbuf->insert_position > 0 && currentline->gbuf->second_position < currentline->gbuf->size){//in the middle of the line
                GapBuffer* new_gbuf = gap_break(currentline->gbuf);
                newLine = line_create_gap(new_gbuf);
                //print out the new line to the terminal???
                document_insert_after(window->document,currentline,newLine);
                window->current++;
            }
        }
        
       /*backspace - remove a char
            if insert_position > 0 (not at the beginning of a line) -> delete the char before insert
                use gap_remove_char()
            if insert_position == 0 (at the beginning of a line) -> combine the current line with the previous line
                copy the current string to the previous string using gap_insert_string
                remove the current line from document document_remove()
                NOTE: if currentLine is the first line -> backspace doesn't work
        */
        /*RORY: FIXED (need to update window->current)-when deleting a whole line
                , thr cursor move to the next line, instead of the previous line

        */
        else if(strcmp(terminal_keyname(ch), "^?") == 0 || ch == KEY_BACKSPACE){
            //break; //testing if this condition is executed - if it is, then the program will break
            if(currentline->gbuf->insert_position > 0){
                gap_remove_char(currentline->gbuf);
            }else if(currentline->gbuf->insert_position == 0){

                if(currentline->previous != NULL){ // or currentline != window->document->head
                    char* currentString = gap_to_string(currentline->gbuf);
                    int currentLength = gap_length(currentline->gbuf);
                    gap_insert_string(currentline->previous->gbuf,currentLength,currentString);
                    document_remove(window->document,currentline); //document or window->document?
                    free(currentString);
                    window->current --;//move to the previous line now that the current line is deleted
                }
            }
        }
        
       
        // else{//if the user is entering a character, insert it
        //     gap_insert_char(currentline->gbuf,ch);
        // }

    }

    terminal_end();

    // On quit, write the document to output file here
    //document_write(document,filename);you're writing to the filename, which holds the input file, that's why it's rewriting everything
    //document_write(document,"test/output.txt");//or output.txt?


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
