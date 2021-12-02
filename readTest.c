#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gap_buffer.h"
#include "document.h"
#include "window.h"
#include "terminal.h"
#include "log.h"
#include "status.h"

int main(){
    const char* filename = "test/input.txt";
    Document* document = document_read(filename);

    for (Line* line = document->head; line != NULL; line = line->next) {
        printf("%s\n", gap_to_string(line->gbuf));
    }



}