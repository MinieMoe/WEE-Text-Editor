#include <stdio.h>
#include <ncurses.h>
#include "terminal.h"



void terminal_end() {
    endwin();
}

void terminal_init() {
    initscr();                      // start curses mode
    raw();                          // disable line buffering
    keypad(stdscr, TRUE);           // turn on arrow keys
    noecho();                       // don't echo to the screen
    timeout(1000);                   // 100 millisecond timeout
}

void terminal_refresh() {
    refresh();
}

void terminal_clear() {
    erase();
}

void terminal_size(int* x, int* y) {
    getmaxyx(stdscr, *y, *x);
}

void terminal_move(int x, int y) {
    move(y, x);
}

void terminal_printc(int x, int y, char c) {
    mvaddch(y, x, c);
}

void terminal_prints(int x, int y, const char* s) {
    mvaddstr(y, x, s);
}

void terminal_prints_reverse(int x, int y, const char* s) {
    attron(A_REVERSE);
    terminal_prints(x, y, s);
    attroff(A_REVERSE);
}

int terminal_read() {
    return getch();
}

const char* terminal_keyname(int ch) {
    return keyname(ch);
}
