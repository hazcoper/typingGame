#include <ncurses.h>
#include <string.h>
#include <unistd.h>  /* only for sleep() */

#define CHAR_LIMIT 15


typedef struct word{
    int pos_x, pos_y;
    bool has_green;
    int counter;
    char text[CHAR_LIMIT];
    char next_letter;
} Word;


Word word_list[30];
int word_count = 0;


int add_word(char word[], int pos_x, int pos_y){
    Word new_word;

    new_word.pos_x = pos_x;
    new_word.pos_y = pos_y;
    new_word.has_green = false;

    new_word.counter = 0;

    strcpy(new_word.text, word);
    new_word.next_letter = new_word.text[new_word.counter];

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

int print_word(int word_index){
    int color_index, aux = 0;
    color_index = -1;
    if (word_list[word_index].has_green == true){
        color_index = word_list[word_index].counter; 

    }
    move(word_list[word_index].pos_x, word_list[word_index].pos_y);
    while(word_list[word_index].text[aux] != '\0'){
        if(aux < color_index){
            attron(COLOR_PAIR(1));
            printw("%c", word_list[word_index].text[aux]);
            attroff(COLOR_PAIR(1));
        }
        else{
            printw("%c", word_list[word_index].text[aux]);
        }
        
        aux += 1;
    }


    return 0;
}

/* Move all of the words one block to the left and checks to see if any have gotten to the limtis */
int move_words(){
    int aux;

    for(aux = 0; aux < word_count; aux++){
        word_list[aux].pos_y -= 1;
    }
    return 0;
}

int check_letter(char letter){
    int aux;
    for (aux = 0; aux < word_count; aux++){
        if(word_list[aux].next_letter == letter){
            word_list[aux].counter += 1;
            word_list[aux].next_letter = word_list[aux].text[word_list[aux].counter];
            word_list[aux].has_green = true;
        }
    }


    return 0;
}

int main(void){
    int aux;
    char my_word[] = "hazcoper";
    char my_word2[] = "test";
    char my_word3[] = "brown";
    char my_word4[] = "jumped";
    char my_word5[] = "lazy";
    char my_word6[] = "dog";

    char pressed_key;
    int counter = 0;
    initscr();

    start_color();
    init_pair(1,COLOR_GREEN,COLOR_BLACK);
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    add_word(my_word, 0,10);
    add_word(my_word2, 10,7);
    add_word(my_word3, 15,2);
    add_word(my_word4, 2,10);
    add_word(my_word5, 7,25);
    add_word(my_word6, 9,14);


    //make de part to draw all of the words

    scrollok(stdscr, TRUE);

    for(aux = 0; aux < word_count; aux++){
        print_word(aux);
    }


    while (1) {
        if (kbhit()) {
            //this means I got a character, incriment counter and see if it is the right word
            pressed_key = getch();

            check_letter(pressed_key);


            erase();
            for(aux = 0; aux < word_count; aux++){
                print_word(aux);
            }
            refresh();

        }
    }
}

