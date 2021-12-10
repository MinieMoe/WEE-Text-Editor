#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gap_buffer.h"


/*When to use malloc:
Need stack? Do you want local var? Is the var on stack fixed size? Yes, then on stack
global? Mostly always no
malloc on Heap? Will the var be used throughout the program? Is the size flexible? Yes, then on Heap
*/
GapBuffer* gap_create() {
    GapBuffer* buffer = (GapBuffer*)malloc(sizeof(GapBuffer));
    buffer->size = DEFAULT_GAP_SIZE;
    buffer->data = (char*)malloc(sizeof(char)*buffer->size);
    /*Note about gap difference:
        when it comes to check how many numbers in a range between 2 numbers,
        1 number is inclusive and the other is exlcusive
        in Python: range(1,4) - 1 is inclusive, 4 is exclusive
        The goal is just to simplify the math to calculate the differernce between 2 numbers
    */
    buffer->insert_position = 0;                                    //inclusive in gap
    buffer->second_position = DEFAULT_GAP_SIZE;                     //exclusive in gap

    /*Note about size of gap buffer vs gap size
        size of gap buffer = size of the char array data
        gap size = second_position - insert_postion
    */
    return buffer;
}

/*Moving cursor/gap to the left or right
    cursor/gap can only move by 1 letter at a time? No

*/
void gap_set_insert_position(GapBuffer* gbuf, int new_position) {
    int gapSize = gbuf->second_position - gbuf->insert_position;
    int substringSize;                                              //size of the substring need to move
    int old_insert_position = gbuf->insert_position;                //keep the original position before updating
    int old_second_position = gbuf->second_position;

    //if the gap buffer is all filled up (no gap), them simply just set move insert and second_position to new_position
    if(gbuf->insert_position == gbuf->second_position){
        gbuf->insert_position = new_position;
        gbuf->second_position = new_position;
    }
    //moving right
    else if(gbuf->insert_position < new_position){

        //new_position is outside of gap/cursor -> update second_position = new_position + 1
        if(new_position >= gbuf->second_position){
            substringSize = new_position - gbuf->second_position + 1;           //the number of chars that need to move so that gap can move to new_position
            //moving gap by updating insert
            gbuf->insert_position = gbuf->insert_position + substringSize;      //move gap right after where substring is now
        
        //new_position is inside gap/cursor -> update insert_position = new_position
        }else if(new_position < gbuf->second_position){
            substringSize = new_position - gbuf->insert_position;
            gbuf->insert_position = new_position;
        }
        gbuf->second_position = gbuf->insert_position + gapSize;                //moving gap by updating second
        memcpy(&(gbuf->data[old_insert_position]),&(gbuf->data[old_second_position]), substringSize);
        memset(&(gbuf->data[gbuf->insert_position]),' ',gapSize);               //clear anything letters within the gap: these letters are from the substring at the old position

        /*swapping places of substring and gap  
           moving the letters after gap (substring) to where insert_position is
           delete the substring at its old place
        */
        
    //moving left
    }else{
        substringSize = gbuf->insert_position - new_position;
        //move gap/cursor to the new position
        gbuf->insert_position = new_position;
        gbuf->second_position = gbuf->insert_position + gapSize;
        //moving substring right after gap/cursor
        memcpy(&(gbuf->data[gbuf->second_position]),&(gbuf->data[new_position]),substringSize);
        memset(&(gbuf->data[gbuf->insert_position]),' ',gapSize);
    }
}

