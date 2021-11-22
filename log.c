#include <stdio.h>
#include <time.h>
#include <string.h>
#include "log.h"

static FILE* fp = NULL;

void log_debug(const char* message, ...) {
    if (fp == NULL) {
        fp = fopen("log.out", "w");
    }
    time_t now;
    time(&now);
    char * date = ctime(&now);
    date[strlen(date) - 1] = '\0';
    fprintf(fp, "LOG [%s] ", date);

    va_list args;
    va_start(args, message);
    vfprintf(fp, message, args);
    fprintf(fp, "\n");
}
