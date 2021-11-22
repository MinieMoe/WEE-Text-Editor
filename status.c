#include "status.h"
#include "terminal.h"

void status_message(int x, int y, const char* s) {
    terminal_prints_reverse(x, y, s);
}
