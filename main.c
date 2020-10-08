#include <ncurses.h>
#include <string.h>
#include <unistd.h>  /* only for sleep() */

#define CHAR_LIMIT 15


typedef struct word{
    int pos_x, pos_y;
    char text[CHAR_LIMIT];
} Word;

Word word_list[30];
int word_count = 0;


int add_word(char word[], int pos_x, int pos_y){
    Word new_word;

    new_word.pos_x = pos_x;
    new_word.pos_y = pos_y;
    printw("%s", word);
    //strcpy(new_word.text, word);
    word_list[word_count] = new_word;
    word_count += 1;
    return 0;
}

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
    int aux;
    char my_word[] = "hazcoper";
    char my_word2[] = "test";
    char my_word3[] = "brown";
    char my_word4[] = "jumped";
    char my_word5[] = "lazy";
    char my_word6[] = "dog";

    add_word(my_word, 0,0);
    add_word(my_word2, 10,7);
    add_word(my_word3, 15,2);
    add_word(my_word4, 2,10);
    add_word(my_word5, 30,15);
    add_word(my_word6, 9,14);

    for(aux = 0; aux < word_count; aux++){
        printw("%s", word_list[aux].text);
    }

    char pressed_key;
    int counter = 0;
    initscr();

    start_color();
    init_pair(1,COLOR_GREEN,COLOR_BLACK);
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);

    //make de part to draw all of the words

    scrollok(stdscr, TRUE);
    while (1) {
        if (kbhit()) {
            //this means I got a character, incriment counter and see if it is the right word
            pressed_key = getch();
            if(my_word[counter] == pressed_key){
                attron(COLOR_PAIR(1));
                move(0, counter);
                printw("%c", pressed_key);
                attroff(COLOR_PAIR(1));
                counter = counter + 1;
            }
            refresh();
        }
    }
}