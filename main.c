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

    start_color();
    init_pair(1,COLOR_RED,COLOR_BLUE);
    attron(COLOR_PAIR(1));
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);

    printw("word");

    scrollok(stdscr, TRUE);
    init_pair(1, COLOR_BLACK, COLOR_RED);
    while (1) {
        if (kbhit()) {
            printw("%c", getch());
            refresh();
        }
    }
    attroff(COLOR_PAIR(1));
}