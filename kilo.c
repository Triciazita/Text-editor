/*** includes ***/
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <ctype.h>
#include <errno.h>

/*** data  ***/
struct termios origin_termios; // backup copy

/*** terminal ***/
void die(const char* s) {
    perror(s);
    exit(1);
}

void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &origin_termios) == -1) {
        die("tcsetattr");
    }

}

void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &origin_termios) == -1) {//Puts the configuration of the terminal in the struct raw.
        die("tcgetattr");
    }
    atexit(disableRawMode); //Calls the function when the program exits.
    
    struct termios raw;
    raw = origin_termios;
    
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON); //Deactivates ctrl c, We have to deactivate carriage return because any carriage return is being turned into new lines!
    raw.c_oflag &= ~(OPOST); //To make a new text line we need a carriage return and a new line \r\n . Deactivates all post-processing of output, like the transformation of \n into \r\n
    raw.c_cflag &= ~(CS8);
    raw.c_lflag &= ~(ECHO | ICANON| ISIG| IEXTEN); // The terminal won't print what I'm writing. NOT bit and then AND to put it back! ICANON deactivates canonical mode and ISIG Ctrl C and Ctrl Z from terminating and suspending the program. IEXTEN disables ctrl v
    raw.c_cc[VMIN] = 0; //cc stands for control characters, the minimum number of bytes that need to be read before read returns.
    raw.c_cc[VTIME] = 1; // Maximum time read waits in tenths of a second. It is currently in 1/10 so 100 miliseconds.


    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) { //Puts the configuration back on the terminal.
        die("tcsetattr");                                 // TCSAFLUSH discards any input that is already waiting to be read.
    }
}

/*** init ***/
int main(int argc, char* argv[]) { 
    enableRawMode(); //Keyboard starts in cooked mode or in other words, canonical mode, which means input is only sent to my program after pressing enter. This is not useful for text editors, we've got to change it.
                       
    while (1) {
        char c = '\0';
        if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) { // when read times out, it returns EAGAIN
            die("read");
        }

        if (iscntrl(c)) { //If it is a control character
            printf("%d\r\n", c);
        }
        else {
            printf("%d (%c)\r\n", c, c);
        }

        if (c == 'q') {
            break;
        }
        
    }

    return 0;
}

