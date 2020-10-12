#include <ncurses.h>
#include <string.h>
#include <unistd.h>  /* only for sleep() */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CHAR_LIMIT 30


typedef struct word{
    int pos_x, pos_y, len;
    bool has_green;
    int counter;
    char text[CHAR_LIMIT];
    char next_letter;
} Word;

FILE *word_file;
WINDOW * win;
Word word_list[30];
int word_count = 0;

int score = 0;
int last_line = 0;

/* Receives a line a determines if a word fits there or not */
bool is_possible(int len, int line){
    int aux1, chr; 
    len = len + 1; 
    //vou receber uma linha, vou palavra a palavra ver se nao tem nada nessa linha
    for(aux1=0; aux1 < word_count; aux1++){
        //nao pode ter nada entre 49-len e 49, portanto se pos_x estiver nesse range ou pos_x + len
        if(word_list[aux1].pos_y == line){
            if(word_list[aux1].pos_x+word_list[aux1].len > 49 - len){
                return false;
                
            }
        }
    }
    //no there is no word in that line
    return true;
}

int generate_random(int l, int r) { //this will generate random number in range l and r
    int i;
   
    int rand_num = (rand() % (r - l + 1)) + l;
   
    return rand_num;
}

int find_line(int len){
    int line, aux, counter;
    //get a random line, check if there is space in that random line, if not try again
    aux = generate_random(1,19);
    counter = 0;
    while(!is_possible(len, aux)){
        //get a random number, set it to aux
        aux = generate_random(1,18);
        counter ++;
        if(counter == 20){
            //means that there is no available line
            return 0;
        }
    }
    return aux;
}


int add_word(){
    Word new_word;
    char aux_string[CHAR_LIMIT];
    int len, aux, aux1, height, y;

    //new_word.pos_x = pos_y;
    new_word.has_green = false;

    new_word.counter = 0;

    fscanf(word_file, "%s", new_word.text);
    len = strlen(new_word.text);
    new_word.len = len;
    
    if(word_count == 0){
        new_word.pos_y = 1;    
    }
    else{
        aux = find_line(len);
        if(aux == 0)
        new_word.pos_y = aux;
    }



    new_word.pos_x = 50 - 1 - len;
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
    wmove(win, word_list[word_index].pos_y, word_list[word_index].pos_x);
    while(word_list[word_index].text[aux] != '\0'){
        if(aux < color_index){
            wattron(win, COLOR_PAIR(1));
            wprintw(win, "%c", word_list[word_index].text[aux]);
            wattroff(win, COLOR_PAIR(1));
        }
        else{
            wprintw(win,"%c", word_list[word_index].text[aux]);
        }
        
        aux += 1;
    }
    

    wrefresh(win);

    return 0;
}


int end_game(){

    while(1){
        wmove(win, 15,15);
        wprintw(win, "END GAME");
        wmove(win, 16,16);
        wprintw(win, "SCORE: %d", score);
        wrefresh(win);
    }
    return 0;
}


/* Move all of the words one block to the left and checks to see if any have gotten to the limtis */
int move_words(){
    int aux;

    for(aux = 0; aux < word_count; aux++){
        word_list[aux].pos_x -= 1;
        if(word_list[aux].pos_x < 2){
            //Game has eneded
            end_game();
        }
    }
    return 0;
}

int check_letter(char letter, int user_counter){
    int aux;
    for (aux = 0; aux < word_count; aux++){
        if(word_list[aux].next_letter == letter){
            if(word_list[aux].counter == user_counter){
                word_list[aux].counter += 1;
                word_list[aux].next_letter = word_list[aux].text[word_list[aux].counter];
                word_list[aux].has_green = true;
            }
        }
        else{
            if(word_list[aux].has_green){
                word_list[aux].has_green = false;
                word_list[aux].counter = 0;
                word_list[aux].next_letter = word_list[aux].text[word_list[aux].counter];
                 
            }
        }
    }
    return 0;
}

int check_completion(){
    int aux, aux1, pos_y;
    for(aux = 0; aux < word_count; aux++){
        if(word_list[aux].counter == strlen(word_list[aux].text)){
            score += 1;
            last_line = word_list[aux].pos_y;

            word_list[aux].pos_y = pos_y;
            
            /* Remove the current word from the list */
            for(aux1 = aux; aux1 < word_count; aux1++){
                word_list[aux1] = word_list[aux1 + 1];
            }
            word_count -= 1;
            werase(win);
            box(win, 0, 0);
            wrefresh(win);
        }
    }

    box(win, 0, 0);
    wrefresh(win);


}



int main(void){
    int aux;
    clock_t start, start1, end, end1;
    float time_dif;
    char pressed_key;
    int counter = 0;


    initscr();


    start_color();
    init_pair(1,COLOR_GREEN,COLOR_BLACK);
    cbreak();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    scrollok(stdscr, TRUE);

    word_file = fopen("word.txt", "r");
    add_word();


    win = newwin(20, 50, 0,0);
    refresh();

    box(win, 0, 0);
    wrefresh(win);



    for(aux = 0; aux < word_count; aux++){
        print_word(aux);
    }

    start = clock();
    start1 = clock();
    while (1) {
        if (kbhit()) {
            //this means I got a character, incriment counter and see if it is the right word
            
            pressed_key = getch();
            if(pressed_key == 32 || pressed_key == 10){
                //spacebar was pressed, check to see if there is any complete word
                counter = -1;
                check_completion();
                add_word();
                
            }
            check_letter(pressed_key, counter);



            wmove(win, 19, counter+1);
            wprintw(win, "%c", pressed_key);
            counter += 1;

        }
        
        for(aux = 0; aux < word_count; aux++){
            print_word(aux);
        }
        wmove(win,1,1);
        wprintw(win,"%d", score);
        wrefresh(win);
        //check time
        end = clock();
        time_dif = (end - start) / CLOCKS_PER_SEC;
        if (time_dif > 0.6){
            move_words();
            start = clock();
            werase(win);
            box(win, 0, 0);

        }

    }
    
    endwin();
}

