#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>

struct termios origin_termios; // backup copy

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &origin_termios);
}

void enableRawMode() {
    tcgetattr(STDIN_FILENO, &origin_termios); //Puts the configuration of the terminal in the struct raw.
    atexit(disableRawMode); //Calls the function when the program exits.
    
    struct termios raw;
    raw = origin_termios;
    
    raw.c_lflag &= ~(ECHO); // The terminal won't print what I'm writing. NOT bit and then AND to put it back!
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw); //Puts the configuration back on the terminal.
    // TCSAFLUSH discards any input that is already waiting to be read.
}


int main(int argc, char* argv[]) { 
    enableRawMode(); //Keyboard starts in cooked mode or in other words, canonical mode, which means input is only sent to my program after pressing enter. This is not useful for text editors, we've got to change it.

    char c;                        
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
        
    }

    return 0;
}