void gap_insert_char(GapBuffer* gbuf, char c) {
    
    /* If we run out of space in the gap to insert more char, then grow the array size by using realloc
        realloc - re-allocate a memory block that was already allocated with new size
    */
    if(gbuf->second_position == gbuf->insert_position){     //gap is all filled up
        //grow the array size
        int oldSize = gbuf->size;
        gbuf->size += DEFAULT_GAP_SIZE;                     //expanding the size of gbuf
        
        /* previously use realloc to expand the size of gbuf->data
        without having to reinitializing char* data
            but realloc is dangerous so avoid using it      
            gbuf->data = (char*) realloc(gbuf->data,gbuf->size);
        */
        
        char* new_data = (char*) malloc (sizeof(char)* gbuf->size);       //initializing a new char data with the expanded storage
        /*Note: strncpy() copy the entire string until it reaches a null terminator
            however, our gbuf->data is not a null terminated string, can't use
            strncpy() to copy the whole string
                strncpy(new_data,gbuf->data,strlen(gbuf->data));    //copying the data from old data to the new one
            use memcpy() instead

            and also because gbuf->data is not a nul terminated string
            can't use strlen() to check the length of gbuf->data
            use gap_length()
        */
        memcpy(new_data,gbuf->data,oldSize);                               //copy data to the new expanded string (VALGRIND leaks here - solution, use oldSize instead of gap_length() because the size of gbuf->data is oldsize )
        free(gbuf->data);                                                 //free old data array
        gbuf->data = new_data;

        //grow the gap size now that more space is available
        /*expanding the gap by moving second_postion to the a new position
            how do we know the new posistion? the difference bewtween the end of the array and the new position = number of letters after the gap

            Hel|lo               - array when run out space, insert = second; lettes after gap are "lo"
            Hel|lo{     }        - array after adding more space: NOTE: {} indicate new space (to avoid confusion with gap indicated by [])
            Hel[     ]lo         - update the gap size by updating second_postion to a new location
                                  such that the difference between new location and end of array = number of letter after gap "lo"
                                  then move "lo" into the new second_position and delete "lo" at the old position by clearing anything within the gap
        */
        if(gbuf->second_position != oldSize -1){                                            //if second_position is already at the end of the old array (before expanding), aka, no letters after the gap
                                                                                            //, then there's no letters to move to the end
            int afterGap_substring_size = oldSize - gbuf->second_position;                  //number of letters after the gap/cursor     
            gbuf->second_position = gbuf->size - afterGap_substring_size;                   //update second_position
            memcpy(&(gbuf->data[gbuf->second_position]),&(gbuf->data[gbuf->insert_position]),afterGap_substring_size);//move letters after the cursor (at insert_pos) to the new place now that gap is expanded
            /*memset(&(gbuf->data[gbuf->insert_position]),' ',gbuf->second_position - gbuf->insert_position);
            clearing letters inside the gap is unnecsaary because when printing out data, everything inside gap is ignored and not printed.
            */
        }else{
            gbuf->second_position = gbuf->size;
        }
    }
    //if gap is not filled up, simply inserting the new char at the insert position and then update the insert_position
    gbuf->data[gbuf->insert_position] = c;
    gbuf->insert_position +=1;
}

void gap_insert_string(GapBuffer* gbuf, int count, const char* s) {
    for(int i = 0; i < count; i ++){
        gap_insert_char(gbuf,s[i]);
    }
}

void gap_remove_char(GapBuffer* gbuf) {
    gbuf->data[gbuf->insert_position - 1] = ' ';
    gbuf->insert_position -=1;
}

char* gap_to_string(GapBuffer* gbuf) {
    int stringSize = gap_length(gbuf) + 1;                      //number of chars in array data (no gap), 1 extra space for null terminator
    char* string = (char*) malloc(sizeof(char)*stringSize);
    
    //putting char from data to string, exlcuding the gap
    for(int s = 0, gb = 0; s < stringSize - 1 && gb < gbuf->size; gb++){
        if(gb >= gbuf->insert_position && gb < gbuf->second_position){
            continue;
        }
        string[s] = gbuf->data[gb];
        s++;                                                        //only increment s when a char is put into string
    }
    //add null terminator at the end of string
    string[stringSize-1] = '\0';

    return string;
}

int gap_length(GapBuffer* gbuf) {
    int gapSize = gbuf->second_position - gbuf->insert_position;
    int stringLength = gbuf->size - gapSize;
    return stringLength;
}

GapBuffer* gap_break(GapBuffer* gbuf) {
    //initialize new GapBuffer
    GapBuffer* new_gbuf = gap_create();
    int afterGap_letters = gbuf->size - gbuf->second_position;

    //copying the chars after the gap (starting at second_position) into the new_gbuf
    char subString[afterGap_letters];
    memcpy(subString,&(gbuf->data[gbuf->second_position]),afterGap_letters);
    gap_insert_string(new_gbuf,afterGap_letters,subString);

    //remove substring from gbuf and expand the gap
    //memset(&(gbuf->data[gbuf->second_position]),' ',afterGap_letters);
    gbuf->second_position = gbuf->size;
    /*NOTE: instead of using the C library, use the method in this class to remove and insert chars into gap buffer
        strncpy(new_gbuf->data,&(gbuf->data[gbuf->second_position]),afterGap_letters);
        new_gbuf->insert_position = afterGap_letters;
        //now that we moved the chars after gap to new gbuf, delete them at the old gbuf
        memset(&(gbuf->data[gbuf->second_position]),' ',afterGap_letters);
    */
    
    return new_gbuf;
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
