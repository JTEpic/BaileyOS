#include "kernel.h"
#include <stdbool.h>
//#include <stdio.h>

void kernel_main(){
    //printf("skibidi");
    char* video_memory = (char*) 0xb8000;
    *video_memory = 'X';

    bool loop = true;
    while(loop){
        //loop
    }
}