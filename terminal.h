#ifndef TERMINAL_H
#define TERMINAL_H

#include <ncurses.h>                      // include to give key codes

// Initialize raw mode in the terminal
void terminal_init();

// Reset to canonical mode in the terminal
void terminal_end();

// Clear the screen
void terminal_clear();

// Refresh the screen to show all the things you have drawn on it
void terminal_refresh();

// Return the size of the screen in the x and y values given
void terminal_size(int* x, int* y);

// Move the cursor
void terminal_move(int x, int y);

// Draw a character at the position
void terminal_printc(int x, int y, char c);

// Draw a string at the given x and y position
void terminal_prints(int x, int y, const char* s);

// Print a string in reverse (black on white) text in the given x and y position.
// This is useful for printing the status bar in reverse text
void terminal_prints_reverse(int x, int y, const char* s);

// Read a character from the terminal.
// This value returns -1 if the user did not type anything after a 1 second timeout.
// If the user types any key, the key value is returned straightaway.
int terminal_read();

// Change keys to name strings.
// - Enter is given as "^J"
// - Control-Q is given as "^Q"
// - Backspace is given as "^?"
// For example, you can check if a key value ch (read from terminal_read)
// is the Enter key with
//     if (strcmp(terminal_keyname(ch), "^J") == 0) { ... }
const char* terminal_keyname(int ch);

#endif // TERMINAL_H
