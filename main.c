#include <ncurses.h>
#include <unistd.h>  /* only for sleep() */

int kbhit(void){
    int ch = getch();

    if (ch != ERR){
        ungetch(ch);
        return 1;
    } 
    else{
        return 0;
    }
}

int main(void){
    initscr();

    cbreak();
    noecho();
    nodelay(stdscr, TRUE);

    scrollok(stdscr, TRUE);
    while (1) {
        if (kbhit()) {
            printw("%c", getch());
            refresh();
        }
    }
}