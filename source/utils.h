#pragma once

#define INV_WHITE "\x1b[47m"
#define BLACK CONSOLE_BLACK
#define RESET CONSOLE_RESET
#define RED CONSOLE_RED
#define GREEN CONSOLE_GREEN
#define BLUE CONSOLE_BLUE
#define YELLOW CONSOLE_YELLOW
#define MAGENTA CONSOLE_MAGENTA
#define CYAN CONSOLE_CYAN

#define MAX_LINES 38

char* keyboard(char* message, size_t size);
void printarraynew(char *array[], int on[], int arraylength, int highlight, int offset, int starty);