#include <ncurses.h>
#include <string.h>
#include <unistd.h>  /* only for sleep() */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>



int main(){

    clock_t start, end;
    float time_dif;
    printf("starting the program\n");
    start = clock();
    while(1){
        //check time
        end = clock();
        time_dif = (end - start) / CLOCKS_PER_SEC;
        if (time_dif > 0.1){
            printf("Hello \n");
            start = clock();
        }
    }
    return 0;
}